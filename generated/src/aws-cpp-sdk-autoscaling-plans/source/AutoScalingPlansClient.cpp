﻿/**
 * Copyright Amazon.com, Inc. or its affiliates. All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0.
 */

#include <aws/core/utils/Outcome.h>
#include <aws/core/auth/AWSAuthSigner.h>
#include <aws/core/client/CoreErrors.h>
#include <aws/core/client/RetryStrategy.h>
#include <aws/core/http/HttpClient.h>
#include <aws/core/http/HttpResponse.h>
#include <aws/core/http/HttpClientFactory.h>
#include <aws/core/auth/AWSCredentialsProviderChain.h>
#include <aws/core/utils/json/JsonSerializer.h>
#include <aws/core/utils/memory/stl/AWSStringStream.h>
#include <aws/core/utils/threading/Executor.h>
#include <aws/core/utils/DNS.h>
#include <aws/core/utils/logging/LogMacros.h>
#include <aws/core/utils/logging/ErrorMacros.h>

#include <aws/autoscaling-plans/AutoScalingPlansClient.h>
#include <aws/autoscaling-plans/AutoScalingPlansErrorMarshaller.h>
#include <aws/autoscaling-plans/AutoScalingPlansEndpointProvider.h>
#include <aws/autoscaling-plans/model/CreateScalingPlanRequest.h>
#include <aws/autoscaling-plans/model/DeleteScalingPlanRequest.h>
#include <aws/autoscaling-plans/model/DescribeScalingPlanResourcesRequest.h>
#include <aws/autoscaling-plans/model/DescribeScalingPlansRequest.h>
#include <aws/autoscaling-plans/model/GetScalingPlanResourceForecastDataRequest.h>
#include <aws/autoscaling-plans/model/UpdateScalingPlanRequest.h>

using namespace Aws;
using namespace Aws::Auth;
using namespace Aws::Client;
using namespace Aws::AutoScalingPlans;
using namespace Aws::AutoScalingPlans::Model;
using namespace Aws::Http;
using namespace Aws::Utils::Json;
using ResolveEndpointOutcome = Aws::Endpoint::ResolveEndpointOutcome;

const char* AutoScalingPlansClient::SERVICE_NAME = "autoscaling-plans";
const char* AutoScalingPlansClient::ALLOCATION_TAG = "AutoScalingPlansClient";

AutoScalingPlansClient::AutoScalingPlansClient(const AutoScalingPlans::AutoScalingPlansClientConfiguration& clientConfiguration,
                                               std::shared_ptr<AutoScalingPlansEndpointProviderBase> endpointProvider) :
  BASECLASS(clientConfiguration,
            Aws::MakeShared<AWSAuthV4Signer>(ALLOCATION_TAG,
                                             Aws::MakeShared<DefaultAWSCredentialsProviderChain>(ALLOCATION_TAG),
                                             SERVICE_NAME,
                                             Aws::Region::ComputeSignerRegion(clientConfiguration.region)),
            Aws::MakeShared<AutoScalingPlansErrorMarshaller>(ALLOCATION_TAG)),
  m_clientConfiguration(clientConfiguration),
  m_executor(clientConfiguration.executor),
  m_endpointProvider(std::move(endpointProvider))
{
  init(m_clientConfiguration);
}

AutoScalingPlansClient::AutoScalingPlansClient(const AWSCredentials& credentials,
                                               std::shared_ptr<AutoScalingPlansEndpointProviderBase> endpointProvider,
                                               const AutoScalingPlans::AutoScalingPlansClientConfiguration& clientConfiguration) :
  BASECLASS(clientConfiguration,
            Aws::MakeShared<AWSAuthV4Signer>(ALLOCATION_TAG,
                                             Aws::MakeShared<SimpleAWSCredentialsProvider>(ALLOCATION_TAG, credentials),
                                             SERVICE_NAME,
                                             Aws::Region::ComputeSignerRegion(clientConfiguration.region)),
            Aws::MakeShared<AutoScalingPlansErrorMarshaller>(ALLOCATION_TAG)),
    m_clientConfiguration(clientConfiguration),
    m_executor(clientConfiguration.executor),
    m_endpointProvider(std::move(endpointProvider))
{
  init(m_clientConfiguration);
}

AutoScalingPlansClient::AutoScalingPlansClient(const std::shared_ptr<AWSCredentialsProvider>& credentialsProvider,
                                               std::shared_ptr<AutoScalingPlansEndpointProviderBase> endpointProvider,
                                               const AutoScalingPlans::AutoScalingPlansClientConfiguration& clientConfiguration) :
  BASECLASS(clientConfiguration,
            Aws::MakeShared<AWSAuthV4Signer>(ALLOCATION_TAG,
                                             credentialsProvider,
                                             SERVICE_NAME,
                                             Aws::Region::ComputeSignerRegion(clientConfiguration.region)),
            Aws::MakeShared<AutoScalingPlansErrorMarshaller>(ALLOCATION_TAG)),
    m_clientConfiguration(clientConfiguration),
    m_executor(clientConfiguration.executor),
    m_endpointProvider(std::move(endpointProvider))
{
  init(m_clientConfiguration);
}

    /* Legacy constructors due deprecation */
  AutoScalingPlansClient::AutoScalingPlansClient(const Client::ClientConfiguration& clientConfiguration) :
  BASECLASS(clientConfiguration,
            Aws::MakeShared<AWSAuthV4Signer>(ALLOCATION_TAG,
                                             Aws::MakeShared<DefaultAWSCredentialsProviderChain>(ALLOCATION_TAG),
                                             SERVICE_NAME,
                                             Aws::Region::ComputeSignerRegion(clientConfiguration.region)),
            Aws::MakeShared<AutoScalingPlansErrorMarshaller>(ALLOCATION_TAG)),
  m_clientConfiguration(clientConfiguration),
  m_executor(clientConfiguration.executor),
  m_endpointProvider(Aws::MakeShared<AutoScalingPlansEndpointProvider>(ALLOCATION_TAG))
{
  init(m_clientConfiguration);
}

AutoScalingPlansClient::AutoScalingPlansClient(const AWSCredentials& credentials,
                                               const Client::ClientConfiguration& clientConfiguration) :
  BASECLASS(clientConfiguration,
            Aws::MakeShared<AWSAuthV4Signer>(ALLOCATION_TAG,
                                             Aws::MakeShared<SimpleAWSCredentialsProvider>(ALLOCATION_TAG, credentials),
                                             SERVICE_NAME,
                                             Aws::Region::ComputeSignerRegion(clientConfiguration.region)),
            Aws::MakeShared<AutoScalingPlansErrorMarshaller>(ALLOCATION_TAG)),
    m_clientConfiguration(clientConfiguration),
    m_executor(clientConfiguration.executor),
    m_endpointProvider(Aws::MakeShared<AutoScalingPlansEndpointProvider>(ALLOCATION_TAG))
{
  init(m_clientConfiguration);
}

AutoScalingPlansClient::AutoScalingPlansClient(const std::shared_ptr<AWSCredentialsProvider>& credentialsProvider,
                                               const Client::ClientConfiguration& clientConfiguration) :
  BASECLASS(clientConfiguration,
            Aws::MakeShared<AWSAuthV4Signer>(ALLOCATION_TAG,
                                             credentialsProvider,
                                             SERVICE_NAME,
                                             Aws::Region::ComputeSignerRegion(clientConfiguration.region)),
            Aws::MakeShared<AutoScalingPlansErrorMarshaller>(ALLOCATION_TAG)),
    m_clientConfiguration(clientConfiguration),
    m_executor(clientConfiguration.executor),
    m_endpointProvider(Aws::MakeShared<AutoScalingPlansEndpointProvider>(ALLOCATION_TAG))
{
  init(m_clientConfiguration);
}

    /* End of legacy constructors due deprecation */
AutoScalingPlansClient::~AutoScalingPlansClient()
{
  ShutdownSdkClient(this, -1);
}

std::shared_ptr<AutoScalingPlansEndpointProviderBase>& AutoScalingPlansClient::accessEndpointProvider()
{
  return m_endpointProvider;
}

void AutoScalingPlansClient::init(const AutoScalingPlans::AutoScalingPlansClientConfiguration& config)
{
  AWSClient::SetServiceClientName("Auto Scaling Plans");
  AWS_CHECK_PTR(SERVICE_NAME, m_endpointProvider);
  m_endpointProvider->InitBuiltInParameters(config);
}

void AutoScalingPlansClient::OverrideEndpoint(const Aws::String& endpoint)
{
  AWS_CHECK_PTR(SERVICE_NAME, m_endpointProvider);
  m_endpointProvider->OverrideEndpoint(endpoint);
}

CreateScalingPlanOutcome AutoScalingPlansClient::CreateScalingPlan(const CreateScalingPlanRequest& request) const
{
  AWS_OPERATION_GUARD(CreateScalingPlan);
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, CreateScalingPlan, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, CreateScalingPlan, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return CreateScalingPlanOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

DeleteScalingPlanOutcome AutoScalingPlansClient::DeleteScalingPlan(const DeleteScalingPlanRequest& request) const
{
  AWS_OPERATION_GUARD(DeleteScalingPlan);
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, DeleteScalingPlan, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, DeleteScalingPlan, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return DeleteScalingPlanOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

DescribeScalingPlanResourcesOutcome AutoScalingPlansClient::DescribeScalingPlanResources(const DescribeScalingPlanResourcesRequest& request) const
{
  AWS_OPERATION_GUARD(DescribeScalingPlanResources);
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, DescribeScalingPlanResources, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, DescribeScalingPlanResources, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return DescribeScalingPlanResourcesOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

DescribeScalingPlansOutcome AutoScalingPlansClient::DescribeScalingPlans(const DescribeScalingPlansRequest& request) const
{
  AWS_OPERATION_GUARD(DescribeScalingPlans);
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, DescribeScalingPlans, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, DescribeScalingPlans, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return DescribeScalingPlansOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

GetScalingPlanResourceForecastDataOutcome AutoScalingPlansClient::GetScalingPlanResourceForecastData(const GetScalingPlanResourceForecastDataRequest& request) const
{
  AWS_OPERATION_GUARD(GetScalingPlanResourceForecastData);
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, GetScalingPlanResourceForecastData, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, GetScalingPlanResourceForecastData, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return GetScalingPlanResourceForecastDataOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

UpdateScalingPlanOutcome AutoScalingPlansClient::UpdateScalingPlan(const UpdateScalingPlanRequest& request) const
{
  AWS_OPERATION_GUARD(UpdateScalingPlan);
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, UpdateScalingPlan, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, UpdateScalingPlan, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return UpdateScalingPlanOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

