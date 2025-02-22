﻿/**
 * Copyright Amazon.com, Inc. or its affiliates. All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0.
 */

#pragma once
#include <aws/billingconductor/BillingConductor_EXPORTS.h>
#include <aws/core/utils/memory/stl/AWSString.h>
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
namespace BillingConductor
{
namespace Model
{
  class DeleteCustomLineItemResult
  {
  public:
    AWS_BILLINGCONDUCTOR_API DeleteCustomLineItemResult();
    AWS_BILLINGCONDUCTOR_API DeleteCustomLineItemResult(const Aws::AmazonWebServiceResult<Aws::Utils::Json::JsonValue>& result);
    AWS_BILLINGCONDUCTOR_API DeleteCustomLineItemResult& operator=(const Aws::AmazonWebServiceResult<Aws::Utils::Json::JsonValue>& result);


    /**
     * <p> Then ARN of the deleted custom line item. </p>
     */
    inline const Aws::String& GetArn() const{ return m_arn; }

    /**
     * <p> Then ARN of the deleted custom line item. </p>
     */
    inline void SetArn(const Aws::String& value) { m_arn = value; }

    /**
     * <p> Then ARN of the deleted custom line item. </p>
     */
    inline void SetArn(Aws::String&& value) { m_arn = std::move(value); }

    /**
     * <p> Then ARN of the deleted custom line item. </p>
     */
    inline void SetArn(const char* value) { m_arn.assign(value); }

    /**
     * <p> Then ARN of the deleted custom line item. </p>
     */
    inline DeleteCustomLineItemResult& WithArn(const Aws::String& value) { SetArn(value); return *this;}

    /**
     * <p> Then ARN of the deleted custom line item. </p>
     */
    inline DeleteCustomLineItemResult& WithArn(Aws::String&& value) { SetArn(std::move(value)); return *this;}

    /**
     * <p> Then ARN of the deleted custom line item. </p>
     */
    inline DeleteCustomLineItemResult& WithArn(const char* value) { SetArn(value); return *this;}


    
    inline const Aws::String& GetRequestId() const{ return m_requestId; }

    
    inline void SetRequestId(const Aws::String& value) { m_requestId = value; }

    
    inline void SetRequestId(Aws::String&& value) { m_requestId = std::move(value); }

    
    inline void SetRequestId(const char* value) { m_requestId.assign(value); }

    
    inline DeleteCustomLineItemResult& WithRequestId(const Aws::String& value) { SetRequestId(value); return *this;}

    
    inline DeleteCustomLineItemResult& WithRequestId(Aws::String&& value) { SetRequestId(std::move(value)); return *this;}

    
    inline DeleteCustomLineItemResult& WithRequestId(const char* value) { SetRequestId(value); return *this;}

  private:

    Aws::String m_arn;

    Aws::String m_requestId;
  };

} // namespace Model
} // namespace BillingConductor
} // namespace Aws
