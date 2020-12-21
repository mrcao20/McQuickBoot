#pragma once

#include <functional>

class McScopedFunction 
{
public:
    McScopedFunction(const std::function<void()> &func) : m_func(func) {}

    McScopedFunction(std::function<void()> &&func) { m_func.swap(func); }

    ~McScopedFunction()
    {
        if(!m_func) {
            return;
        }
        m_func();
    }
    
    std::function<void()> take() noexcept 
    {
        auto func = m_func;
        m_func = nullptr;
        return func;
    }

    void clear() noexcept { take(); }

private:
    std::function<void()> m_func;
};
