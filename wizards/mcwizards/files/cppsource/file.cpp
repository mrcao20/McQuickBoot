%{Cpp:LicenseTemplate}\
#include "%{CN}.h"
%{JS: Cpp.openNamespaces('%{CN}')}
MC_INIT(%{CN})
MC_INIT_END

MC_DECL_PRIVATE_DATA(%{CN})
MC_DECL_PRIVATE_DATA_END

%{CN}::%{CN}(QObject *parent)
    : QObject(parent)
{
    MC_NEW_PRIVATE_DATA(%{CN});
}

%{CN}::~%{CN}()
{
}
%{JS: Cpp.closeNamespaces('%{CN}')}\
