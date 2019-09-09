#ifndef FUNCTIONWRAPPER_HPP
#define FUNCTIONWRAPPER_HPP
#include <memory>
class FunctionWrapper{
    struct impl_base {
        virtual void call() = 0;
        virtual ~impl_base() {}
    };
    std::unique_ptr<impl_base> impl;
    template<typename Function>
    struct impl_type: impl_base
    {
        Function func;
        impl_type(Function && func_): func(std::move(func_)){}
        void call () { func(); };
    };
public:
    template<typename Function>
    FunctionWrapper(Function && f): impl(new impl_type<Function>(std::move(f))){}
    void operator()() {impl->call();}
    FunctionWrapper() = default;
    FunctionWrapper(FunctionWrapper&& other): impl(std::move(other.impl)){}
    FunctionWrapper& operator=(FunctionWrapper&& other)
    {
        impl = std::move(other.impl);
        return *this;
    }
    FunctionWrapper(const FunctionWrapper&) = delete;
    FunctionWrapper(FunctionWrapper&) = delete;
    FunctionWrapper& operator=(const FunctionWrapper&) = delete;
};
#endif