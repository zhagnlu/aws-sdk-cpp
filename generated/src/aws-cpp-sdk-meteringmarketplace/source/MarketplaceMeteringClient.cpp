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

#include <aws/meteringmarketplace/MarketplaceMeteringClient.h>
#include <aws/meteringmarketplace/MarketplaceMeteringErrorMarshaller.h>
#include <aws/meteringmarketplace/MarketplaceMeteringEndpointProvider.h>
#include <aws/meteringmarketplace/model/BatchMeterUsageRequest.h>
#include <aws/meteringmarketplace/model/MeterUsageRequest.h>
#include <aws/meteringmarketplace/model/RegisterUsageRequest.h>
#include <aws/meteringmarketplace/model/ResolveCustomerRequest.h>

using namespace Aws;
using namespace Aws::Auth;
using namespace Aws::Client;
using namespace Aws::MarketplaceMetering;
using namespace Aws::MarketplaceMetering::Model;
using namespace Aws::Http;
using namespace Aws::Utils::Json;
using ResolveEndpointOutcome = Aws::Endpoint::ResolveEndpointOutcome;

const char* MarketplaceMeteringClient::SERVICE_NAME = "aws-marketplace";
const char* MarketplaceMeteringClient::ALLOCATION_TAG = "MarketplaceMeteringClient";

MarketplaceMeteringClient::MarketplaceMeteringClient(const MarketplaceMetering::MarketplaceMeteringClientConfiguration& clientConfiguration,
                                                     std::shared_ptr<MarketplaceMeteringEndpointProviderBase> endpointProvider) :
  BASECLASS(clientConfiguration,
            Aws::MakeShared<AWSAuthV4Signer>(ALLOCATION_TAG,
                                             Aws::MakeShared<DefaultAWSCredentialsProviderChain>(ALLOCATION_TAG),
                                             SERVICE_NAME,
                                             Aws::Region::ComputeSignerRegion(clientConfiguration.region)),
            Aws::MakeShared<MarketplaceMeteringErrorMarshaller>(ALLOCATION_TAG)),
  m_clientConfiguration(clientConfiguration),
  m_executor(clientConfiguration.executor),
  m_endpointProvider(std::move(endpointProvider))
{
  init(m_clientConfiguration);
}

MarketplaceMeteringClient::MarketplaceMeteringClient(const AWSCredentials& credentials,
                                                     std::shared_ptr<MarketplaceMeteringEndpointProviderBase> endpointProvider,
                                                     const MarketplaceMetering::MarketplaceMeteringClientConfiguration& clientConfiguration) :
  BASECLASS(clientConfiguration,
            Aws::MakeShared<AWSAuthV4Signer>(ALLOCATION_TAG,
                                             Aws::MakeShared<SimpleAWSCredentialsProvider>(ALLOCATION_TAG, credentials),
                                             SERVICE_NAME,
                                             Aws::Region::ComputeSignerRegion(clientConfiguration.region)),
            Aws::MakeShared<MarketplaceMeteringErrorMarshaller>(ALLOCATION_TAG)),
    m_clientConfiguration(clientConfiguration),
    m_executor(clientConfiguration.executor),
    m_endpointProvider(std::move(endpointProvider))
{
  init(m_clientConfiguration);
}

MarketplaceMeteringClient::MarketplaceMeteringClient(const std::shared_ptr<AWSCredentialsProvider>& credentialsProvider,
                                                     std::shared_ptr<MarketplaceMeteringEndpointProviderBase> endpointProvider,
                                                     const MarketplaceMetering::MarketplaceMeteringClientConfiguration& clientConfiguration) :
  BASECLASS(clientConfiguration,
            Aws::MakeShared<AWSAuthV4Signer>(ALLOCATION_TAG,
                                             credentialsProvider,
                                             SERVICE_NAME,
                                             Aws::Region::ComputeSignerRegion(clientConfiguration.region)),
            Aws::MakeShared<MarketplaceMeteringErrorMarshaller>(ALLOCATION_TAG)),
    m_clientConfiguration(clientConfiguration),
    m_executor(clientConfiguration.executor),
    m_endpointProvider(std::move(endpointProvider))
{
  init(m_clientConfiguration);
}

    /* Legacy constructors due deprecation */
  MarketplaceMeteringClient::MarketplaceMeteringClient(const Client::ClientConfiguration& clientConfiguration) :
  BASECLASS(clientConfiguration,
            Aws::MakeShared<AWSAuthV4Signer>(ALLOCATION_TAG,
                                             Aws::MakeShared<DefaultAWSCredentialsProviderChain>(ALLOCATION_TAG),
                                             SERVICE_NAME,
                                             Aws::Region::ComputeSignerRegion(clientConfiguration.region)),
            Aws::MakeShared<MarketplaceMeteringErrorMarshaller>(ALLOCATION_TAG)),
  m_clientConfiguration(clientConfiguration),
  m_executor(clientConfiguration.executor),
  m_endpointProvider(Aws::MakeShared<MarketplaceMeteringEndpointProvider>(ALLOCATION_TAG))
{
  init(m_clientConfiguration);
}

MarketplaceMeteringClient::MarketplaceMeteringClient(const AWSCredentials& credentials,
                                                     const Client::ClientConfiguration& clientConfiguration) :
  BASECLASS(clientConfiguration,
            Aws::MakeShared<AWSAuthV4Signer>(ALLOCATION_TAG,
                                             Aws::MakeShared<SimpleAWSCredentialsProvider>(ALLOCATION_TAG, credentials),
                                             SERVICE_NAME,
                                             Aws::Region::ComputeSignerRegion(clientConfiguration.region)),
            Aws::MakeShared<MarketplaceMeteringErrorMarshaller>(ALLOCATION_TAG)),
    m_clientConfiguration(clientConfiguration),
    m_executor(clientConfiguration.executor),
    m_endpointProvider(Aws::MakeShared<MarketplaceMeteringEndpointProvider>(ALLOCATION_TAG))
{
  init(m_clientConfiguration);
}

MarketplaceMeteringClient::MarketplaceMeteringClient(const std::shared_ptr<AWSCredentialsProvider>& credentialsProvider,
                                                     const Client::ClientConfiguration& clientConfiguration) :
  BASECLASS(clientConfiguration,
            Aws::MakeShared<AWSAuthV4Signer>(ALLOCATION_TAG,
                                             credentialsProvider,
                                             SERVICE_NAME,
                                             Aws::Region::ComputeSignerRegion(clientConfiguration.region)),
            Aws::MakeShared<MarketplaceMeteringErrorMarshaller>(ALLOCATION_TAG)),
    m_clientConfiguration(clientConfiguration),
    m_executor(clientConfiguration.executor),
    m_endpointProvider(Aws::MakeShared<MarketplaceMeteringEndpointProvider>(ALLOCATION_TAG))
{
  init(m_clientConfiguration);
}

    /* End of legacy constructors due deprecation */
MarketplaceMeteringClient::~MarketplaceMeteringClient()
{
  ShutdownSdkClient(this, -1);
}

std::shared_ptr<MarketplaceMeteringEndpointProviderBase>& MarketplaceMeteringClient::accessEndpointProvider()
{
  return m_endpointProvider;
}

void MarketplaceMeteringClient::init(const MarketplaceMetering::MarketplaceMeteringClientConfiguration& config)
{
  AWSClient::SetServiceClientName("Marketplace Metering");
  AWS_CHECK_PTR(SERVICE_NAME, m_endpointProvider);
  m_endpointProvider->InitBuiltInParameters(config);
}

void MarketplaceMeteringClient::OverrideEndpoint(const Aws::String& endpoint)
{
  AWS_CHECK_PTR(SERVICE_NAME, m_endpointProvider);
  m_endpointProvider->OverrideEndpoint(endpoint);
}

BatchMeterUsageOutcome MarketplaceMeteringClient::BatchMeterUsage(const BatchMeterUsageRequest& request) const
{
  AWS_OPERATION_GUARD(BatchMeterUsage);
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, BatchMeterUsage, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, BatchMeterUsage, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return BatchMeterUsageOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

MeterUsageOutcome MarketplaceMeteringClient::MeterUsage(const MeterUsageRequest& request) const
{
  AWS_OPERATION_GUARD(MeterUsage);
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, MeterUsage, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, MeterUsage, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return MeterUsageOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

RegisterUsageOutcome MarketplaceMeteringClient::RegisterUsage(const RegisterUsageRequest& request) const
{
  AWS_OPERATION_GUARD(RegisterUsage);
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, RegisterUsage, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, RegisterUsage, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return RegisterUsageOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

ResolveCustomerOutcome MarketplaceMeteringClient::ResolveCustomer(const ResolveCustomerRequest& request) const
{
  AWS_OPERATION_GUARD(ResolveCustomer);
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, ResolveCustomer, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, ResolveCustomer, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  return ResolveCustomerOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

