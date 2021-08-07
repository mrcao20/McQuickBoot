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
#include "McIoc/ApplicationContext/impl/McSettingApplicationContext.h"

#include <QFile>
#include <QDebug>

#include "McIoc/BeanDefinitionReader/impl/McSettingBeanDefinitionReader.h"

McSettingApplicationContext::McSettingApplicationContext(QObject *parent)
    : McReadableApplicationContext(parent)
{
}

McSettingApplicationContext::McSettingApplicationContext(
        QSettingsConstPtrRef setting, QObject *parent)
    : McSettingApplicationContext(QList<QSettingsPtr>() << setting, parent)
{
}

McSettingApplicationContext::McSettingApplicationContext(
        const QList<QSettingsPtr> &settings, QObject *parent)
    : McSettingApplicationContext(parent)
{
    setSettings(settings);
}

McSettingApplicationContext::McSettingApplicationContext(
        QSettings::Format format, const QString &location, QObject *parent)
    : McSettingApplicationContext(format, QStringList() << location, parent)
{
}

McSettingApplicationContext::McSettingApplicationContext(
        QSettings::Format format, const QStringList &locations, QObject *parent)
    : McSettingApplicationContext(parent)
{
    QList<QSettingsPtr> settings;
    for(auto location : locations) {
        if(!QFile::exists(location)) {
            qCritical() << "file '" << location << "' not exists";
            continue;
        }
        
        QSettings *setting = new QSettings(location, format);
        if (setting->status() != QSettings::Status::NoError) {
            qCritical() << "create yaml setting failure. status:"
                        << setting->status();
            continue;
        }
        settings.append(QSettingsPtr(setting));
    }
    setSettings(settings);
}

McSettingApplicationContext::McSettingApplicationContext(
        IMcBeanDefinitionReaderConstPtrRef reader, QObject *parent)
    : McReadableApplicationContext(reader, parent)
{
}

McSettingApplicationContext::McSettingApplicationContext(
        IMcConfigurableBeanFactoryConstPtrRef factory,
        IMcBeanDefinitionReaderConstPtrRef reader,
        QObject *parent)
    : McReadableApplicationContext(factory, reader, parent)
{
}

McSettingApplicationContext::~McSettingApplicationContext()
{
}

void McSettingApplicationContext::setSetting(QSettingsConstPtrRef setting) noexcept 
{
    setSettings(QList<QSettingsPtr>() << setting);
}

void McSettingApplicationContext::setSettings(const QList<QSettingsPtr> &settings) noexcept 
{
    setReader(McSettingBeanDefinitionReaderPtr::create(settings));
}
