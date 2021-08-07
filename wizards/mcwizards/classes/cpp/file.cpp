%{Cpp:LicenseTemplate}\
@if "%{HdrDirPath}" == "%{SrcDirPath}"
#include "%{JS: Util.relativeFilePath('%{Path}/%{HdrFileName}', '%{Path}' + '/' + Util.path('%{SrcFileName}'))}"
@else
#include "%{JS: value('HdrPath').substr(value('IncludeBasePath').length)}"
@endif
%{JS: Cpp.openNamespaces('%{Class}')}
MC_AUTO_INIT(%{CN})
MC_INIT_END

MC_DECL_PRIVATE_DATA(%{CN})
MC_DECL_PRIVATE_DATA_END

@if '%{Base}' === 'QObject'
%{CN}::%{CN}(QObject *parent) noexcept
    : QObject(parent)
@else
%{CN}::%{CN}() noexcept
@endif
{
    MC_NEW_PRIVATE_DATA(%{CN});
}

%{CN}::~%{CN}()
{
}
%{JS: Cpp.closeNamespaces('%{Class}')}\
