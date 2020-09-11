#pragma once

#ifndef MC_NO_IOC
#include <McIoc/Thread/IMcDeleteThreadWhenQuit.h>
#endif

#include "../McLogGlobal.h"

MC_FORWARD_DECL_PRIVATE_DATA(McDeleteThreadWhenQuit);

class McDeleteThreadWhenQuit
#ifndef MC_NO_IOC
    : public IMcDeleteThreadWhenQuit 
{
    MC_DEFINE_TYPELIST(MC_DECL_TYPELIST(IMcDeleteThreadWhenQuit))
#else
{
    MC_DEFINE_TYPELIST()
#endif
public:
    McDeleteThreadWhenQuit() noexcept;
    virtual ~McDeleteThreadWhenQuit() noexcept;
    
    virtual void deleteWhenQuit() noexcept;
private:
    MC_DECL_PRIVATE(McDeleteThreadWhenQuit)
};

MC_DECL_METATYPE(McDeleteThreadWhenQuit)
