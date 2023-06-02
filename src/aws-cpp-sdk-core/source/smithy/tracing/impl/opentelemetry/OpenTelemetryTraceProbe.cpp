/**
 * Copyright Amazon.com, Inc. or its affiliates. All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0.
 */
#include <smithy/tracing/impl/opentelemetry/OpenTelemetryTraceProbe.h>

using namespace smithy::components::tracing::opentelemetry;
using namespace opentelemetry::sdk::metrics;

static const char *ALLOC_TAG = "OpenTelemetryTraceProbe";

void OpenTelemetryTraceProbe::DispatchEvents(const Aws::Vector<TraceEvent> &events) {
    AWS_UNREFERENCED_PARAM(events);
}

OpenTelemetryTraceProbe::OpenTelemetryTraceProbe(Aws::UniquePtr<PushMetricExporter> &metricExporter) :
    metricExporter(std::move(metricExporter)) {}

Aws::UniquePtr<OpenTelemetryTraceProbe>
OpenTelemetryTraceProbe::BuildProbe(Aws::UniquePtr<::opentelemetry::sdk::metrics::PushMetricExporter> metricExporter) {
    return Aws::MakeUnique<OpenTelemetryTraceProbe>(ALLOC_TAG, metricExporter);
}
