#pragma once

#include "../IMcDeleteThreadWhenQuit.h"

MC_FORWARD_DECL_PRIVATE_DATA(McDefaultDeleteThreadWhenQuit);

/*!
 * \brief The McDefaultDeleteThreadWhenQuit class
 * 
 * 继承该类之后，当对象析构时，会同时析构该对象的生存线程
 */
class MCIOCCONTAINER_EXPORT McDefaultDeleteThreadWhenQuit 
        : public IMcDeleteThreadWhenQuit {
    
    MC_DEFINE_TYPELIST(MC_DECL_TYPELIST(IMcDeleteThreadWhenQuit))
public:
    McDefaultDeleteThreadWhenQuit() noexcept;
    ~McDefaultDeleteThreadWhenQuit() override;
    
    void deleteWhenQuit() noexcept override;
    
private:
    MC_DECL_PRIVATE(McDefaultDeleteThreadWhenQuit)
};

MC_DECL_METATYPE(McDefaultDeleteThreadWhenQuit)
