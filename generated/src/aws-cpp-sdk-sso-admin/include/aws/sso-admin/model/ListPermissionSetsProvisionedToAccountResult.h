﻿/**
 * Copyright Amazon.com, Inc. or its affiliates. All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0.
 */

#pragma once
#include <aws/sso-admin/SSOAdmin_EXPORTS.h>
#include <aws/core/utils/memory/stl/AWSString.h>
#include <aws/core/utils/memory/stl/AWSVector.h>
#include <utility>

namespace Aws
{
template<typename RESULT_TYPE>
class AmazonWebServiceResult;

namespace Utils
{
namespace Json
{
  class JsonValue;
} // namespace Json
} // namespace Utils
namespace SSOAdmin
{
namespace Model
{
  class ListPermissionSetsProvisionedToAccountResult
  {
  public:
    AWS_SSOADMIN_API ListPermissionSetsProvisionedToAccountResult();
    AWS_SSOADMIN_API ListPermissionSetsProvisionedToAccountResult(const Aws::AmazonWebServiceResult<Aws::Utils::Json::JsonValue>& result);
    AWS_SSOADMIN_API ListPermissionSetsProvisionedToAccountResult& operator=(const Aws::AmazonWebServiceResult<Aws::Utils::Json::JsonValue>& result);


    /**
     * <p>The pagination token for the list API. Initially the value is null. Use the
     * output of previous API calls to make subsequent calls.</p>
     */
    inline const Aws::String& GetNextToken() const{ return m_nextToken; }

    /**
     * <p>The pagination token for the list API. Initially the value is null. Use the
     * output of previous API calls to make subsequent calls.</p>
     */
    inline void SetNextToken(const Aws::String& value) { m_nextToken = value; }

    /**
     * <p>The pagination token for the list API. Initially the value is null. Use the
     * output of previous API calls to make subsequent calls.</p>
     */
    inline void SetNextToken(Aws::String&& value) { m_nextToken = std::move(value); }

    /**
     * <p>The pagination token for the list API. Initially the value is null. Use the
     * output of previous API calls to make subsequent calls.</p>
     */
    inline void SetNextToken(const char* value) { m_nextToken.assign(value); }

    /**
     * <p>The pagination token for the list API. Initially the value is null. Use the
     * output of previous API calls to make subsequent calls.</p>
     */
    inline ListPermissionSetsProvisionedToAccountResult& WithNextToken(const Aws::String& value) { SetNextToken(value); return *this;}

    /**
     * <p>The pagination token for the list API. Initially the value is null. Use the
     * output of previous API calls to make subsequent calls.</p>
     */
    inline ListPermissionSetsProvisionedToAccountResult& WithNextToken(Aws::String&& value) { SetNextToken(std::move(value)); return *this;}

    /**
     * <p>The pagination token for the list API. Initially the value is null. Use the
     * output of previous API calls to make subsequent calls.</p>
     */
    inline ListPermissionSetsProvisionedToAccountResult& WithNextToken(const char* value) { SetNextToken(value); return *this;}


    /**
     * <p>Defines the level of access that an AWS account has.</p>
     */
    inline const Aws::Vector<Aws::String>& GetPermissionSets() const{ return m_permissionSets; }

    /**
     * <p>Defines the level of access that an AWS account has.</p>
     */
    inline void SetPermissionSets(const Aws::Vector<Aws::String>& value) { m_permissionSets = value; }

    /**
     * <p>Defines the level of access that an AWS account has.</p>
     */
    inline void SetPermissionSets(Aws::Vector<Aws::String>&& value) { m_permissionSets = std::move(value); }

    /**
     * <p>Defines the level of access that an AWS account has.</p>
     */
    inline ListPermissionSetsProvisionedToAccountResult& WithPermissionSets(const Aws::Vector<Aws::String>& value) { SetPermissionSets(value); return *this;}

    /**
     * <p>Defines the level of access that an AWS account has.</p>
     */
    inline ListPermissionSetsProvisionedToAccountResult& WithPermissionSets(Aws::Vector<Aws::String>&& value) { SetPermissionSets(std::move(value)); return *this;}

    /**
     * <p>Defines the level of access that an AWS account has.</p>
     */
    inline ListPermissionSetsProvisionedToAccountResult& AddPermissionSets(const Aws::String& value) { m_permissionSets.push_back(value); return *this; }

    /**
     * <p>Defines the level of access that an AWS account has.</p>
     */
    inline ListPermissionSetsProvisionedToAccountResult& AddPermissionSets(Aws::String&& value) { m_permissionSets.push_back(std::move(value)); return *this; }

    /**
     * <p>Defines the level of access that an AWS account has.</p>
     */
    inline ListPermissionSetsProvisionedToAccountResult& AddPermissionSets(const char* value) { m_permissionSets.push_back(value); return *this; }


    
    inline const Aws::String& GetRequestId() const{ return m_requestId; }

    
    inline void SetRequestId(const Aws::String& value) { m_requestId = value; }

    
    inline void SetRequestId(Aws::String&& value) { m_requestId = std::move(value); }

    
    inline void SetRequestId(const char* value) { m_requestId.assign(value); }

    
    inline ListPermissionSetsProvisionedToAccountResult& WithRequestId(const Aws::String& value) { SetRequestId(value); return *this;}

    
    inline ListPermissionSetsProvisionedToAccountResult& WithRequestId(Aws::String&& value) { SetRequestId(std::move(value)); return *this;}

    
    inline ListPermissionSetsProvisionedToAccountResult& WithRequestId(const char* value) { SetRequestId(value); return *this;}

  private:

    Aws::String m_nextToken;

    Aws::Vector<Aws::String> m_permissionSets;

    Aws::String m_requestId;
  };

} // namespace Model
} // namespace SSOAdmin
} // namespace Aws
