﻿/**
 * Copyright Amazon.com, Inc. or its affiliates. All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0.
 */

#pragma once
#include <aws/securitylake/SecurityLake_EXPORTS.h>
#include <aws/securitylake/SecurityLakeRequest.h>
#include <aws/core/utils/memory/stl/AWSVector.h>
#include <aws/core/utils/memory/stl/AWSString.h>
#include <aws/securitylake/model/DataLakeConfiguration.h>
#include <utility>

namespace Aws
{
namespace SecurityLake
{
namespace Model
{

  /**
   */
  class CreateDataLakeRequest : public SecurityLakeRequest
  {
  public:
    AWS_SECURITYLAKE_API CreateDataLakeRequest();

    // Service request name is the Operation name which will send this request out,
    // each operation should has unique request name, so that we can get operation's name from this request.
    // Note: this is not true for response, multiple operations may have the same response name,
    // so we can not get operation's name from response.
    inline virtual const char* GetServiceRequestName() const override { return "CreateDataLake"; }

    AWS_SECURITYLAKE_API Aws::String SerializePayload() const override;


    /**
     * <p>Specify the Region or Regions that will contribute data to the rollup
     * region.</p>
     */
    inline const Aws::Vector<DataLakeConfiguration>& GetConfigurations() const{ return m_configurations; }

    /**
     * <p>Specify the Region or Regions that will contribute data to the rollup
     * region.</p>
     */
    inline bool ConfigurationsHasBeenSet() const { return m_configurationsHasBeenSet; }

    /**
     * <p>Specify the Region or Regions that will contribute data to the rollup
     * region.</p>
     */
    inline void SetConfigurations(const Aws::Vector<DataLakeConfiguration>& value) { m_configurationsHasBeenSet = true; m_configurations = value; }

    /**
     * <p>Specify the Region or Regions that will contribute data to the rollup
     * region.</p>
     */
    inline void SetConfigurations(Aws::Vector<DataLakeConfiguration>&& value) { m_configurationsHasBeenSet = true; m_configurations = std::move(value); }

    /**
     * <p>Specify the Region or Regions that will contribute data to the rollup
     * region.</p>
     */
    inline CreateDataLakeRequest& WithConfigurations(const Aws::Vector<DataLakeConfiguration>& value) { SetConfigurations(value); return *this;}

    /**
     * <p>Specify the Region or Regions that will contribute data to the rollup
     * region.</p>
     */
    inline CreateDataLakeRequest& WithConfigurations(Aws::Vector<DataLakeConfiguration>&& value) { SetConfigurations(std::move(value)); return *this;}

    /**
     * <p>Specify the Region or Regions that will contribute data to the rollup
     * region.</p>
     */
    inline CreateDataLakeRequest& AddConfigurations(const DataLakeConfiguration& value) { m_configurationsHasBeenSet = true; m_configurations.push_back(value); return *this; }

    /**
     * <p>Specify the Region or Regions that will contribute data to the rollup
     * region.</p>
     */
    inline CreateDataLakeRequest& AddConfigurations(DataLakeConfiguration&& value) { m_configurationsHasBeenSet = true; m_configurations.push_back(std::move(value)); return *this; }


    /**
     * <p>The Amazon Resource Name (ARN) used to create and update the Glue table. This
     * table contains partitions generated by the ingestion and normalization of Amazon
     * Web Services log sources and custom sources.</p>
     */
    inline const Aws::String& GetMetaStoreManagerRoleArn() const{ return m_metaStoreManagerRoleArn; }

    /**
     * <p>The Amazon Resource Name (ARN) used to create and update the Glue table. This
     * table contains partitions generated by the ingestion and normalization of Amazon
     * Web Services log sources and custom sources.</p>
     */
    inline bool MetaStoreManagerRoleArnHasBeenSet() const { return m_metaStoreManagerRoleArnHasBeenSet; }

    /**
     * <p>The Amazon Resource Name (ARN) used to create and update the Glue table. This
     * table contains partitions generated by the ingestion and normalization of Amazon
     * Web Services log sources and custom sources.</p>
     */
    inline void SetMetaStoreManagerRoleArn(const Aws::String& value) { m_metaStoreManagerRoleArnHasBeenSet = true; m_metaStoreManagerRoleArn = value; }

    /**
     * <p>The Amazon Resource Name (ARN) used to create and update the Glue table. This
     * table contains partitions generated by the ingestion and normalization of Amazon
     * Web Services log sources and custom sources.</p>
     */
    inline void SetMetaStoreManagerRoleArn(Aws::String&& value) { m_metaStoreManagerRoleArnHasBeenSet = true; m_metaStoreManagerRoleArn = std::move(value); }

    /**
     * <p>The Amazon Resource Name (ARN) used to create and update the Glue table. This
     * table contains partitions generated by the ingestion and normalization of Amazon
     * Web Services log sources and custom sources.</p>
     */
    inline void SetMetaStoreManagerRoleArn(const char* value) { m_metaStoreManagerRoleArnHasBeenSet = true; m_metaStoreManagerRoleArn.assign(value); }

    /**
     * <p>The Amazon Resource Name (ARN) used to create and update the Glue table. This
     * table contains partitions generated by the ingestion and normalization of Amazon
     * Web Services log sources and custom sources.</p>
     */
    inline CreateDataLakeRequest& WithMetaStoreManagerRoleArn(const Aws::String& value) { SetMetaStoreManagerRoleArn(value); return *this;}

    /**
     * <p>The Amazon Resource Name (ARN) used to create and update the Glue table. This
     * table contains partitions generated by the ingestion and normalization of Amazon
     * Web Services log sources and custom sources.</p>
     */
    inline CreateDataLakeRequest& WithMetaStoreManagerRoleArn(Aws::String&& value) { SetMetaStoreManagerRoleArn(std::move(value)); return *this;}

    /**
     * <p>The Amazon Resource Name (ARN) used to create and update the Glue table. This
     * table contains partitions generated by the ingestion and normalization of Amazon
     * Web Services log sources and custom sources.</p>
     */
    inline CreateDataLakeRequest& WithMetaStoreManagerRoleArn(const char* value) { SetMetaStoreManagerRoleArn(value); return *this;}

  private:

    Aws::Vector<DataLakeConfiguration> m_configurations;
    bool m_configurationsHasBeenSet = false;

    Aws::String m_metaStoreManagerRoleArn;
    bool m_metaStoreManagerRoleArnHasBeenSet = false;
  };

} // namespace Model
} // namespace SecurityLake
} // namespace Aws
