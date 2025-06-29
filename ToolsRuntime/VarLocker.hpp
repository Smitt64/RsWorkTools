#ifndef VARLOCKER_HPP
#define VARLOCKER_HPP

#include <iostream>

template <typename T>
class VarLocker
{
private:
    T value_;
    bool is_locked_;

public:
    explicit VarLocker(const T& initial_value = T(), bool initially_locked = false)
        : value_(initial_value), is_locked_(initially_locked)
    {}

    void set(const T& new_value)
    {
        if (!is_locked_)
            value_ = new_value;
    }

    const T& get() const
    {
        return value_;
    }

    void lock()
    {
        is_locked_ = true;
    }

    void unlock() {
        is_locked_ = false;
    }

    bool is_locked() const
    {
        return is_locked_;
    }

    VarLocker<T>& operator=(const T& new_value)
    {
        set(new_value);
        return *this;
    }

    operator T() const
    {
        return value_;
    }

    friend std::ostream& operator<<(std::ostream& os, const VarLocker<T>& vl)
    {
        os << vl.value_;
        return os;
    }
};

#endif // VARLOCKER_HPP
