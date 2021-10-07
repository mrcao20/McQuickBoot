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
#include "Controller2.h"

#include <QDebug>

#include "Domain/RequestVo.h"
#include "Domain/ReturnVo.h"
#include "Service/Service2.h"

MC_STATIC()
MC_REGISTER_BEAN_FACTORY(Controller2)
MC_STATIC_END

void Controller2::method1()
{
    m_service2->method1();
}

int Controller2::method2(const QString &arg)
{
    return m_service2->method2(arg);
}

ReturnVoPtr Controller2::method3(const QList<RequestVoPtr> &vos)
{
    return m_service2->method3(vos);
}

ReturnVoPtr Controller2::method4(const RequestVoPtr &vo)
{
    qDebug() << QStringLiteral(u"method4:") << vo->req;
    auto ret = ReturnVoPtr::create();
    ret->ret = QStringLiteral(u"method4返回");
    return ret;
}
