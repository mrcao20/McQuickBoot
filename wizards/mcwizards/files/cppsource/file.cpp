%{Cpp:LicenseTemplate}\
#include "%{CN}.h"
%{JS: Cpp.openNamespaces('%{CN}')}
MC_STATIC()
MC_REGISTER_BEAN_FACTORY(MC_TYPELIST(%{CN}))
MC_STATIC_END

MC_DECL_PRIVATE_DATA(%{CN})
MC_DECL_PRIVATE_DATA_END

%{CN}::%{CN}(QObject *parent) noexcept
    : QObject(parent)
{
    MC_NEW_PRIVATE_DATA(%{CN});
}

%{CN}::~%{CN}()
{
}
%{JS: Cpp.closeNamespaces('%{CN}')}\
