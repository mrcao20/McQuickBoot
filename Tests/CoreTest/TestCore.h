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

#include <QObject>

#include <McCore/McGlobal.h>

struct GadgetData
{
    Q_GADGET
public:
    MC_POCO_PROPERTY(QString, text, {"gadgetData"})
    MC_POCO_PROPERTY(QSharedPointer<GadgetData>, child)
};

MC_DECL_POINTER(GadgetData)

class ObjectData : public QObject
{
    Q_OBJECT
    MC_JSON_SERIALIZATION()
public:
    MC_POCO_PROPERTY(QString, text, {"objectData"})
    MC_POCO_PROPERTY(QList<GadgetDataPtr>, gadgets)
};

MC_DECL_POINTER(ObjectData)

class TestCore : public QObject
{
    Q_OBJECT
private Q_SLOTS:
    void jsonCase();
    void pathPlaceholderCase();
    void eventDispatcherCase();
    void metaTypeCase();
    void loadPluginCase();
    void loadLibraryCase();
    void loadMemoryLibraryCase();
};
