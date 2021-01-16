%{Cpp:LicenseTemplate}\
@if '%{Cpp:PragmaOnce}'
#pragma once
@else
#ifndef %{JS: Cpp.headerGuard('%{FilePath}')}
#define %{JS: Cpp.headerGuard('%{FilePath}')}
@endif

#include <McIoc/McGlobal.h>
%{JS: Cpp.openNamespaces('%{CN}')}
struct %{CN}
{
    Q_GADGET
public:
    MC_POCO_PROPERTY()
};

MC_DECL_METATYPE(%{CN})
%{JS: Cpp.closeNamespaces('%{CN}')}
@if ! '%{Cpp:PragmaOnce}'
#endif // %{JS: Cpp.headerGuard('%{FilePath}')}
@endif
