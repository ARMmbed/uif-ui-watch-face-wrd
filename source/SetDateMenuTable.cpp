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

#include "uif-ui-watch-face-wrd/SetDateMenuTable.h"

#include "mbed-time/Calendar.h"

#include "UIFramework/UITextView.h"
#include "UIFramework/UIImageView.h"


extern const char* numberStringDouble[];

SharedPointer<UIView> SetDateMenuTable::viewAtIndex(uint32_t index) const
{
    uint8_t days = Calendar::getDaysInMonth();

    UIView* cell = NULL;

    if ( (index == 0) || (index == (days + 1u)) )
    {
        cell = new UIImageView(NULL);
    }
    else if (index <= days)
    {
        cell = new UITextView(numberStringDouble[index], &Font_Menu);
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

uint32_t SetDateMenuTable::getSize() const
{
    uint8_t days = Calendar::getDaysInMonth();

    return days + 2;
}

uint32_t SetDateMenuTable::widthAtIndex(uint32_t index) const
{
    (void) index;

    return 128;
}

uint32_t SetDateMenuTable::heightAtIndex(uint32_t index) const
{
    uint8_t days = Calendar::getDaysInMonth();

    uint32_t ret = 0;

    if ( (index == 0) || (index == (days + 1u)) )
    {
        ret = 128;
    }
    else
    {
        ret = 35;
    }

    return ret;
}

SharedPointer<UIView::Action> SetDateMenuTable::actionAtIndex(uint32_t index)
{
    Calendar::setDate(index);

    return SharedPointer<UIView::Action>(new UIView::Action(UIView::Action::Back));
}

const char* SetDateMenuTable::getTitle() const
{
    return "menu:set:date";
}

uint32_t SetDateMenuTable::getFirstIndex() const
{
    return 1;
}

uint32_t SetDateMenuTable::getLastIndex() const
{
    return Calendar::getDaysInMonth();
}

uint32_t SetDateMenuTable::getDefaultIndex() const
{
    return Calendar::getDate();
}
