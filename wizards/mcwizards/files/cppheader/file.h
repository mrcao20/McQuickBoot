%{Cpp:LicenseTemplate}\
@if '%{Cpp:PragmaOnce}'
#pragma once
@else
#ifndef %{JS: Cpp.headerGuard('%{FilePath}')}
#define %{JS: Cpp.headerGuard('%{FilePath}')}
@endif

#include <McIoc/McGlobal.h>
%{JS: Cpp.openNamespaces('%{CN}')}
MC_FORWARD_DECL_PRIVATE_DATA(%{CN});

class %{CN} : public QObject
{
    Q_OBJECT
    MC_DECL_INIT(%{CN})
    MC_DEFINE_TYPELIST();
public:
    Q_INVOKABLE explicit %{CN}(QObject *parent = nullptr);
    ~%{CN}() override;
	
private:
    MC_DECL_PRIVATE(%{CN})
};

MC_DECL_METATYPE(%{CN})
%{JS: Cpp.closeNamespaces('%{CN}')}
@if ! '%{Cpp:PragmaOnce}'
#endif // %{JS: Cpp.headerGuard('%{FilePath}')}
@endif
