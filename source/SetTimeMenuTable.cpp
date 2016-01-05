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

#include "uif-ui-watch-face-wrd/SetTimeMenuTable.h"

#include "UIFramework/UITextView.h"
#include "UIFramework/UIImageView.h"

#include "uif-ui-watch-face-wrd/SetHourMenuTable.h"
#include "uif-ui-watch-face-wrd/SetMinuteMenuTable.h"
#include "uif-ui-watch-face-wrd/SetDateMenuTable.h"
#include "uif-ui-watch-face-wrd/SetMonthMenuTable.h"
#include "uif-ui-watch-face-wrd/SetYearMenuTable.h"

#ifndef __NUMBERSTRINGS_H__
#define __NUMBERSTRINGS_H__



const char* numberStringSingle[] = {
    "0",
    "1",
    "2",
    "3",
    "4",
    "5",
    "6",
    "7",
    "8",
    "9"
};

const char* numberStringDouble[] = {
    "00",
    "01",
    "02",
    "03",
    "04",
    "05",
    "06",
    "07",
    "08",
    "09",

    "10",
    "11",
    "12",
    "13",
    "14",
    "15",
    "16",
    "17",
    "18",
    "19",

    "20",
    "21",
    "22",
    "23",
    "24",
    "25",
    "26",
    "27",
    "28",
    "29",

    "30",
    "31",
    "32",
    "33",
    "34",
    "35",
    "36",
    "37",
    "38",
    "39",

    "40",
    "41",
    "42",
    "43",
    "44",
    "45",
    "46",
    "47",
    "48",
    "49",

    "50",
    "51",
    "52",
    "53",
    "54",
    "55",
    "56",
    "57",
    "58",
    "59",

    "60"
};

#endif

typedef enum {
    CELL_TOP_FILLER = 0, // Always first
    CELL_HOUR,
    CELL_MINUTE,
    CELL_DATE,
    CELL_MONTH,
    CELL_YEAR,
    // Always last
    CELL_END_FILLER
} entries_t;

SharedPointer<UIView> SetTimeMenuTable::viewAtIndex(uint32_t index) const
{
    UIView* cell = NULL;

    switch(index)
    {
        case CELL_TOP_FILLER:
                // top filler
                cell = new UIImageView(NULL);
                break;

        // time
        case CELL_HOUR:
                cell = new UITextView("Hour", &Font_Menu);
                break;
        case CELL_MINUTE:
                cell = new UITextView("Minute", &Font_Menu);
                break;

        // date
        case CELL_DATE:
                cell = new UITextView("Date", &Font_Menu);
                break;
        case CELL_MONTH:
                cell = new UITextView("Month", &Font_Menu);
                break;
        case CELL_YEAR:
                cell = new UITextView("Year", &Font_Menu);
                break;

        case CELL_END_FILLER:
        default:
                // bottom filler
                cell = new UIImageView(NULL);
                break;
    }

    if (cell)
    {
        cell->setHorizontalAlignment(UIView::ALIGN_LEFT);
        cell->setVerticalAlignment(UIView::VALIGN_MIDDLE);
        cell->setWidth(0);
        cell->setHeight(0);
    }

    return SharedPointer<UIView>(cell);
}

uint32_t SetTimeMenuTable::getSize() const
{
    return CELL_END_FILLER + 1;
}

uint32_t SetTimeMenuTable::widthAtIndex(uint32_t index) const
{
    (void) index;

    return 128;
}

uint32_t SetTimeMenuTable::heightAtIndex(uint32_t index) const
{
    uint32_t ret = 0;

    if ( (index == CELL_TOP_FILLER) || (index == CELL_END_FILLER) )
    {
        ret = 128;
    }
    else
    {
        ret = 35;
    }

    return ret;
}

SharedPointer<UIView::Action> SetTimeMenuTable::actionAtIndex(uint32_t index)
{
    UIView::Array* table = NULL;

    switch(index)
    {
        // time
        case CELL_HOUR:
                          table = new SetHourMenuTable();
                          break;
        case CELL_MINUTE:
                          table = new SetMinuteMenuTable();
                          break;
        // date
        case CELL_DATE:
                          table = new SetDateMenuTable();
                          break;
        case CELL_MONTH:
                          table = new SetMonthMenuTable();
                          break;
        case CELL_YEAR:
                          table = new SetYearMenuTable();
                          break;
        default:
                          break;
    }

    SharedPointer<UIView::Array> tablePointer(table);

    SharedPointer<UIView::Action> returnObject(new UIView::Action(tablePointer));

    return returnObject;
}

const char* SetTimeMenuTable::getTitle() const
{
    return "menu:set:time";
}

uint32_t SetTimeMenuTable::getFirstIndex() const
{
    return CELL_TOP_FILLER + 1;
}

uint32_t SetTimeMenuTable::getLastIndex() const
{
    return CELL_END_FILLER - 1;
}

uint32_t SetTimeMenuTable::getDefaultIndex() const
{
    return CELL_HOUR;
}

