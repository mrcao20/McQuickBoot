#pragma once

#include <McIoc/McGlobal.h>

#include "McBootMacroGlobal.h"

#include "McAbstractQuickBoot.h"

//! 此宏所对应的对象将在Application析构时销毁，所以一旦Application开始析构，就再也不要调用此宏
#define $ (McAbstractQuickBoot::instance()->requestor())
