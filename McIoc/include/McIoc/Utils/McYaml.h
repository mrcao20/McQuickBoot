/* ***** BEGIN LICENSE BLOCK *****
 * Version: MPL 2.0
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * ***** END LICENSE BLOCK ***** */

/*
 * Copyright (c) 2014, Filip Brcic <brcha@gna.org>. All rights reserved.
 *
 * This file is part of lusim
 */

#pragma once

#include <QtCore/QSettings>

#include "McIoc/McGlobal.h"

class MCIOC_EXPORT McYaml
{
    MC_DECL_INIT(McYaml)
public:
    static inline QSettings::Format format() noexcept { return m_format; }

private:
    static QSettings::Format m_format;
};
