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

#include "mbed-drivers/mbed.h"

#include "uif-ui-watch-face-wrd/WatchFaceUI.h"

#include "uiframework/UIFramework.h"
#include "uif-matrixlcd/MatrixLCD.h"

static SPI lcdspi(LCD_SPI_MOSI, NC, LCD_SPI_CLK);
static uif::MatrixLCD lcd(lcdspi, LCD_CS, LCD_DISP, LCD_EXT_COM_IN);
static SharedPointer<UIFramework> uiFramework;

void app_start(int, char *[])
{
    SharedPointer<UIView> view(new WatchFaceUI());
    view->setWidth(128);
    view->setHeight(128);

    // UI framework
    uiFramework = SharedPointer<UIFramework>(new UIFramework(lcd, view));
}
