%{Cpp:LicenseTemplate}\
@if '%{Cpp:PragmaOnce}'
#pragma once
@else
#ifndef %{GUARD}
#define %{GUARD}
@endif

%{JS: QtSupport.qtIncludes([ ( '%{IncludeQObject}' )          ? 'QtCore/%{IncludeQObject}'  : ''],
                           [ ( '%{IncludeQObject}' )          ? 'QtCore/%{IncludeQObject}'  : ''])}\
@if '%{IncludeIocGlobal}'
#include <%{IncludeIocGlobal}>
@endif
%{JS: Cpp.openNamespaces('%{Class}')}
MC_FORWARD_DECL_PRIVATE_DATA(%{CN});

@if '%{Base}'
class %{CN} : public %{Base}
@else
class %{CN}
@endif
{
     Q_OBJECT
	 MC_DEFINE_TYPELIST();
public:
@if '%{Base}' === 'QObject'
    Q_INVOKABLE explicit %{CN}(QObject *parent = nullptr);
    ~%{CN}() override;
@else
    %{CN}();
    ~%{CN}();
@endif

private:
	MC_DECL_PRIVATE(%{CN})
};

MC_DECL_METATYPE(%{CN})
%{JS: Cpp.closeNamespaces('%{Class}')}
@if ! '%{Cpp:PragmaOnce}'
#endif // %{GUARD}
@endif
