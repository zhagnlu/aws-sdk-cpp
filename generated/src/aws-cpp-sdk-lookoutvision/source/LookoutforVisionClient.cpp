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

#include <aws/lookoutvision/LookoutforVisionClient.h>
#include <aws/lookoutvision/LookoutforVisionErrorMarshaller.h>
#include <aws/lookoutvision/LookoutforVisionEndpointProvider.h>
#include <aws/lookoutvision/model/CreateDatasetRequest.h>
#include <aws/lookoutvision/model/CreateModelRequest.h>
#include <aws/lookoutvision/model/CreateProjectRequest.h>
#include <aws/lookoutvision/model/DeleteDatasetRequest.h>
#include <aws/lookoutvision/model/DeleteModelRequest.h>
#include <aws/lookoutvision/model/DeleteProjectRequest.h>
#include <aws/lookoutvision/model/DescribeDatasetRequest.h>
#include <aws/lookoutvision/model/DescribeModelRequest.h>
#include <aws/lookoutvision/model/DescribeModelPackagingJobRequest.h>
#include <aws/lookoutvision/model/DescribeProjectRequest.h>
#include <aws/lookoutvision/model/DetectAnomaliesRequest.h>
#include <aws/lookoutvision/model/ListDatasetEntriesRequest.h>
#include <aws/lookoutvision/model/ListModelPackagingJobsRequest.h>
#include <aws/lookoutvision/model/ListModelsRequest.h>
#include <aws/lookoutvision/model/ListProjectsRequest.h>
#include <aws/lookoutvision/model/ListTagsForResourceRequest.h>
#include <aws/lookoutvision/model/StartModelRequest.h>
#include <aws/lookoutvision/model/StartModelPackagingJobRequest.h>
#include <aws/lookoutvision/model/StopModelRequest.h>
#include <aws/lookoutvision/model/TagResourceRequest.h>
#include <aws/lookoutvision/model/UntagResourceRequest.h>
#include <aws/lookoutvision/model/UpdateDatasetEntriesRequest.h>

using namespace Aws;
using namespace Aws::Auth;
using namespace Aws::Client;
using namespace Aws::LookoutforVision;
using namespace Aws::LookoutforVision::Model;
using namespace Aws::Http;
using namespace Aws::Utils::Json;
using ResolveEndpointOutcome = Aws::Endpoint::ResolveEndpointOutcome;

const char* LookoutforVisionClient::SERVICE_NAME = "lookoutvision";
const char* LookoutforVisionClient::ALLOCATION_TAG = "LookoutforVisionClient";

LookoutforVisionClient::LookoutforVisionClient(const LookoutforVision::LookoutforVisionClientConfiguration& clientConfiguration,
                                               std::shared_ptr<LookoutforVisionEndpointProviderBase> endpointProvider) :
  BASECLASS(clientConfiguration,
            Aws::MakeShared<Aws::Auth::DefaultAuthSignerProvider>(ALLOCATION_TAG,
                                                                  Aws::MakeShared<DefaultAWSCredentialsProviderChain>(ALLOCATION_TAG),
                                                                  SERVICE_NAME,
                                                                  Aws::Region::ComputeSignerRegion(clientConfiguration.region)),
            Aws::MakeShared<LookoutforVisionErrorMarshaller>(ALLOCATION_TAG)),
  m_clientConfiguration(clientConfiguration),
  m_executor(clientConfiguration.executor),
  m_endpointProvider(std::move(endpointProvider))
{
  init(m_clientConfiguration);
}

LookoutforVisionClient::LookoutforVisionClient(const AWSCredentials& credentials,
                                               std::shared_ptr<LookoutforVisionEndpointProviderBase> endpointProvider,
                                               const LookoutforVision::LookoutforVisionClientConfiguration& clientConfiguration) :
  BASECLASS(clientConfiguration,
            Aws::MakeShared<Aws::Auth::DefaultAuthSignerProvider>(ALLOCATION_TAG,
                                                                  Aws::MakeShared<SimpleAWSCredentialsProvider>(ALLOCATION_TAG, credentials),
                                                                  SERVICE_NAME,
                                                                  Aws::Region::ComputeSignerRegion(clientConfiguration.region)),
            Aws::MakeShared<LookoutforVisionErrorMarshaller>(ALLOCATION_TAG)),
    m_clientConfiguration(clientConfiguration),
    m_executor(clientConfiguration.executor),
    m_endpointProvider(std::move(endpointProvider))
{
  init(m_clientConfiguration);
}

LookoutforVisionClient::LookoutforVisionClient(const std::shared_ptr<AWSCredentialsProvider>& credentialsProvider,
                                               std::shared_ptr<LookoutforVisionEndpointProviderBase> endpointProvider,
                                               const LookoutforVision::LookoutforVisionClientConfiguration& clientConfiguration) :
  BASECLASS(clientConfiguration,
            Aws::MakeShared<Aws::Auth::DefaultAuthSignerProvider>(ALLOCATION_TAG,
                                                                  credentialsProvider,
                                                                  SERVICE_NAME,
                                                                  Aws::Region::ComputeSignerRegion(clientConfiguration.region)),
            Aws::MakeShared<LookoutforVisionErrorMarshaller>(ALLOCATION_TAG)),
    m_clientConfiguration(clientConfiguration),
    m_executor(clientConfiguration.executor),
    m_endpointProvider(std::move(endpointProvider))
{
  init(m_clientConfiguration);
}

    /* Legacy constructors due deprecation */
  LookoutforVisionClient::LookoutforVisionClient(const Client::ClientConfiguration& clientConfiguration) :
  BASECLASS(clientConfiguration,
            Aws::MakeShared<Aws::Auth::DefaultAuthSignerProvider>(ALLOCATION_TAG,
                                                                  Aws::MakeShared<DefaultAWSCredentialsProviderChain>(ALLOCATION_TAG),
                                                                  SERVICE_NAME,
                                                                  Aws::Region::ComputeSignerRegion(clientConfiguration.region)),
            Aws::MakeShared<LookoutforVisionErrorMarshaller>(ALLOCATION_TAG)),
  m_clientConfiguration(clientConfiguration),
  m_executor(clientConfiguration.executor),
  m_endpointProvider(Aws::MakeShared<LookoutforVisionEndpointProvider>(ALLOCATION_TAG))
{
  init(m_clientConfiguration);
}

LookoutforVisionClient::LookoutforVisionClient(const AWSCredentials& credentials,
                                               const Client::ClientConfiguration& clientConfiguration) :
  BASECLASS(clientConfiguration,
            Aws::MakeShared<Aws::Auth::DefaultAuthSignerProvider>(ALLOCATION_TAG,
                                                                  Aws::MakeShared<SimpleAWSCredentialsProvider>(ALLOCATION_TAG, credentials),
                                                                  SERVICE_NAME,
                                                                  Aws::Region::ComputeSignerRegion(clientConfiguration.region)),
            Aws::MakeShared<LookoutforVisionErrorMarshaller>(ALLOCATION_TAG)),
    m_clientConfiguration(clientConfiguration),
    m_executor(clientConfiguration.executor),
    m_endpointProvider(Aws::MakeShared<LookoutforVisionEndpointProvider>(ALLOCATION_TAG))
{
  init(m_clientConfiguration);
}

LookoutforVisionClient::LookoutforVisionClient(const std::shared_ptr<AWSCredentialsProvider>& credentialsProvider,
                                               const Client::ClientConfiguration& clientConfiguration) :
  BASECLASS(clientConfiguration,
            Aws::MakeShared<Aws::Auth::DefaultAuthSignerProvider>(ALLOCATION_TAG,
                                                                  credentialsProvider,
                                                                  SERVICE_NAME,
                                                                  Aws::Region::ComputeSignerRegion(clientConfiguration.region)),
            Aws::MakeShared<LookoutforVisionErrorMarshaller>(ALLOCATION_TAG)),
    m_clientConfiguration(clientConfiguration),
    m_executor(clientConfiguration.executor),
    m_endpointProvider(Aws::MakeShared<LookoutforVisionEndpointProvider>(ALLOCATION_TAG))
{
  init(m_clientConfiguration);
}

    /* End of legacy constructors due deprecation */
LookoutforVisionClient::~LookoutforVisionClient()
{
  ShutdownSdkClient(this, -1);
}

std::shared_ptr<LookoutforVisionEndpointProviderBase>& LookoutforVisionClient::accessEndpointProvider()
{
  return m_endpointProvider;
}

void LookoutforVisionClient::init(const LookoutforVision::LookoutforVisionClientConfiguration& config)
{
  AWSClient::SetServiceClientName("LookoutVision");
  AWS_CHECK_PTR(SERVICE_NAME, m_endpointProvider);
  m_endpointProvider->InitBuiltInParameters(config);
}

void LookoutforVisionClient::OverrideEndpoint(const Aws::String& endpoint)
{
  AWS_CHECK_PTR(SERVICE_NAME, m_endpointProvider);
  m_endpointProvider->OverrideEndpoint(endpoint);
}

CreateDatasetOutcome LookoutforVisionClient::CreateDataset(const CreateDatasetRequest& request) const
{
  AWS_OPERATION_GUARD(CreateDataset);
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, CreateDataset, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.ProjectNameHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("CreateDataset", "Required field: ProjectName, is not set");
    return CreateDatasetOutcome(Aws::Client::AWSError<LookoutforVisionErrors>(LookoutforVisionErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [ProjectName]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, CreateDataset, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/2020-11-20/projects/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetProjectName());
  endpointResolutionOutcome.GetResult().AddPathSegments("/datasets");
  return CreateDatasetOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

CreateModelOutcome LookoutforVisionClient::CreateModel(const CreateModelRequest& request) const
{
  AWS_OPERATION_GUARD(CreateModel);
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, CreateModel, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.ProjectNameHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("CreateModel", "Required field: ProjectName, is not set");
    return CreateModelOutcome(Aws::Client::AWSError<LookoutforVisionErrors>(LookoutforVisionErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [ProjectName]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, CreateModel, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/2020-11-20/projects/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetProjectName());
  endpointResolutionOutcome.GetResult().AddPathSegments("/models");
  return CreateModelOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

CreateProjectOutcome LookoutforVisionClient::CreateProject(const CreateProjectRequest& request) const
{
  AWS_OPERATION_GUARD(CreateProject);
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, CreateProject, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, CreateProject, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/2020-11-20/projects");
  return CreateProjectOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

DeleteDatasetOutcome LookoutforVisionClient::DeleteDataset(const DeleteDatasetRequest& request) const
{
  AWS_OPERATION_GUARD(DeleteDataset);
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, DeleteDataset, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.ProjectNameHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("DeleteDataset", "Required field: ProjectName, is not set");
    return DeleteDatasetOutcome(Aws::Client::AWSError<LookoutforVisionErrors>(LookoutforVisionErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [ProjectName]", false));
  }
  if (!request.DatasetTypeHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("DeleteDataset", "Required field: DatasetType, is not set");
    return DeleteDatasetOutcome(Aws::Client::AWSError<LookoutforVisionErrors>(LookoutforVisionErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [DatasetType]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, DeleteDataset, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/2020-11-20/projects/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetProjectName());
  endpointResolutionOutcome.GetResult().AddPathSegments("/datasets/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetDatasetType());
  return DeleteDatasetOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_DELETE, Aws::Auth::SIGV4_SIGNER));
}

DeleteModelOutcome LookoutforVisionClient::DeleteModel(const DeleteModelRequest& request) const
{
  AWS_OPERATION_GUARD(DeleteModel);
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, DeleteModel, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.ProjectNameHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("DeleteModel", "Required field: ProjectName, is not set");
    return DeleteModelOutcome(Aws::Client::AWSError<LookoutforVisionErrors>(LookoutforVisionErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [ProjectName]", false));
  }
  if (!request.ModelVersionHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("DeleteModel", "Required field: ModelVersion, is not set");
    return DeleteModelOutcome(Aws::Client::AWSError<LookoutforVisionErrors>(LookoutforVisionErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [ModelVersion]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, DeleteModel, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/2020-11-20/projects/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetProjectName());
  endpointResolutionOutcome.GetResult().AddPathSegments("/models/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetModelVersion());
  return DeleteModelOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_DELETE, Aws::Auth::SIGV4_SIGNER));
}

DeleteProjectOutcome LookoutforVisionClient::DeleteProject(const DeleteProjectRequest& request) const
{
  AWS_OPERATION_GUARD(DeleteProject);
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, DeleteProject, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.ProjectNameHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("DeleteProject", "Required field: ProjectName, is not set");
    return DeleteProjectOutcome(Aws::Client::AWSError<LookoutforVisionErrors>(LookoutforVisionErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [ProjectName]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, DeleteProject, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/2020-11-20/projects/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetProjectName());
  return DeleteProjectOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_DELETE, Aws::Auth::SIGV4_SIGNER));
}

DescribeDatasetOutcome LookoutforVisionClient::DescribeDataset(const DescribeDatasetRequest& request) const
{
  AWS_OPERATION_GUARD(DescribeDataset);
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, DescribeDataset, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.ProjectNameHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("DescribeDataset", "Required field: ProjectName, is not set");
    return DescribeDatasetOutcome(Aws::Client::AWSError<LookoutforVisionErrors>(LookoutforVisionErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [ProjectName]", false));
  }
  if (!request.DatasetTypeHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("DescribeDataset", "Required field: DatasetType, is not set");
    return DescribeDatasetOutcome(Aws::Client::AWSError<LookoutforVisionErrors>(LookoutforVisionErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [DatasetType]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, DescribeDataset, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/2020-11-20/projects/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetProjectName());
  endpointResolutionOutcome.GetResult().AddPathSegments("/datasets/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetDatasetType());
  return DescribeDatasetOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_GET, Aws::Auth::SIGV4_SIGNER));
}

DescribeModelOutcome LookoutforVisionClient::DescribeModel(const DescribeModelRequest& request) const
{
  AWS_OPERATION_GUARD(DescribeModel);
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, DescribeModel, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.ProjectNameHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("DescribeModel", "Required field: ProjectName, is not set");
    return DescribeModelOutcome(Aws::Client::AWSError<LookoutforVisionErrors>(LookoutforVisionErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [ProjectName]", false));
  }
  if (!request.ModelVersionHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("DescribeModel", "Required field: ModelVersion, is not set");
    return DescribeModelOutcome(Aws::Client::AWSError<LookoutforVisionErrors>(LookoutforVisionErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [ModelVersion]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, DescribeModel, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/2020-11-20/projects/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetProjectName());
  endpointResolutionOutcome.GetResult().AddPathSegments("/models/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetModelVersion());
  return DescribeModelOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_GET, Aws::Auth::SIGV4_SIGNER));
}

DescribeModelPackagingJobOutcome LookoutforVisionClient::DescribeModelPackagingJob(const DescribeModelPackagingJobRequest& request) const
{
  AWS_OPERATION_GUARD(DescribeModelPackagingJob);
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, DescribeModelPackagingJob, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.ProjectNameHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("DescribeModelPackagingJob", "Required field: ProjectName, is not set");
    return DescribeModelPackagingJobOutcome(Aws::Client::AWSError<LookoutforVisionErrors>(LookoutforVisionErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [ProjectName]", false));
  }
  if (!request.JobNameHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("DescribeModelPackagingJob", "Required field: JobName, is not set");
    return DescribeModelPackagingJobOutcome(Aws::Client::AWSError<LookoutforVisionErrors>(LookoutforVisionErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [JobName]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, DescribeModelPackagingJob, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/2020-11-20/projects/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetProjectName());
  endpointResolutionOutcome.GetResult().AddPathSegments("/modelpackagingjobs/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetJobName());
  return DescribeModelPackagingJobOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_GET, Aws::Auth::SIGV4_SIGNER));
}

DescribeProjectOutcome LookoutforVisionClient::DescribeProject(const DescribeProjectRequest& request) const
{
  AWS_OPERATION_GUARD(DescribeProject);
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, DescribeProject, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.ProjectNameHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("DescribeProject", "Required field: ProjectName, is not set");
    return DescribeProjectOutcome(Aws::Client::AWSError<LookoutforVisionErrors>(LookoutforVisionErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [ProjectName]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, DescribeProject, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/2020-11-20/projects/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetProjectName());
  return DescribeProjectOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_GET, Aws::Auth::SIGV4_SIGNER));
}

DetectAnomaliesOutcome LookoutforVisionClient::DetectAnomalies(const DetectAnomaliesRequest& request) const
{
  AWS_OPERATION_GUARD(DetectAnomalies);
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, DetectAnomalies, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.ProjectNameHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("DetectAnomalies", "Required field: ProjectName, is not set");
    return DetectAnomaliesOutcome(Aws::Client::AWSError<LookoutforVisionErrors>(LookoutforVisionErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [ProjectName]", false));
  }
  if (!request.ModelVersionHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("DetectAnomalies", "Required field: ModelVersion, is not set");
    return DetectAnomaliesOutcome(Aws::Client::AWSError<LookoutforVisionErrors>(LookoutforVisionErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [ModelVersion]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, DetectAnomalies, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/2020-11-20/projects/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetProjectName());
  endpointResolutionOutcome.GetResult().AddPathSegments("/models/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetModelVersion());
  endpointResolutionOutcome.GetResult().AddPathSegments("/detect");
  return DetectAnomaliesOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

ListDatasetEntriesOutcome LookoutforVisionClient::ListDatasetEntries(const ListDatasetEntriesRequest& request) const
{
  AWS_OPERATION_GUARD(ListDatasetEntries);
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, ListDatasetEntries, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.ProjectNameHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("ListDatasetEntries", "Required field: ProjectName, is not set");
    return ListDatasetEntriesOutcome(Aws::Client::AWSError<LookoutforVisionErrors>(LookoutforVisionErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [ProjectName]", false));
  }
  if (!request.DatasetTypeHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("ListDatasetEntries", "Required field: DatasetType, is not set");
    return ListDatasetEntriesOutcome(Aws::Client::AWSError<LookoutforVisionErrors>(LookoutforVisionErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [DatasetType]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, ListDatasetEntries, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/2020-11-20/projects/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetProjectName());
  endpointResolutionOutcome.GetResult().AddPathSegments("/datasets/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetDatasetType());
  endpointResolutionOutcome.GetResult().AddPathSegments("/entries");
  return ListDatasetEntriesOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_GET, Aws::Auth::SIGV4_SIGNER));
}

ListModelPackagingJobsOutcome LookoutforVisionClient::ListModelPackagingJobs(const ListModelPackagingJobsRequest& request) const
{
  AWS_OPERATION_GUARD(ListModelPackagingJobs);
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, ListModelPackagingJobs, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.ProjectNameHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("ListModelPackagingJobs", "Required field: ProjectName, is not set");
    return ListModelPackagingJobsOutcome(Aws::Client::AWSError<LookoutforVisionErrors>(LookoutforVisionErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [ProjectName]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, ListModelPackagingJobs, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/2020-11-20/projects/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetProjectName());
  endpointResolutionOutcome.GetResult().AddPathSegments("/modelpackagingjobs");
  return ListModelPackagingJobsOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_GET, Aws::Auth::SIGV4_SIGNER));
}

ListModelsOutcome LookoutforVisionClient::ListModels(const ListModelsRequest& request) const
{
  AWS_OPERATION_GUARD(ListModels);
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, ListModels, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.ProjectNameHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("ListModels", "Required field: ProjectName, is not set");
    return ListModelsOutcome(Aws::Client::AWSError<LookoutforVisionErrors>(LookoutforVisionErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [ProjectName]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, ListModels, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/2020-11-20/projects/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetProjectName());
  endpointResolutionOutcome.GetResult().AddPathSegments("/models");
  return ListModelsOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_GET, Aws::Auth::SIGV4_SIGNER));
}

ListProjectsOutcome LookoutforVisionClient::ListProjects(const ListProjectsRequest& request) const
{
  AWS_OPERATION_GUARD(ListProjects);
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, ListProjects, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, ListProjects, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/2020-11-20/projects");
  return ListProjectsOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_GET, Aws::Auth::SIGV4_SIGNER));
}

ListTagsForResourceOutcome LookoutforVisionClient::ListTagsForResource(const ListTagsForResourceRequest& request) const
{
  AWS_OPERATION_GUARD(ListTagsForResource);
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, ListTagsForResource, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.ResourceArnHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("ListTagsForResource", "Required field: ResourceArn, is not set");
    return ListTagsForResourceOutcome(Aws::Client::AWSError<LookoutforVisionErrors>(LookoutforVisionErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [ResourceArn]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, ListTagsForResource, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/2020-11-20/tags/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetResourceArn());
  return ListTagsForResourceOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_GET, Aws::Auth::SIGV4_SIGNER));
}

StartModelOutcome LookoutforVisionClient::StartModel(const StartModelRequest& request) const
{
  AWS_OPERATION_GUARD(StartModel);
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, StartModel, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.ProjectNameHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("StartModel", "Required field: ProjectName, is not set");
    return StartModelOutcome(Aws::Client::AWSError<LookoutforVisionErrors>(LookoutforVisionErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [ProjectName]", false));
  }
  if (!request.ModelVersionHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("StartModel", "Required field: ModelVersion, is not set");
    return StartModelOutcome(Aws::Client::AWSError<LookoutforVisionErrors>(LookoutforVisionErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [ModelVersion]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, StartModel, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/2020-11-20/projects/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetProjectName());
  endpointResolutionOutcome.GetResult().AddPathSegments("/models/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetModelVersion());
  endpointResolutionOutcome.GetResult().AddPathSegments("/start");
  return StartModelOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

StartModelPackagingJobOutcome LookoutforVisionClient::StartModelPackagingJob(const StartModelPackagingJobRequest& request) const
{
  AWS_OPERATION_GUARD(StartModelPackagingJob);
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, StartModelPackagingJob, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.ProjectNameHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("StartModelPackagingJob", "Required field: ProjectName, is not set");
    return StartModelPackagingJobOutcome(Aws::Client::AWSError<LookoutforVisionErrors>(LookoutforVisionErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [ProjectName]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, StartModelPackagingJob, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/2020-11-20/projects/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetProjectName());
  endpointResolutionOutcome.GetResult().AddPathSegments("/modelpackagingjobs");
  return StartModelPackagingJobOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

StopModelOutcome LookoutforVisionClient::StopModel(const StopModelRequest& request) const
{
  AWS_OPERATION_GUARD(StopModel);
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, StopModel, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.ProjectNameHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("StopModel", "Required field: ProjectName, is not set");
    return StopModelOutcome(Aws::Client::AWSError<LookoutforVisionErrors>(LookoutforVisionErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [ProjectName]", false));
  }
  if (!request.ModelVersionHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("StopModel", "Required field: ModelVersion, is not set");
    return StopModelOutcome(Aws::Client::AWSError<LookoutforVisionErrors>(LookoutforVisionErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [ModelVersion]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, StopModel, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/2020-11-20/projects/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetProjectName());
  endpointResolutionOutcome.GetResult().AddPathSegments("/models/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetModelVersion());
  endpointResolutionOutcome.GetResult().AddPathSegments("/stop");
  return StopModelOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

TagResourceOutcome LookoutforVisionClient::TagResource(const TagResourceRequest& request) const
{
  AWS_OPERATION_GUARD(TagResource);
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, TagResource, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.ResourceArnHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("TagResource", "Required field: ResourceArn, is not set");
    return TagResourceOutcome(Aws::Client::AWSError<LookoutforVisionErrors>(LookoutforVisionErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [ResourceArn]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, TagResource, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/2020-11-20/tags/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetResourceArn());
  return TagResourceOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

UntagResourceOutcome LookoutforVisionClient::UntagResource(const UntagResourceRequest& request) const
{
  AWS_OPERATION_GUARD(UntagResource);
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, UntagResource, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.ResourceArnHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("UntagResource", "Required field: ResourceArn, is not set");
    return UntagResourceOutcome(Aws::Client::AWSError<LookoutforVisionErrors>(LookoutforVisionErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [ResourceArn]", false));
  }
  if (!request.TagKeysHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("UntagResource", "Required field: TagKeys, is not set");
    return UntagResourceOutcome(Aws::Client::AWSError<LookoutforVisionErrors>(LookoutforVisionErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [TagKeys]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, UntagResource, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/2020-11-20/tags/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetResourceArn());
  return UntagResourceOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_DELETE, Aws::Auth::SIGV4_SIGNER));
}

UpdateDatasetEntriesOutcome LookoutforVisionClient::UpdateDatasetEntries(const UpdateDatasetEntriesRequest& request) const
{
  AWS_OPERATION_GUARD(UpdateDatasetEntries);
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, UpdateDatasetEntries, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.ProjectNameHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("UpdateDatasetEntries", "Required field: ProjectName, is not set");
    return UpdateDatasetEntriesOutcome(Aws::Client::AWSError<LookoutforVisionErrors>(LookoutforVisionErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [ProjectName]", false));
  }
  if (!request.DatasetTypeHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("UpdateDatasetEntries", "Required field: DatasetType, is not set");
    return UpdateDatasetEntriesOutcome(Aws::Client::AWSError<LookoutforVisionErrors>(LookoutforVisionErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [DatasetType]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, UpdateDatasetEntries, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/2020-11-20/projects/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetProjectName());
  endpointResolutionOutcome.GetResult().AddPathSegments("/datasets/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetDatasetType());
  endpointResolutionOutcome.GetResult().AddPathSegments("/entries");
  return UpdateDatasetEntriesOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_PATCH, Aws::Auth::SIGV4_SIGNER));
}

