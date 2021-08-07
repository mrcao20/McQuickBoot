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
#include "McIoc/ApplicationContext/impl/McReadableApplicationContext.h"

#include "McIoc/BeanDefinitionReader/IMcBeanDefinitionReader.h"
#include "McIoc/BeanFactory/impl/McPointerBeanFactory.h"
#include "McIoc/BeanFactory/impl/McSharedBeanFactory.h"
#include "McIoc/PropertyParser/impl/McDefaultPropertyConverter.h"

MC_DECL_PRIVATE_DATA(McReadableApplicationContext)
IMcBeanDefinitionReaderPtr beanDefinitionReader;
MC_DECL_PRIVATE_DATA_END

McReadableApplicationContext::McReadableApplicationContext(QObject *parent)
    : McReadableApplicationContext(IMcConfigurableBeanFactoryPtr(McSharedBeanFactoryPtr::create()),
                                   parent)
{
}

McReadableApplicationContext::McReadableApplicationContext(
        IMcConfigurableBeanFactoryConstPtrRef factory,
        QObject *parent)
    : McAbstractApplicationContext(factory, parent)
{
    MC_NEW_PRIVATE_DATA(McReadableApplicationContext);

    auto related = McPointerBeanFactoryPtr::create();
    auto converter = McDefaultPropertyConverterPtr::create();
    converter->addReferenceResolver(factory.staticCast<McAbstractBeanFactory>().data());
    converter->addReferenceResolver(related.data());
    factory->setPropertyConverter(converter);
    related->setPropertyConverter(converter);
    addRelatedBeanFactory(related);
}

McReadableApplicationContext::McReadableApplicationContext(
        IMcConfigurableBeanFactoryConstPtrRef factory,
        IMcBeanDefinitionReaderConstPtrRef reader,
        QObject *parent)
    : McReadableApplicationContext(factory, parent)
{
   setReader(reader);
}

McReadableApplicationContext::McReadableApplicationContext(IMcBeanDefinitionReaderConstPtrRef reader,
                                                           QObject *parent)
    : McReadableApplicationContext(IMcConfigurableBeanFactoryPtr(McSharedBeanFactoryPtr::create()),
                                   reader,
                                   parent)
{
}

McReadableApplicationContext::~McReadableApplicationContext() 
{
}

void McReadableApplicationContext::readBeans() noexcept 
{
    d->beanDefinitionReader->readBeanDefinition(this);
}

void McReadableApplicationContext::setReader(
        IMcBeanDefinitionReaderConstPtrRef reader) noexcept 
{
    d->beanDefinitionReader = reader;
    
    readBeans();
}
