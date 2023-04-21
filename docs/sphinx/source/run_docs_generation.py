#!/usr/bin/env python3

# Copyright Amazon.com, Inc. or its affiliates. All Rights Reserved.
# SPDX-License-Identifier: Apache-2.0.
import argparse
import concurrent.futures
import fileinput
import multiprocessing
import os
import shutil
import subprocess
from pathlib import Path

import jinja2

from doxygen.doxygen_wrapper import DoxygenWrapper, DOXYGEN_OUTPUT_DIR

# Although Sphinx is a Python app, it is executed as a separate standalone app
SPHINX_BUILD_EXE = shutil.which("sphinx-build")

def parse_arguments() -> dict:
    """Parse arguments and/or print help

    :return: dict with parsed arguments
    """
    parser = argparse.ArgumentParser(description="Docs generation wrapper for AWS-SDK-CPP")
    parser.add_argument("--sdk_version", type=str, help="Current SDK version", default=None)
    parser.add_argument("--sdk_root", type=str, help="Path to the SDK repo root",
                        default=str(Path(__file__).parents[3]))
    args = vars(parser.parse_args())

    return args


class DocsGenerator(object):
    def __init__(self, sdk_version, sdk_root):
        self.sdk_root = sdk_root
        self.sphinx_source_dir = os.path.abspath(os.path.join(sdk_root, "docs", "sphinx", "source"))
        self.docs_sphinx_dir = os.path.abspath(os.path.join(sdk_root, "docs", "sphinx"))
        self.template_loader = jinja2.FileSystemLoader(searchpath=f"{self.sphinx_source_dir}/rst_templates")
        self.template_env = jinja2.Environment(loader=self.template_loader, autoescape=True)
        self.doxygen_wrapper = DoxygenWrapper(sdk_version=sdk_version, sdk_root=sdk_root, output_dir=DOXYGEN_OUTPUT_DIR)
        self.thread_pool = concurrent.futures.ThreadPoolExecutor(max_workers=max(multiprocessing.cpu_count() - 1, 1))

        self.components = {"core": ["aws-cpp-sdk-core"]}
        libs = [lib for lib in os.listdir(f"{sdk_root}/src") if lib != "aws-cpp-sdk-core"]
        self.components.update({"libs": libs})
        clients = [client for client in os.listdir(f"{sdk_root}/generated/src")]
        self.components.update({"clients": clients})

    def generate_main_index(self):
        # Generate high-level index aka fake Sphinx toctree
        # api / main_index / index.rst
        shutil.rmtree(f"{self.sphinx_source_dir}/api/main", ignore_errors=True)
        os.makedirs(f"{self.sphinx_source_dir}/api/main", exist_ok=True)
        for entry in ["index", "core", "libs", "clients"]:
            template = self.template_env.get_template(f"main_{entry}.rst.jinja")
            rendered_entry = template.render()
            with open(f"{self.sphinx_source_dir}/api/main/{entry}.rst", mode="w", encoding="utf-8") as module_rst:
                module_rst.write(rendered_entry)

        template = self.template_env.get_template(f"main_module_ext_link.rst.jinja")
        for group, components in self.components.items():
            os.makedirs(f"{self.sphinx_source_dir}/api/main/{group}", exist_ok=True)
            for component in components:
                os.makedirs(f"{self.sphinx_source_dir}/api/main/{group}/{component}", exist_ok=True)
                with open(f"{self.sphinx_source_dir}/api/main/{group}/{component}/module.rst", mode="w", encoding="utf-8") as module_rst:
                    rendered_module = template.render(t_group=group, t_component=component)
                    module_rst.write(rendered_module)

    def _generate_component_index(self, group, component):
        # Copy generic main toctree and replace individual toctree of a specific client with a full sub-toctree
        shutil.rmtree(f"{self.sphinx_source_dir}/api/volumes/{component}", ignore_errors=True)
        shutil.copytree(f"{self.sphinx_source_dir}/api/main",
                        f"{self.sphinx_source_dir}/api/volumes/{component}")
        shutil.rmtree(f"{self.sphinx_source_dir}/api/volumes/{component}/{group}/{component}", ignore_errors=True)

        shutil.move(f"{self.sphinx_source_dir}/api/full_doxy_index/{group}/{component}",
                    f"{self.sphinx_source_dir}/api/volumes/{component}/{group}")

    def generate_all_components_index(self):
        self.doxygen_wrapper.generate_apidoc(f"{self.sphinx_source_dir}/api/full_doxy_index")

        thread_pool = concurrent.futures.ThreadPoolExecutor(max_workers=max(multiprocessing.cpu_count() - 1, 1))
        futures = set()
        os.makedirs(f"{self.sphinx_source_dir}/api/volumes", exist_ok=True)
        for group, components in self.components.items():
            for component in components:
                futures.add(thread_pool.submit(self._generate_component_index, group=group, component=component))
        for future in futures:
            future.result()

        # shutil.rmtree(f"{self.sphinx_source_dir}/api/main", ignore_errors=True)
        # shutil.rmtree(f"{self.sphinx_source_dir}/api/full_doxy_index", ignore_errors=True)

    @staticmethod
    def _call_sphinx(command: str, env: dict = None, cwd: str = os.getcwd(), timeout: int = 3600):
        """A wrapper to call doxygen executable with SDK config
        with arguments of this method to be set as environment variables
        """
        sphinx_cmd = ["/usr/local/bin/python3", "sphinx-build-3"]
        sphinx_cmd += command.split(" ")
        sphinx_cmd_str = str(sphinx_cmd).replace("', '", " ")

        print(f"\n\nRUNNING SPHINX:\n\n{sphinx_cmd_str}\n")
        for key, val in env.items():
            print(f"    export {key}={val}")
        print("\n\n\n")

        try:
            process = subprocess.run(sphinx_cmd, env=env, cwd=cwd, timeout=timeout, check=False, capture_output=False)
        except subprocess.CalledProcessError as exc:
            print(f"Failed to call sphinx: {exc},\n{exc.stdout},\n{exc.stderr}")
            # raise exc
        if process and process.stdout:
            output = process.stdout.decode()
        else:
            output = ""
        return output

    def _generate_single_component_html(self, group, component, dependency_map):
        print(f"!!!! GENERATING {group} / {component}")

        fake_tmp_source_dir = f"{self.docs_sphinx_dir}/source-{component}"

        shutil.rmtree(fake_tmp_source_dir, ignore_errors=True)
        shutil.copytree(f"{self.docs_sphinx_dir}/source",
                        fake_tmp_source_dir)

        shutil.copytree(f"{self.docs_sphinx_dir}/api/volumes/{component}",
                        f"{fake_tmp_source_dir}/api")

        env = {"DOCS_SDK_ROOT": self.sdk_root,
               "DOCS_DOXYGEN_OUTPUT_DIR": f"{self.sdk_root}/{self.doxygen_wrapper.output_dir}",
               "DOCS_COMPONENT": component,
               "DOCS_BASE_BUILD_DIR": "../../build/ALL"
               }

        deps = dependency_map.get(component, None)
        if deps:
            env["DOCS_DEPENDENCIES"] = ";".join(deps)

        sphinx_command = f"-a -b html ./source-{component} ../build/ALL/{component}"
        sphinx_out = self._call_sphinx(sphinx_command, env, cwd=self.docs_sphinx_dir)
        shutil.rmtree(fake_tmp_source_dir, ignore_errors=True)

        return sphinx_out

    def generate_html(self, dependency_map):
        shutil.rmtree(f"{self.sphinx_source_dir}/../api", ignore_errors=True)
        shutil.move(f"{self.sphinx_source_dir}/api",
                    f"{self.sphinx_source_dir}/../api")

        thread_pool = concurrent.futures.ThreadPoolExecutor(max_workers=4)
        for group in ["core", "clients", "libs"]:
            futures = dict()
            for component in self.components[group]:
                futures[component] = thread_pool.submit(self._generate_single_component_html,
                                                        group=group,
                                                        component=component,
                                                        dependency_map=dependency_map)

            for name, future in futures.items():
                future.result()
                print(f"\n\n!!!! Done {name}\n\n\n")

    def merge_html(self, src, dst):
        shutil.rmtree(f"{dst}", ignore_errors=True)
        shutil.copytree(f"{src}/aws-cpp-sdk-core", f"{dst}")
        shutil.rmtree(f"{dst}/.doctrees", ignore_errors=True)
        shutil.rmtree(f"{dst}/_sources", ignore_errors=True)
        os.remove(f"{dst}/objects.inv")
        os.remove(f"{dst}/.buildinfo")

        def _merge_component(grp, comp):
            shutil.rmtree(f"{dst}/api/{grp}/{comp}", ignore_errors=True)
            try:
                shutil.copytree(f"{src}/{comp}/api/{grp}/{comp}", f"{dst}/api/{grp}/{comp}")
            except Exception as exc:
                print(f"Failed to merge component {grp}/{comp}: {exc}")

        futures = set()
        for group in ["clients", "libs"]:
            for component in self.components[group]:
                futures.add(self.thread_pool.submit(_merge_component, grp=group, comp=component))
        for future in futures:
            future.result()


def main():
    args = parse_arguments()
    generator = DocsGenerator(args.get("sdk_version"), args.get("sdk_root"))
    generator.generate_main_index()
    dependency_map = generator.doxygen_wrapper.generate_all()
    generator.generate_all_components_index()

    generator.generate_html(dependency_map)

    generator.merge_html(f"{generator.sdk_root}/docs/build/ALL", f"{generator.sdk_root}/docs/build/merged_docs")

if __name__ == '__main__':
    main()
