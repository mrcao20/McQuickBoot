%{Cpp:LicenseTemplate}\
@if '%{Cpp:PragmaOnce}'
#pragma once
@else
#ifndef %{JS: Cpp.headerGuard('%{FilePath}')}
#define %{JS: Cpp.headerGuard('%{FilePath}')}
@endif

#include <McIoc/McGlobal.h>
%{JS: Cpp.openNamespaces('%{CN}')}
struct %{CN} : public QObject
{
    Q_OBJECT
public:
    Q_INVOKABLE %{CN}() noexcept = default;
    
    MC_POCO_PROPERTY()
};

MC_DECL_METATYPE(%{CN})
%{JS: Cpp.closeNamespaces('%{CN}')}
@if ! '%{Cpp:PragmaOnce}'
#endif // %{JS: Cpp.headerGuard('%{FilePath}')}
@endif
