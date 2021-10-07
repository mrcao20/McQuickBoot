/*
 * MIT License
 *
 * Copyright (c) 2021 mrcao20
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */
#pragma once

#include <qglobal.h>

//! version format MC_MAJOR.MC_MINOR.MC_PATCH.MC_INTERNAL
//! version when you make big feature changes.
#define MC_MAJOR 1
//! version when you make small feature changes.
#define MC_MINOR 5
//! version when you make backwards-compatible bug fixes.
#define MC_PATCH 3
//! MC_INTERNAL version for perpurse like feature test, bug fix test,development, et
#define MC_INTERNAL 8

#ifdef QT_DEBUG
#define MC_VERSION_STR (QString::number(MC_MAJOR) + "." + QString::number(MC_MINOR) + "." + QString::number(MC_PATCH) + "." + QString::number(MC_INTERNAL))
#define MC_VERSION ((MC_MAJOR<<24)|(MC_MINOR<<16)|(MC_PATCH<<8)|(MC_INTERNAL))
#else
#define MC_VERSION_STR (QString::number(MC_MAJOR) + "." + QString::number(MC_MINOR) + "." + QString::number(MC_PATCH))
#define MC_VERSION ((MC_MAJOR<<16)|(MC_MINOR<<8)|(MC_PATCH))
#endif
