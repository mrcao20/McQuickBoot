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

#include "../IMcBeanDefinitionReader.h"

MC_FORWARD_DECL_PRIVATE_DATA(McAbstractBeanDefinitionReader)

class MCIOC_EXPORT McAbstractBeanDefinitionReader : public QObject, public IMcBeanDefinitionReader
{
    Q_OBJECT
public:
    explicit McAbstractBeanDefinitionReader(QObject *parent = nullptr);
    ~McAbstractBeanDefinitionReader() override;
    
    IMcBeanDefinitionRegistry *registry() const noexcept;
    
    void readBeanDefinition(IMcBeanDefinitionRegistry *registry) noexcept override;
    
protected:
    virtual void doReadBeanDefinition() noexcept = 0;
    
    Qt::ConnectionType getConnectionType(const QString &typeStr) noexcept;
    Qt::ConnectionType connectionTypeStrToEnum(const QString &typeStr) noexcept;
    
private:
    MC_DECL_PRIVATE(McAbstractBeanDefinitionReader)
};

MC_DECL_POINTER(McAbstractBeanDefinitionReader)
