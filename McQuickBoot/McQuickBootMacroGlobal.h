/*
 * Copyright (c) 2021 mrcao20/mrcao20@163.com
 * McQuickBoot is licensed under Mulan PSL v2.
 * You can use this software according to the terms and conditions of the Mulan PSL v2.
 * You may obtain a copy of Mulan PSL v2 at:
 *          http://license.coscl.org.cn/MulanPSL2
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND,
 * EITHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT,
 * MERCHANTABILITY OR FIT FOR A PARTICULAR PURPOSE.
 * See the Mulan PSL v2 for more details.
 */
#pragma once

#include <McCore/McMacroGlobal.h>

#ifndef MC_EXPORT_DISABLE
# if defined(MC_QUICKBOOT_LIBRARY)
#  define MC_QUICKBOOT_EXPORT Q_DECL_EXPORT
# else
#  define MC_QUICKBOOT_EXPORT Q_DECL_IMPORT
# endif
#else
# define MC_QUICKBOOT_EXPORT
#endif
