#pragma once

class IMcNonCopyable
{
public:
    IMcNonCopyable() noexcept = default;
    virtual ~IMcNonCopyable() = default;
    
private:
    IMcNonCopyable(const IMcNonCopyable &) = delete;
    IMcNonCopyable &operator=(const IMcNonCopyable &) = delete;
    IMcNonCopyable(IMcNonCopyable &&) = delete;
    IMcNonCopyable &operator=(IMcNonCopyable &&) = delete;
};
