#!/usr/bin/env python3

# Copyright Amazon.com, Inc. or its affiliates. All Rights Reserved.
# SPDX-License-Identifier: Apache-2.0.
import concurrent
import concurrent.futures
import fileinput
import multiprocessing
import os
import re
import shutil
import subprocess

import jinja2

DOXYGEN_EXE = shutil.which("doxygen")
CMAKE_EXE = shutil.which("cmake3")
if not CMAKE_EXE:
    CMAKE_EXE = shutil.which("cmake")

DOXYGEN_OUTPUT_DIR = "docs/build/doxygen"

APIDOC_MODULE_RST_REFERENCE = \
"""{{ t_module_name }}
==========

.. toctree::
   :maxdepth: 1

{% for t_module_rst in t_module_rsts -%}
{% raw %}   {% endraw %}{{ t_module_rst }}
{% endfor %}

"""


class DoxygenWrapper(object):
    def __init__(self, sdk_version, sdk_root):
        if not DOXYGEN_EXE:
            raise FileNotFoundError("Doxygen executable is missing!")
        self.doxygen_version = re.search("""^\d+\.\d+\.\d+""", self._call_doxygen("--version"))[0]
        if int(self.doxygen_version.split(".")[0]) != 1 or int(self.doxygen_version.split(".")[1]) < 9:
            raise Exception(f"Invalid doxygen version: {self.doxygen_version}. Expected 1.9 at least.")

        self.configuration_file = os.path.dirname(os.path.abspath(__file__)) + "/config/Doxyfile-prj.cfg"
        if sdk_version:
            self.sdk_version = sdk_version
        else:
            self.sdk_version = self._read_sdk_version(sdk_root)
        self.sdk_root = sdk_root

        self.components_in_src = set(os.listdir(sdk_root + "/src"))
        self.components_in_src.remove("aws-cpp-sdk-core")

    @staticmethod
    def _read_sdk_version(sdk_root: str) -> str:
        """Returns current SDK version from a VersionConfig.h
        :param sdk_root (str): filepath (absolute or relative) to the sdk root dir
        """
        with open(sdk_root + "/src/aws-cpp-sdk-core/include/aws/core/VersionConfig.h") as version_config_file:
            version_config = version_config_file.read()
            m_major = re.search("#define AWS_SDK_VERSION_MAJOR (?P<version_major>\d+)", version_config)
            m_minor = re.search("#define AWS_SDK_VERSION_MINOR (?P<version_minor>\d+)", version_config)
            m_patch = re.search("#define AWS_SDK_VERSION_PATCH (?P<version_patch>\d+)", version_config)

            return f"{m_major}.{m_minor}.{m_patch}"

        return "UNKNOWN_VERSION"

    @staticmethod
    def _call_doxygen(command: str, env: dict = None, cwd: str = os.getcwd(), timeout: int = 180):
        """A wrapper to call doxygen executable with SDK config
        with arguments of this method to be set as environment variables
        """
        doxygen_cmd = [DOXYGEN_EXE]
        doxygen_cmd += command.split(" ")
        try:
            process = subprocess.run(doxygen_cmd, env=env, cwd=cwd, timeout=timeout, check=True, capture_output=True)
        except subprocess.CalledProcessError as exc:
            print(f"Failed to call doxygen: {exc},\n{exc.stdout},\n{exc.stderr}")
            raise exc
        output = process.stdout.decode().replace("\n", "")
        return output

    def _create_dependency_map(self):
        os.makedirs(self.sdk_root + "/tmp_deps_map_build", exist_ok=True)
        subprocess.run([CMAKE_EXE,
                        "-GNinja",
                        "-Btmp_deps_map_build",
                        "--graphviz=tmp_deps_map_build/aws-cpp-sdk.dot",
                        "."],
                       cwd=self.sdk_root)

        # Format dot file
        subprocess.run([("cat tmp_deps_map_build/aws-cpp-sdk.dot | "
                         "grep \"\-> aws-cpp\|\-> aws-crt-cpp\" | "
                         "grep -v \"test\" | "
                         "grep -v \"sample\" | "
                         "sed -e\"s/.*\/\///g\" > tmp_deps_map_build/aws-cpp-sdk-formatted.dot")],
                       cwd=self.sdk_root,
                       shell=True)

        dependency_map = {}
        for line in fileinput.input(self.sdk_root + "/tmp_deps_map_build/aws-cpp-sdk-formatted.dot", inplace=True):
            line = line.replace(" ", "")
            dependency_tuple = line.split('->')
            if "crt" in dependency_tuple[1]:
                dependency_map[dependency_tuple[0]] = []
            elif dependency_tuple[0] in dependency_map:
                dependency_map[dependency_tuple[0]].append(dependency_tuple[1].strip())
            else:
                dependency_map[dependency_tuple[0]] = [dependency_tuple[1].strip()]

        # subprocess.run(["rm -rf tmp_deps_map_build"], shell=True, cwd=self.sdk_root)
        return dependency_map

    def _get_src_path(self, component_name):
        if component_name == "aws-cpp-sdk-core" or component_name in self.components_in_src:
            return f"src/{component_name}"
        else:
            return f"generated/src/{component_name}"

    def _get_doc_path(self, component_name):
        if component_name == "aws-cpp-sdk-core":
            return f"{DOXYGEN_OUTPUT_DIR}/sdk_core/{component_name}"
        elif component_name in self.components_in_src:
            return f"{DOXYGEN_OUTPUT_DIR}/sdk_libraries/{component_name}"
        else:
            return f"{DOXYGEN_OUTPUT_DIR}/generated/{component_name}"

    def _get_tagfiles_dependency(self, component_name, dependency_map):
        separator = " \\\n                         "
        return separator.join(list(map(self._get_doc_path, dependency_map.get(component_name, []))))

    def generate_component_xml(self, client_name, client_dir, output_dir, tagfiles):
        os.makedirs(self.sdk_root + "/" + output_dir, exist_ok=True)
        export_macro = ""
        if client_name.startswith("aws-cpp-sdk-"):
            export_macro = f"AWS_{client_name[12:].upper().replace('-', '')}_API="

        env = {"PROJECT_NUMBER": self.sdk_version,
               "CLIENT_NAME": client_name,
               "INPUT": client_dir,
               "OUTPUT_DIRECTORY": output_dir,
               "TAGFILES": tagfiles,
               "DOXYGEN_CONFIG_DIR": self.sdk_root + "/docs/sphinx/source/doxygen/config/",
               "PREDEFINED": export_macro}

        return self._call_doxygen(self.configuration_file, env, cwd=self.sdk_root)

    def process_one_client_async(self, dependency_map, client_name, thread_pool, client_futures):
        # Wait for dependencies to be processed
        for dependency in dependency_map.get(client_name, []):
            if dependency not in client_futures.keys():
                client_futures[dependency] = self.process_one_client_async(dependency_map, dependency,
                                                                           thread_pool, client_futures)

        for dependency in dependency_map.get(client_name, []):
            client_futures[dependency].result()

        return thread_pool.submit(
            self.generate_component_xml,
            client_name=client_name,
            client_dir=self._get_src_path(client_name),
            output_dir=self._get_doc_path(client_name),
            tagfiles=self._get_tagfiles_dependency(client_name, dependency_map))

    def generate_apidoc(self, rst_destination):
        shutil.rmtree(rst_destination, ignore_errors=True)

        thread_pool = concurrent.futures.ThreadPoolExecutor(max_workers=max(multiprocessing.cpu_count() - 1, 1))
        client_futures = {}
        doxy_groups = set(os.listdir(f"{self.sdk_root}/{DOXYGEN_OUTPUT_DIR}"))
        for group in doxy_groups:
            doxy_components = set(os.listdir(f"{self.sdk_root}/{DOXYGEN_OUTPUT_DIR}/{group}"))
            for component in doxy_components:
                apidoc_args = ["breathe-apidoc",
                               "-f",
                               f"-o", f"{rst_destination}/{group}/{component}",
                               "-p", f"{component}",
                               "-g", "class,struct,namespace",
                               f"{self.sdk_root}/{DOXYGEN_OUTPUT_DIR}/{group}/{component}/xml"]
                thread_pool.submit(subprocess.run(apidoc_args))
        for client, future in client_futures.items():
            future.result()

        # Remove all items from core repeated in other modules
        core_rst = set()
        core_modules = os.listdir(f"{rst_destination}/sdk_core")
        for module in core_modules:
            module_rst_group = os.listdir(f"{rst_destination}/sdk_core/{module}")
            for item in module_rst_group:
                if os.path.isdir(f"{rst_destination}/sdk_core/{module}/{item}"):
                    core_rst.update(os.listdir(f"{rst_destination}/sdk_core/{module}/{item}"))

        for group in doxy_groups:
            if group == "sdk_core":
                continue
            modules = set(os.listdir(f"{rst_destination}/{group}"))
            for module in modules:
                module_rst_dirs = os.listdir(f"{rst_destination}/{group}/{module}")
                for module_rst_dir in module_rst_dirs:
                    if not os.path.isdir(f"{rst_destination}/{group}/{module}/{module_rst_dir}"):
                        continue
                    module_rsts = os.listdir(f"{rst_destination}/{group}/{module}/{module_rst_dir}")
                    for module_rst in module_rsts:
                        if os.path.isfile(f"{rst_destination}/{group}/{module}/{module_rst_dir}/{module_rst}") and \
                                module_rst in core_rst:
                            print(f"Removing duplicated core item {module_rst} from {module}")
                            os.remove(f"{rst_destination}/{group}/{module}/{module_rst_dir}/{module_rst}")


        jinja2_env = jinja2.Environment(autoescape=jinja2.select_autoescape(['html', 'xml']))
        jinja2_template = jinja2_env.from_string(APIDOC_MODULE_RST_REFERENCE)

        def remove_suffix(input_string, suffix):
            if suffix and input_string.endswith(suffix):
                return input_string[:-len(suffix)]
            return input_string

        for group in doxy_groups:
            modules = set(os.listdir(f"{rst_destination}/{group}"))
            for module in modules:
                module_items = set(os.listdir(f"{rst_destination}/{group}/{module}"))
                module_rsts = [remove_suffix(item, ".rst") for item in module_items
                               if remove_suffix(item, ".rst") in {"classlist", "structlist", "namespacelist"}]
                module_rsts.sort()
                rendered_script = jinja2_template.render(t_module_name=module,
                                                         t_module_rsts=module_rsts)

                with open(f"{rst_destination}/{group}/{module}/module.rst", mode="w", encoding="utf-8") as module_rst:
                    module_rst.write(rendered_script)

    def generate_all(self):
        dependency_map = self._create_dependency_map()

        doc_gen_thread_pool = concurrent.futures.ThreadPoolExecutor(max_workers=max(multiprocessing.cpu_count() - 1, 1))

        client_futures = {}
        client_futures["aws-cpp-sdk-core"] = self.process_one_client_async(dependency_map, "aws-cpp-sdk-core",
                                                                           doc_gen_thread_pool, client_futures)

        for client in dependency_map.keys():
            if client not in client_futures.keys():
                client_futures[client] = self.process_one_client_async(dependency_map, client,
                                                                       doc_gen_thread_pool, client_futures)

        # Wait for all generation to complete
        for client, future in client_futures.items():
            future.result()

        return f"{self.sdk_root}/{DOXYGEN_OUTPUT_DIR}"


if __name__ == '__main__':
    wrapper = DoxygenWrapper(None, "")

    wrapper.generate_all()
    wrapper.generate_apidoc(f"{wrapper.sdk_root}/docs/sphinx/source/api")
