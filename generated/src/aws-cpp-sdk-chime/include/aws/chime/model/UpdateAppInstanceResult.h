﻿/**
 * Copyright Amazon.com, Inc. or its affiliates. All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0.
 */

#pragma once
#include <aws/chime/Chime_EXPORTS.h>
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
namespace Chime
{
namespace Model
{
  class UpdateAppInstanceResult
  {
  public:
    AWS_CHIME_API UpdateAppInstanceResult();
    AWS_CHIME_API UpdateAppInstanceResult(const Aws::AmazonWebServiceResult<Aws::Utils::Json::JsonValue>& result);
    AWS_CHIME_API UpdateAppInstanceResult& operator=(const Aws::AmazonWebServiceResult<Aws::Utils::Json::JsonValue>& result);


    /**
     * <p>The ARN of the <code>AppInstance</code>. </p>
     */
    inline const Aws::String& GetAppInstanceArn() const{ return m_appInstanceArn; }

    /**
     * <p>The ARN of the <code>AppInstance</code>. </p>
     */
    inline void SetAppInstanceArn(const Aws::String& value) { m_appInstanceArn = value; }

    /**
     * <p>The ARN of the <code>AppInstance</code>. </p>
     */
    inline void SetAppInstanceArn(Aws::String&& value) { m_appInstanceArn = std::move(value); }

    /**
     * <p>The ARN of the <code>AppInstance</code>. </p>
     */
    inline void SetAppInstanceArn(const char* value) { m_appInstanceArn.assign(value); }

    /**
     * <p>The ARN of the <code>AppInstance</code>. </p>
     */
    inline UpdateAppInstanceResult& WithAppInstanceArn(const Aws::String& value) { SetAppInstanceArn(value); return *this;}

    /**
     * <p>The ARN of the <code>AppInstance</code>. </p>
     */
    inline UpdateAppInstanceResult& WithAppInstanceArn(Aws::String&& value) { SetAppInstanceArn(std::move(value)); return *this;}

    /**
     * <p>The ARN of the <code>AppInstance</code>. </p>
     */
    inline UpdateAppInstanceResult& WithAppInstanceArn(const char* value) { SetAppInstanceArn(value); return *this;}


    
    inline const Aws::String& GetRequestId() const{ return m_requestId; }

    
    inline void SetRequestId(const Aws::String& value) { m_requestId = value; }

    
    inline void SetRequestId(Aws::String&& value) { m_requestId = std::move(value); }

    
    inline void SetRequestId(const char* value) { m_requestId.assign(value); }

    
    inline UpdateAppInstanceResult& WithRequestId(const Aws::String& value) { SetRequestId(value); return *this;}

    
    inline UpdateAppInstanceResult& WithRequestId(Aws::String&& value) { SetRequestId(std::move(value)); return *this;}

    
    inline UpdateAppInstanceResult& WithRequestId(const char* value) { SetRequestId(value); return *this;}

  private:

    Aws::String m_appInstanceArn;

    Aws::String m_requestId;
  };

} // namespace Model
} // namespace Chime
} // namespace Aws
