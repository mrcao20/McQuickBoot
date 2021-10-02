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

#include "McAbstractXmlPathConfig.h"

MC_FORWARD_DECL_CLASS(IMcWidgetApplicationContext)

MC_FORWARD_DECL_PRIVATE_DATA(McWidgetConfig);

class McWidgetConfig : public McAbstractXmlPathConfig
{
    Q_OBJECT
    MC_DECL_SUPER(McAbstractXmlPathConfig)
    MC_COMPONENT("widgetConfig")
    MC_CONFIGURATION_PROPERTIES("boot.application.widget")
    Q_PROPERTY(QString mainWindowName READ mainWindowName WRITE setMainWindowName)
    Q_PROPERTY(int destroyPriority READ destroyPriority WRITE setDestroyPriority)
public:
    explicit McWidgetConfig(QObject *parent = nullptr) noexcept;
    ~McWidgetConfig() override;

    QString mainWindowName() const noexcept;
    void setMainWindowName(const QString &val) noexcept;
    int destroyPriority() const noexcept;
    void setDestroyPriority(int val) noexcept;

    IMcWidgetApplicationContextPtr appCtx() const noexcept;

protected:
    void doFinished() noexcept override;

private:
    MC_DECL_PRIVATE(McWidgetConfig)
};

MC_DECL_METATYPE(McWidgetConfig)
