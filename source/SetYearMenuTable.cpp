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

#include "uif-ui-watch-face-wrd/SetYearMenuTable.h"

#include "mbed-time/Calendar.h"

#include "UIFramework/UITextView.h"
#include "UIFramework/UIImageView.h"

#include <cstdio>

typedef enum {
    CELL_TOP_FILLER = 0, // Always first
    // Always last
    CELL_END_FILLER = 26
} entries_t;

const char* yearStringQuadruple[] = {
    "2015",
    "2016",
    "2017",
    "2018",
    "2019",

    "2020",
    "2021",
    "2022",
    "2023",
    "2024",
    "2025",
    "2026",
    "2027",
    "2028",
    "2029",

    "2030",
    "2031",
    "2032",
    "2033",
    "2034",
    "2035",
    "2036",
    "2037",
    "2038"
};

SharedPointer<UIView> SetYearMenuTable::viewAtIndex(uint32_t index) const
{
    UIView* cell = NULL;

    /*  POSIX time starts in 1970.
    */
    if (index < 1970)
    {
        cell = new UIImageView(NULL);
    }
    else
    {
        char yearArray[5];
        snprintf(yearArray, 5, "%4d", (int) index);
        std::string yearString(yearArray);

        cell = new UITextView(yearString, &Font_Menu);
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

uint32_t SetYearMenuTable::getSize() const
{
    return 9999;
}

uint32_t SetYearMenuTable::widthAtIndex(uint32_t index) const
{
    (void) index;

    return 128;
}

uint32_t SetYearMenuTable::heightAtIndex(uint32_t index) const
{
    (void) index;

    return 35;
}

SharedPointer<UIView::Action> SetYearMenuTable::actionAtIndex(uint32_t index)
{
    Calendar::setYear(index);

    return SharedPointer<UIView::Action>(new UIView::Action(UIView::Action::Back));
}

const char* SetYearMenuTable::getTitle() const
{
    return "menu:set:year";
}

uint32_t SetYearMenuTable::getFirstIndex() const
{
    return 1970;
}

uint32_t SetYearMenuTable::getLastIndex() const
{
    return 2020;
}

uint32_t SetYearMenuTable::getDefaultIndex() const
{
    return Calendar::getYear();
}
