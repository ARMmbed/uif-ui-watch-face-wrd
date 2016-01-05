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

#include "uif-ui-watch-face-wrd/SetMinuteMenuTable.h"

#include "mbed-time/Calendar.h"

#include "UIFramework/UITextView.h"
#include "UIFramework/UIImageView.h"


typedef enum {
  CELL_TOP_FILLER = 0, // Always first
  // Always last
  CELL_END_FILLER = 61
} entries_t;


extern const char* numberStringDouble[];

SharedPointer<UIView> SetMinuteMenuTable::viewAtIndex(uint32_t index) const
{
    UIView* cell = NULL;

    if ( (index == CELL_TOP_FILLER) || (index == CELL_END_FILLER) )
    {
        cell = new UIImageView(NULL);
    }
    else if (index < CELL_END_FILLER)
    {
        cell = new UITextView(numberStringDouble[index - 1], &Font_Menu);
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

uint32_t SetMinuteMenuTable::getSize() const
{
    return CELL_END_FILLER + 1;
}

uint32_t SetMinuteMenuTable::widthAtIndex(uint32_t index) const
{
    (void) index;

    return 128;
}

uint32_t SetMinuteMenuTable::heightAtIndex(uint32_t index) const
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

SharedPointer<UIView::Action> SetMinuteMenuTable::actionAtIndex(uint32_t index)
{
    /* setting the minute mark triggers a reset of the second counter */
    Calendar::setSecond(0);
    Calendar::setMinute(index - 1);

    return SharedPointer<UIView::Action>(new UIView::Action(UIView::Action::Back));
}

const char* SetMinuteMenuTable::getTitle() const
{
    return "menu:set:minute";
}

uint32_t SetMinuteMenuTable::getFirstIndex() const
{
    return CELL_TOP_FILLER + 1;
}

uint32_t SetMinuteMenuTable::getLastIndex() const
{
    return CELL_END_FILLER - 1;
}

uint32_t SetMinuteMenuTable::getDefaultIndex() const
{
    return Calendar::getMinute() + 1;
}
