/*
 * Copyright (c) 2016, ARM Limited, All Rights Reserved
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License"); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef __UIF_UI_WATCHFACEUI_H__
#define __UIF_UI_WATCHFACEUI_H__

#include "UIFramework/UIView.h"
#include "UIFramework/UITextView.h"

class WatchFaceUI : public UIView
{
public:
    /**
     * @brief UIFramework implementation of the Wearable Reference Design
     *        watch face.
     * @details The Wearable Reference Design watch face is a digital clock
     *          showing hour, minute, weekday, and day in month.
     */
    WatchFaceUI();

    /**
     * @brief Fill frame buffer with the current time and WRD watch face.
     *
     * @param canvas FrameBuffer-object wrapped in a SharedPointer.
     * @param xOffset Number of pixels the camera has been translated along the
     *        horizontal axis.
     * @param yOffset Number of pixels the camera has been translated along the
     *        vertical axis.
     * @return The time in milliseconds to when the object wants to be called
     *         again. This is a lower-bound, meaning calling the function sooner
     *         will only result in the same data being filled into the frame
     *         buffer again.
     */
    virtual uint32_t fillFrameBuffer(SharedPointer<FrameBuffer>& canvas, int16_t xOffset, int16_t yOffset);

private:
    typedef struct {
        int32_t top_margin;
        int32_t left_margin;

        /* most significant part of the minute */
        int32_t minute_x0_x;
        int32_t minute_x0_y;
        int32_t minute_x0_x_offset;
        int32_t minute_x0_y_offset;

        /* least significant part of the minute */
        int32_t minute_0x_x;
        int32_t minute_0x_y;
        int32_t minute_0x_x_offset;
        int32_t minute_0x_y_offset;

        /* hour component */
        int32_t hour_x;
        int32_t hour_y;
        int32_t hour_x_offset;
        int32_t hour_y_offset;

        /* day */
        int32_t day_x;
        int32_t day_y;
        int32_t day_x_offset;
        int32_t day_y_offset;

        /* date */
        int32_t date_x;
        int32_t date_y;
        int32_t date_x_offset;
        int32_t date_y_offset;
    } positions_t;

    /**
     * @brief Calculate new center position with animation taken into account.
     *
     * @param positions posision_t struct to be filled by function.
     * @param progress Animation progress as a fraction of the total period,
     *        i.e., 0 <= progress <= period.
     * @param period Animation period.
     */
    void calculateCenter(positions_t* positions, uint32_t progress, uint32_t period);

    /**
     * @brief Calculate positions based on the camera view.
     *
     * @param positions posision_t struct to be filled by function.
     * @param xOffset Number of pixels the camera has been translated along the
     *        horizontal axis.
     * @param yOffset Number of pixels the camera has been translated along the
     *        vertical axis.
     */
    void calculatePositions(positions_t* positions, int xOffset, int yOffset);

    uint32_t transitionTimeInMilliSeconds;

    /* Cache cells between calls. */
    UITextView* hour_cell;
    UITextView* next_hour_cell;
    UITextView* minute_x0_cell;
    UITextView* minute_0x_cell;
    UITextView* next_minute_x0_cell;
    UITextView* next_minute_0x_cell;
    UITextView* day_cell;
    UITextView* date_cell;

    /* Keep track of changes in time. Triggers animations. */
    uint8_t hour_int;
    uint8_t minute_int;
    uint8_t second_int;
    uint8_t date_int;
    uint8_t day_int;

    /* Controls animations. */
    bool animateMinute;
    uint32_t animateStartTime;
};

#endif // __UIF_UI_WATCHFACEUI_H__
