%{Cpp:LicenseTemplate}\
@if '%{Cpp:PragmaOnce}'
#pragma once
@else
#ifndef %{GLOBAL_GUARD}
#define %{GLOBAL_GUARD}
@endif

#include <McBoot/McBootGlobal.h>

@if ! '%{Cpp:PragmaOnce}'

#endif // %{GLOBAL_GUARD}
@endif
