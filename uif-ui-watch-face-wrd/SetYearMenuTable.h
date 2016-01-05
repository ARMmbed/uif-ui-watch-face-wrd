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

#ifndef __UIF_UI_SETYEARMENUTABLE_H__
#define __UIF_UI_SETYEARMENUTABLE_H__

#include "UIFramework/UIView.h"

class SetYearMenuTable : public UIView::Array
{
public:
    /**
     * @brief Get number of elements in the menu.
     *
     * @return Number of elements
     */
    virtual uint32_t getSize(void) const;

    /**
     * @brief Get UIView object at the given index.
     *
     * @param index Cell to retrieve. Index must be between getFirstIndex
     *              and getLastIndex.
     * @return UIView-object wrapped inside a SharedPointer
     */
    virtual SharedPointer<UIView> viewAtIndex(uint32_t index) const;

    /**
     * @brief Get pixel height of the cell at the given index.
     *
     * @param index Cell to get height of. Index must be between getFirstIndex
     *              and getLastIndex.
     * @return Height in number of pixels.
     */
    virtual uint32_t heightAtIndex(uint32_t index) const;

    /**
     * @brief Get pixel width of the cell at the given index.
     *
     * @param index Cell to get width of. Index must be between getFirstIndex
     *              and getLastIndex.
     * @return Width in number of pixels.
     */
    virtual uint32_t widthAtIndex(uint32_t index) const;

    /**
     * @brief Get the table's title.
     * @return const char* to '/0'-terminated string. Can be NULL.
     */
    virtual const char* getTitle(void) const;

    /**
     * @brief Get the lowest valid index for this table.
     * @return Lowest valid index.
     */
    virtual uint32_t getFirstIndex(void) const;

    /**
     * @brief Get the highest valid index for this table.
     * @return Highest valid index.
     */
    virtual uint32_t getLastIndex(void) const;

    /**
     * @brief Get the default index for this table.
     * @return Default index.
     */
    virtual uint32_t getDefaultIndex(void) const;

    /**
     * @brief Invoke action associated with the cell at the given index.
     *
     * @param index Cell to invoke action on.
     * @return UIView::Action-object wrapped in a SharedPointer. This object
     *         contains the result of the invoked action.
     */
    virtual SharedPointer<UIView::Action> actionAtIndex(uint32_t index);
};

#endif // __UIF_UI_SETYEARMENUTABLE_H__
