﻿/**
 * Copyright Amazon.com, Inc. or its affiliates. All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0.
 */

#pragma once
#include <aws/appstream/AppStream_EXPORTS.h>

namespace Aws
{
namespace Utils
{
namespace Json
{
  class JsonValue;
  class JsonView;
} // namespace Json
} // namespace Utils
namespace AppStream
{
namespace Model
{

  /**
   * <p>Describes the capacity status for a fleet.</p><p><h3>See Also:</h3>   <a
   * href="http://docs.aws.amazon.com/goto/WebAPI/appstream-2016-12-01/ComputeCapacityStatus">AWS
   * API Reference</a></p>
   */
  class ComputeCapacityStatus
  {
  public:
    AWS_APPSTREAM_API ComputeCapacityStatus();
    AWS_APPSTREAM_API ComputeCapacityStatus(Aws::Utils::Json::JsonView jsonValue);
    AWS_APPSTREAM_API ComputeCapacityStatus& operator=(Aws::Utils::Json::JsonView jsonValue);
    AWS_APPSTREAM_API Aws::Utils::Json::JsonValue Jsonize() const;


    /**
     * <p>The desired number of streaming instances.</p>
     */
    inline int GetDesired() const{ return m_desired; }

    /**
     * <p>The desired number of streaming instances.</p>
     */
    inline bool DesiredHasBeenSet() const { return m_desiredHasBeenSet; }

    /**
     * <p>The desired number of streaming instances.</p>
     */
    inline void SetDesired(int value) { m_desiredHasBeenSet = true; m_desired = value; }

    /**
     * <p>The desired number of streaming instances.</p>
     */
    inline ComputeCapacityStatus& WithDesired(int value) { SetDesired(value); return *this;}


    /**
     * <p>The total number of simultaneous streaming instances that are running.</p>
     */
    inline int GetRunning() const{ return m_running; }

    /**
     * <p>The total number of simultaneous streaming instances that are running.</p>
     */
    inline bool RunningHasBeenSet() const { return m_runningHasBeenSet; }

    /**
     * <p>The total number of simultaneous streaming instances that are running.</p>
     */
    inline void SetRunning(int value) { m_runningHasBeenSet = true; m_running = value; }

    /**
     * <p>The total number of simultaneous streaming instances that are running.</p>
     */
    inline ComputeCapacityStatus& WithRunning(int value) { SetRunning(value); return *this;}


    /**
     * <p>The number of instances in use for streaming.</p>
     */
    inline int GetInUse() const{ return m_inUse; }

    /**
     * <p>The number of instances in use for streaming.</p>
     */
    inline bool InUseHasBeenSet() const { return m_inUseHasBeenSet; }

    /**
     * <p>The number of instances in use for streaming.</p>
     */
    inline void SetInUse(int value) { m_inUseHasBeenSet = true; m_inUse = value; }

    /**
     * <p>The number of instances in use for streaming.</p>
     */
    inline ComputeCapacityStatus& WithInUse(int value) { SetInUse(value); return *this;}


    /**
     * <p>The number of currently available instances that can be used to stream
     * sessions.</p>
     */
    inline int GetAvailable() const{ return m_available; }

    /**
     * <p>The number of currently available instances that can be used to stream
     * sessions.</p>
     */
    inline bool AvailableHasBeenSet() const { return m_availableHasBeenSet; }

    /**
     * <p>The number of currently available instances that can be used to stream
     * sessions.</p>
     */
    inline void SetAvailable(int value) { m_availableHasBeenSet = true; m_available = value; }

    /**
     * <p>The number of currently available instances that can be used to stream
     * sessions.</p>
     */
    inline ComputeCapacityStatus& WithAvailable(int value) { SetAvailable(value); return *this;}

  private:

    int m_desired;
    bool m_desiredHasBeenSet = false;

    int m_running;
    bool m_runningHasBeenSet = false;

    int m_inUse;
    bool m_inUseHasBeenSet = false;

    int m_available;
    bool m_availableHasBeenSet = false;
  };

} // namespace Model
} // namespace AppStream
} // namespace Aws
