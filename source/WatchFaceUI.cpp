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

#include "uif-ui-watch-face-wrd/WatchFaceUI.h"

#include "mbed-time/Calendar.h"


#if 0
#include "swo/swo.h"
#define printf(...) { swoprintf(__VA_ARGS__); }
#else
#define printf(...)
#endif

#define WATCH_FACE_DEBUG 0

/* Set minimum time between animation frames.
*/
#define FRAME_LIMIT_MS 40

/* Constants to customize watch layout. To use a fixed clock face, comment out
   top_margin and left_margin in calculatePositionsWithXOffset:andYOffset.
*/
const static int center_space = 3; // space between hour/minutes and day/date
const static int line_space = 3;   // space between hour/day
const static int minute_kerning = 2; // space between x0/0x minutes

static const char * day_list[] = {
    "SUN",
    "MON",
    "TUE",
    "WED",
    "THU",
    "FRI",
    "SAT"
  };

WatchFaceUI::WatchFaceUI()
    :   UIView(),
        transitionTimeInMilliSeconds(250),
        animateMinute(false)
{
    /* Default size unless overwritten by parent. */
    UIView::width = 128;
    UIView::height = 128;

    /* Get initial time to display. */
    hour_int = Calendar::getHour();
    minute_int = Calendar::getMinute();
    second_int = Calendar::getSecond();
    date_int = Calendar::getDate();
    day_int = Calendar::getDay();

    printf("WatchFaceUI: %d %d %d %d %d\r\n",
            hour_int, minute_int, second_int, date_int, day_int);

    /* convert int to c-string */
    char hour_array[3];
    hour_array[0] = (hour_int / 10) + 48;
    hour_array[1] = (hour_int % 10) + 48;
    hour_array[2] = '\0';

    char minute_x0_array[2];
    minute_x0_array[0] = (minute_int / 10) + 48;
    minute_x0_array[1] = '\0';

    char minute_0x_array[2];
    minute_0x_array[0] = (minute_int % 10) + 48;
    minute_0x_array[1] = '\0';

    char date_array[3];
    date_array[0] = (date_int / 10) + 48;
    date_array[1] = (date_int % 10) + 48;
    date_array[2] = '\0';

    /* allocate cells */
    hour_cell = new UITextView(hour_array, &Font_Hours);
    next_hour_cell = hour_cell;

    minute_x0_cell = new UITextView(minute_x0_array, &Font_Minutes);
    minute_0x_cell = new UITextView(minute_0x_array, &Font_Minutes);
    next_minute_x0_cell = minute_x0_cell;
    next_minute_0x_cell = minute_0x_cell;

    day_cell = new UITextView(day_list[day_int], &Font_Dayndate);
    date_cell = new UITextView(date_array, &Font_Dayndate);

    /*  Pre-fetch. This converts the c-string into a cached CompBuf and allows
        the c-string to be freed when the stack unrolls.
    */
    hour_cell->prefetch(0, 0);
    minute_x0_cell->prefetch(0, 0);
    minute_0x_cell->prefetch(0, 0);
    day_cell->prefetch(0, 0);
    date_cell->prefetch(0, 0);

    /*  Invert cells to draw white on black.
    */
    hour_cell->setInverse(true);
    minute_x0_cell->setInverse(true);
    minute_0x_cell->setInverse(true);
    day_cell->setInverse(true);
    date_cell->setInverse(true);
}

void WatchFaceUI::calculateCenter(positions_t* positions,
                                  uint32_t progress,
                                  uint32_t period)
{
    /* center clock on screen. */

    if (progress < period)
    {
        uint32_t left_now = (width - hour_cell->getWidth()
                                   - minute_x0_cell->getWidth()
                                   - minute_0x_cell->getWidth()
                                   - center_space) / 2;

        uint32_t left_next = (width - next_hour_cell->getWidth()
                                    - next_minute_x0_cell->getWidth()
                                    - next_minute_0x_cell->getWidth()
                                    - center_space) / 2;

        if (left_next < left_now)
        {
            uint32_t diff = left_now - left_next;

            positions->left_margin = left_now - ((diff * progress) / period);
        }
        else
        {
            uint32_t diff = left_next - left_now;

            positions->left_margin = left_now + ((diff * progress) / period);
        }
    }
    else
    {
        positions->left_margin = (width - next_hour_cell->getWidth()
                                        - next_minute_x0_cell->getWidth()
                                        - next_minute_0x_cell->getWidth()
                                        - center_space) / 2;
    }

    positions->top_margin = (height - minute_x0_cell->getHeight()) / 2;
}

void WatchFaceUI::calculatePositions(positions_t* positions, int xOffset, int yOffset)
{
    /*  Calculate positions and offsets for all UIView compoinents.

        Animation positions and offsets are calculated locally.
     */
    int xBase = (xOffset < 0) ? xOffset : 0;
    int yBase = (yOffset < 0) ? yOffset : 0;

    /* most significant part of the minute */
    positions->minute_x0_x = xBase + positions->left_margin
                                   + hour_cell->getWidth()
                                   + center_space;

    positions->minute_x0_y = yBase + positions->top_margin;

    positions->minute_x0_x_offset = (positions->minute_x0_x < 0)
                                   ? positions->minute_x0_x : 0;

    positions->minute_x0_y_offset = (positions->minute_x0_y < 0)
                                   ? positions->minute_x0_y : 0;

    /* least significant part of the minute */
    positions->minute_0x_x = positions->minute_x0_x + minute_x0_cell->getWidth()
                                                    + minute_kerning;

    positions->minute_0x_y = yBase + positions->top_margin
                                   + minute_x0_cell->getHeight()
                                   - minute_0x_cell->getHeight();

    positions->minute_0x_x_offset = (positions->minute_0x_x < 0)
                                   ? positions->minute_0x_x : 0;

    positions->minute_0x_y_offset = (positions->minute_0x_y < 0)
                                   ? positions->minute_0x_y : 0;

    /* hour component */
    positions->hour_x = xBase + positions->left_margin;
    positions->hour_y = yBase + positions->top_margin;
    positions->hour_x_offset = (positions->hour_x < 0) ? positions->hour_x : 0;
    positions->hour_y_offset = (positions->hour_y < 0) ? positions->hour_y : 0;

    /* day */
    positions->day_x = positions->hour_x;
    positions->day_y = yBase + positions->top_margin
                             + hour_cell->getHeight()
                             + line_space;

    positions->day_x_offset = (positions->day_x < 0) ? positions->day_x : 0;
    positions->day_y_offset = (positions->day_y < 0) ? positions->day_y : 0;

    /* date */
    positions->date_x = positions->day_x + day_cell->getWidth() + center_space;
    positions->date_y = positions->day_y;
    positions->date_x_offset = (positions->date_x < 0) ? positions->date_x : 0;
    positions->date_y_offset = (positions->date_y < 0) ? positions->date_y : 0;
}


/*
 * :
 * :                         top_margin
 * :                                              center_space                      minute_kerning
 * :               ------------------------------              --------------------                --------------------
 * :              |               :              |            |                    |              |                    |
 * :              |               :              |            |                    |              |                    |
 * :              |                              |            |                    |              |                    |
 * : left_margin  |             hour             |            |      minute_x0     |              |      minute_0x     |
 * :              |                              |            |                    |              |                    |
 * :              |               :              |            |                    |              |                    |
 * :              |               :              |            |                    |              |                    |
 * :              |               :              |            |                    |              |                    |
 * :               ------------------------------             |                    |              |                    |
 * :                                                          |                    |              |                    |
 * :                line_space                                |                    |              |                    |
 * :                                                          |                    |              |                    |
 * :               --------- center_space -------             |                    |              |                    |
 * :              |         |            |       |            |                    |              |                    |
 * :              |  day    |            | date  |            |                    |              |                    |
 * :              |         |            |       |            |                    |              |                    |
 * :               ---------              -------              --------------------                --------------------
 * :
 */

uint32_t WatchFaceUI::fillFrameBuffer(SharedPointer<FrameBuffer>& canvas,
                                      int16_t xOffset,
                                      int16_t yOffset)
{
    /*  Get time, both for displaying but also for keeping track of call
        intervals.
    */
    uint8_t new_hour = 0;
    uint8_t new_minute = 0;
    uint8_t new_second = 0;

#if WATCH_FACE_DEBUG
    Calendar::getHour(&new_second, &new_hour, &new_minute);
#else
    Calendar::getHour(&new_hour, &new_minute, &new_second);
#endif

//    printf("calendar: %d %d %d\n\r", new_hour, new_minute, new_second);

    uint8_t new_date = Calendar::getDate();
    uint8_t new_day = Calendar::getDay();

    positions_t positions;

    /****************************************************************************
        Object creation
    */

    /* MINUTE
    */

    /*  Create new objects if the minute has changed and we are not still
        animating the previous change.
    */
    if ((new_minute != minute_int) && (animateMinute == false))
    {
        /*  Create new objects since we are animating the change. The next_ prefix
            is used to show which object/variable it replaces.
        */
        /* most significant minute digit */
        char next_minute_x0_array[2];
        next_minute_x0_array[0] = (new_minute / 10) + 48;
        next_minute_x0_array[1] = '\0';

        next_minute_x0_cell = new UITextView(next_minute_x0_array, &Font_Minutes);

        next_minute_x0_cell->prefetch(0, 0);
        next_minute_x0_cell->setInverse(true);
        next_minute_x0_cell->setHorizontalAlignment(UIView::ALIGN_LEFT);
        next_minute_x0_cell->setVerticalAlignment(UIView::VALIGN_TOP);

        /* least significant minute digit */
        char next_minute_0x_array[2];
        next_minute_0x_array[0] = (new_minute % 10) + 48;
        next_minute_0x_array[1] = '\0';

        next_minute_0x_cell = new UITextView(next_minute_0x_array, &Font_Minutes);

        next_minute_0x_cell->prefetch(0, 0);
        next_minute_0x_cell->setInverse(true);
        next_minute_0x_cell->setHorizontalAlignment(UIView::ALIGN_LEFT);
        next_minute_0x_cell->setVerticalAlignment(UIView::VALIGN_TOP);

        minute_int = new_minute;
        animateMinute = true;
        animateStartTime = UIView::getTimeInMilliseconds();
    }

    /* HOUR
    */
    /* Wait until minute has finished animating before updating hour
    */
    if (new_hour != hour_int)
    {
        char next_hour_array[3];
        next_hour_array[0] = (new_hour / 10) + 48;
        next_hour_array[1] = (new_hour % 10) + 48;
        next_hour_array[2] = '\0';

        next_hour_cell = new UITextView(next_hour_array, &Font_Hours);

        next_hour_cell->prefetch(0, 0);
        next_hour_cell->setInverse(true);
        next_hour_cell->setHorizontalAlignment(UIView::ALIGN_RIGHT);
        next_hour_cell->setVerticalAlignment(UIView::VALIGN_TOP);

        hour_int = new_hour;

        /*  If the hour changed without the minute changing as well,
            it means the time was set manually and the clock face should
            update immediately. Otherwise wait until the minute animation
            has completed before updating the hour.
        */
        if (animateMinute == false)
        {
            delete hour_cell;
            hour_cell = next_hour_cell;
        }
    }

    /*  DAY
    */
    if (new_day != day_int)
    {
        /* The name of the day is stored in the array day_list. */
        delete day_cell;
        day_cell = new UITextView(day_list[new_day], &Font_Dayndate);

        day_cell->prefetch(0, 0);
        day_cell->setInverse(true);
        day_cell->setHorizontalAlignment(UIView::ALIGN_LEFT);
        day_cell->setVerticalAlignment(UIView::VALIGN_TOP);

        day_int = new_day;
    }

    /*  DATE
    */
    if (new_date != date_int)
    {
        char date_array[3];
        date_array[0] = (new_date / 10) + 48;
        date_array[1] = (new_date % 10) + 48;
        date_array[2] = '\0';

        delete date_cell;
        date_cell = new UITextView(date_array, &Font_Dayndate);

        date_cell->prefetch(0, 0);
        date_cell->setInverse(true);
        date_cell->setHorizontalAlignment(UIView::ALIGN_RIGHT);
        date_cell->setVerticalAlignment(UIView::VALIGN_TOP);

        date_int = new_date;
    }

    /* Propagate xOffset and yOffset from parent to child objects. */

    /* fill canvas with black */
    canvas->drawRectangle(0, canvas->getWidth(), 0, canvas->getHeight(), 0);

    if (animateMinute)
    {
        /*  The animateOffset controls how fast the numbers scroll by.
            Ideally the offset increment should be a function of how long ago the
            last call came.
        */
        uint32_t now = UIView::getTimeInMilliseconds();
        uint32_t progress = now - animateStartTime;
        uint32_t animateOffset = (height * progress) / transitionTimeInMilliSeconds;

        /*  Calculate positions and offsets for all UIView compoinents.
            Animation positions and offsets are calculated locally.
        */
        calculateCenter(&positions, progress, transitionTimeInMilliSeconds);
        calculatePositions(&positions, xOffset, yOffset);

        /*  The animation is over when the offset has been cycled through a
            complete perceived height.
        */
        if (animateOffset < height)
        {
            /*  Digits are animated individually and only when they change.
            */
            if ((minute_int % 10) == 0)
            {
                /*  The old digit is moved up by subtracting animateOffset to the
                    y-coordinate. The new digit is moved up by first adding the
                    perceived screen height before subtracting the animateOffset.
                    The xOffset and yOffset is only applied when it is negative.
                    This asymmetry comes from the CompBuf pointing to the beginning
                    of the data buffer.
                */
                /* calculate position */
                positions.minute_x0_y -= animateOffset;
                positions.minute_0x_y -= animateOffset;
                positions.minute_x0_y_offset = (positions.minute_x0_y < 0) ?
                                                positions.minute_x0_y : 0;
                positions.minute_0x_y_offset = (positions.minute_0x_y < 0) ?
                                                positions.minute_0x_y : 0;

                /* old digits */
                /* create sub canvas */
                SharedPointer<FrameBuffer> minute_x0_canvas =
                             canvas->getFrameBuffer(positions.minute_x0_x,
                                                    positions.minute_x0_y,
                                                    minute_x0_cell->getWidth(),
                                                    minute_x0_cell->getHeight());

                /* fill sub canvas */
                minute_x0_cell->fillFrameBuffer(minute_x0_canvas,
                                                positions.minute_x0_x_offset,
                                                positions.minute_0x_y_offset);

                /* create sub canvas */
                SharedPointer<FrameBuffer> minute_0x_canvas =
                             canvas->getFrameBuffer(positions.minute_0x_x,
                                                    positions.minute_0x_y,
                                                    minute_0x_cell->getWidth(),
                                                    minute_0x_cell->getHeight());
                /* fill sub canvas */
                minute_0x_cell->fillFrameBuffer(minute_0x_canvas,
                                                positions.minute_0x_x_offset,
                                                positions.minute_0x_y_offset);

                /* new digits */
                /* calculate position */
                int32_t next_minute_x0_y = positions.minute_x0_y + height;
                int32_t next_minute_0x_x = positions.minute_x0_x +
                                           next_minute_x0_cell->getWidth() +
                                           minute_kerning;
                int32_t next_minute_0x_y = positions.minute_0x_y + height;

                int32_t next_minute_x0_y_offset = (next_minute_x0_y < 0) ?
                                                   next_minute_x0_y : 0;
                int32_t next_minute_0x_y_offset = (next_minute_0x_y < 0) ?
                                                   next_minute_0x_y : 0;

                /* create sub canvas */
                SharedPointer<FrameBuffer> next_minute_x0_canvas =
                         canvas->getFrameBuffer(positions.minute_x0_x,
                                                next_minute_x0_y,
                                                next_minute_x0_cell->getWidth(),
                                                next_minute_x0_cell->getHeight());
                /* fill sub canvas */
                next_minute_x0_cell->fillFrameBuffer(next_minute_x0_canvas,
                                                     positions.minute_x0_x_offset,
                                                     next_minute_x0_y_offset);

                /* create sub canvas */
                SharedPointer<FrameBuffer> next_minute_0x_canvas =
                        canvas->getFrameBuffer(next_minute_0x_x,
                                               next_minute_0x_y,
                                               next_minute_0x_cell->getWidth(),
                                               next_minute_0x_cell->getHeight());

                /* fill sub canvas */
                next_minute_0x_cell->fillFrameBuffer(next_minute_0x_canvas,
                                                     positions.minute_0x_x_offset,
                                                     next_minute_0x_y_offset);
            }
            else
            {
                /* stationary digit */
                SharedPointer<FrameBuffer> minute_x0_canvas =
                             canvas->getFrameBuffer(positions.minute_x0_x,
                                                    positions.minute_x0_y,
                                                    minute_x0_cell->getWidth(),
                                                    minute_x0_cell->getHeight());

                /* fill sub canvas */
                minute_x0_cell->fillFrameBuffer(minute_x0_canvas,
                                                positions.minute_x0_x_offset,
                                                positions.minute_x0_y_offset);

                /* scrolling digit */
                /* old digit */
                positions.minute_0x_y -= animateOffset;
                positions.minute_0x_y_offset = (positions.minute_0x_y < 0) ?
                                                positions.minute_0x_y : 0;

                /* create sub canvas */
                SharedPointer<FrameBuffer> minute_0x_canvas =
                             canvas->getFrameBuffer(positions.minute_0x_x,
                                                    positions.minute_0x_y,
                                                    minute_0x_cell->getWidth(),
                                                    minute_0x_cell->getHeight());

                /* fill sub canvas */
                minute_0x_cell->fillFrameBuffer(minute_0x_canvas,
                                                positions.minute_0x_x_offset,
                                                positions.minute_0x_y_offset);

                /* new digit */
                int32_t next_minute_0x_y = positions.minute_0x_y + height;
                int32_t next_minute_0x_y_offset = (next_minute_0x_y < 0) ?
                                                   next_minute_0x_y : 0;

                /* create sub canvas */
                SharedPointer<FrameBuffer> next_minute_0x_canvas =
                        canvas->getFrameBuffer(positions.minute_0x_x,
                                               next_minute_0x_y,
                                               next_minute_0x_cell->getWidth(),
                                               next_minute_0x_cell->getHeight());

                /* fill sub canvas */
                next_minute_0x_cell->fillFrameBuffer(next_minute_0x_canvas,
                                                     positions.minute_0x_x_offset,
                                                     next_minute_0x_y_offset);
            }
        }
        else
        {
            /*  Animation is complete when the animateOffset is larger than the
                percevied screen height. Cleanup by replacing the old objects
                with the new ones.
            */
            animateMinute = false;

            if (hour_cell != next_hour_cell)
            {
                delete hour_cell;

                hour_cell = next_hour_cell;
            }

            if (minute_x0_cell != next_minute_x0_cell)
            {
                delete minute_x0_cell;

                minute_x0_cell = next_minute_x0_cell;
            }

            if (minute_0x_cell != next_minute_0x_cell)
            {
                delete minute_0x_cell;

                minute_0x_cell = next_minute_0x_cell;
            }

            /*  Update positions for UIView elements now that the animation
                is over.
            */
            calculatePositions(&positions, xOffset, yOffset);

            /* Update screen with the stationary digits */
            /* create sub canvas */
            SharedPointer<FrameBuffer> minute_x0_canvas =
                           canvas->getFrameBuffer(positions.minute_x0_x,
                                                  positions.minute_x0_y,
                                                  minute_x0_cell->getWidth(),
                                                  minute_x0_cell->getHeight());

            /* fill sub canvas */
            minute_x0_cell->fillFrameBuffer(minute_x0_canvas,
                                            positions.minute_x0_x_offset,
                                            positions.minute_x0_y_offset);

            /* create sub canvas */
            SharedPointer<FrameBuffer> minute_0x_canvas =
                           canvas->getFrameBuffer(positions.minute_0x_x,
                                                  positions.minute_0x_y,
                                                  minute_0x_cell->getWidth(),
                                                  minute_0x_cell->getHeight());

            /* fill sub canvas */
            minute_0x_cell->fillFrameBuffer(minute_0x_canvas,
                                            positions.minute_0x_x_offset,
                                            positions.minute_0x_y_offset);
        }
    }
    else
    {
        /*  Calculate positions and offsets for all UIView compoinents.
            Animation positions and offsets are calculated locally.
        */
        calculateCenter(&positions, 0, 0);
        calculatePositions(&positions, xOffset, yOffset);

        /* Update screen with the stationary digits */
        /* create sub canvas */
        SharedPointer<FrameBuffer> minute_x0_canvas =
                         canvas->getFrameBuffer(positions.minute_x0_x,
                                                positions.minute_x0_y,
                                                minute_x0_cell->getWidth(),
                                                minute_x0_cell->getHeight());
        /* fill sub canvas */
        minute_x0_cell->fillFrameBuffer(minute_x0_canvas,
                                        positions.minute_x0_x_offset,
                                        positions.minute_x0_y_offset);

        /* create sub canvas */
        SharedPointer<FrameBuffer> minute_0x_canvas =
                         canvas->getFrameBuffer(positions.minute_0x_x,
                                                positions.minute_0x_y,
                                                minute_0x_cell->getWidth(),
                                                minute_0x_cell->getHeight());

        /* fill sub canvas */
        minute_0x_cell->fillFrameBuffer(minute_0x_canvas,
                                        positions.minute_0x_x_offset,
                                        positions.minute_0x_y_offset);
    }


    /*  HOUR
    */

    /* calculate final (x,y) position and offset for the hour UIView and fill canvas. */
    SharedPointer<FrameBuffer> hour_canvas =
                             canvas->getFrameBuffer(positions.hour_x,
                                                    positions.hour_y,
                                                    hour_cell->getWidth(),
                                                    hour_cell->getHeight());

    hour_cell->fillFrameBuffer(hour_canvas,
                               positions.hour_x_offset,
                               positions.hour_y_offset);


    /*  DAY
    */

    /* calculate final (x,y) position and offset for the day UIView and fill canvas. */
    SharedPointer<FrameBuffer> day_canvas =
                             canvas->getFrameBuffer(positions.day_x,
                                                    positions.day_y,
                                                    day_cell->getWidth(),
                                                    day_cell->getHeight());

    day_cell->fillFrameBuffer(day_canvas,
                              positions.day_x_offset,
                              positions.day_y_offset);


    /*  DATE
    */

    /* calculate final (x,y) position and offset for the date UIView and fill canvas. */
    SharedPointer<FrameBuffer> date_canvas =
                             canvas->getFrameBuffer(positions.date_x,
                                                    positions.date_y,
                                                    day_cell->getWidth(),
                                                    day_cell->getHeight());

    date_cell->fillFrameBuffer(date_canvas,
                               positions.date_x_offset,
                               positions.date_y_offset);

#if WATCH_FACE_DEBUG
    return (animateMinute) ? FRAME_LIMIT_MS : 100;
#else
    // update watch face every minute if animation not in progress
    return (animateMinute) ? FRAME_LIMIT_MS : (60 - new_second) * 1000;
#endif
}
