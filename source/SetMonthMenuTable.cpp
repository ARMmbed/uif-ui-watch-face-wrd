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

#include "uif-ui-watch-face-wrd/SetMonthMenuTable.h"

#include "mbed-time/Calendar.h"

#include "UIFramework/UITextView.h"
#include "UIFramework/UIImageView.h"


typedef enum {
    CELL_TOP_FILLER = 0, // Always first
    // Always last
    CELL_END_FILLER = 13
} entries_t;



static const char * month[] = {
    "January",
    "February",
    "March",
    "April",
    "May",
    "June",
    "July",
    "August",
    "September",
    "October",
    "November",
    "December"
};


SharedPointer<UIView> SetMonthMenuTable::viewAtIndex(uint32_t index) const
{
    UIView* cell = NULL;

    if ( (index == CELL_TOP_FILLER) || (index == CELL_END_FILLER) )
    {
        cell = new UIImageView(NULL);
    }
    else if (index < CELL_END_FILLER)
    {
        cell = new UITextView(month[index - 1], &Font_Menu);
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

uint32_t SetMonthMenuTable::getSize() const
{
    return CELL_END_FILLER + 1;
}

uint32_t SetMonthMenuTable::widthAtIndex(uint32_t index) const
{
    (void) index;

    return 128;
}

uint32_t SetMonthMenuTable::heightAtIndex(uint32_t index) const
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

SharedPointer<UIView::Action> SetMonthMenuTable::actionAtIndex(uint32_t index)
{
    Calendar::setMonth(index);

    return SharedPointer<UIView::Action>(new UIView::Action(UIView::Action::Back));
}

const char* SetMonthMenuTable::getTitle() const
{
    return "menu:set:month";
}

uint32_t SetMonthMenuTable::getFirstIndex() const
{
    return CELL_TOP_FILLER + 1;
}

uint32_t SetMonthMenuTable::getLastIndex() const
{
    return CELL_END_FILLER - 1;
}

uint32_t SetMonthMenuTable::getDefaultIndex() const
{
    return Calendar::getMonth();
}

