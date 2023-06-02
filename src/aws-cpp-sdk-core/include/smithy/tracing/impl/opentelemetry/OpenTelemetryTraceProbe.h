/**
 * Copyright Amazon.com, Inc. or its affiliates. All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0.
 */

#pragma once
#include <smithy/tracing/TraceProbe.h>
#include <opentelemetry/sdk/metrics/push_metric_exporter.h>
#include <opentelemetry/exporters/ostream/metric_exporter.h>

namespace smithy {
    namespace components {
        namespace tracing {
            namespace opentelemetry {
                class OpenTelemetryTraceProbe: public TraceProbe {
                public:
                    explicit OpenTelemetryTraceProbe(Aws::UniquePtr<::opentelemetry::sdk::metrics::PushMetricExporter> &metricExporter);
                    virtual ~OpenTelemetryTraceProbe() override = default;
                    virtual void DispatchEvents(const Aws::Vector<TraceEvent> &events) override;
                    static Aws::UniquePtr<OpenTelemetryTraceProbe> BuildProbe(Aws::UniquePtr<::opentelemetry::sdk::metrics::PushMetricExporter> metricExporter);
                private:
                    Aws::UniquePtr<::opentelemetry::sdk::metrics::PushMetricExporter> metricExporter;
                };
            }
        }
    }
}