#pragma once

#include "OwnPtr.h"
#include "StdLibExtras.h"
#include "Types.h"

namespace ASL {

template<typename>
class Function;

template<typename Out, typename... In>
class Function<Out(In...)> {
public:
    Function() = default;
    Function(std::nullptr_t) {}

    template<typename CallableType, class = typename EnableIf<!(IsPointer<CallableType>::value && IsFunction<typename RemovePointer<CallableType>::Type>::value) && IsRvalueReference<CallableType&&>::value>::Type>
    Function(CallableType&& callable)
        : m_callable_wrapper(create_own<CallableWrapper<CallableType>>(move(callable)))
    {
    }

    template<typename FunctionType, class = typename EnableIf<IsPointer<FunctionType>::value && IsFunction<typename RemovePointer<FunctionType>::Type>::value>::Type>
    Function(FunctionType f)
        : m_callable_wrapper(create_own<CallableWrapper<FunctionType>>(move(f)))
    {
    }

    Out operator()(In... in) const
    {
        ASSERT(m_callable_wrapper);
        return m_callable_wrapper->call(forward<In>(in)...);
    }

    explicit operator bool() const { return !!m_callable_wrapper; }

    template<typename CallableType, class = typename EnableIf<!(IsPointer<CallableType>::value && IsFunction<typename RemovePointer<CallableType>::Type>::value) && IsRvalueReference<CallableType&&>::value>::Type>
    Function& operator=(CallableType&& callable)
    {
        m_callable_wrapper = create_own<CallableWrapper<CallableType>>(move(callable));
        return *this;
    }

    template<typename FunctionType, class = typename EnableIf<IsPointer<FunctionType>::value && IsFunction<typename RemovePointer<FunctionType>::Type>::value>::Type>
    Function& operator=(FunctionType f)
    {
        m_callable_wrapper = create_own<CallableWrapper<FunctionType>>(move(f));
        return *this;
    }

    Function& operator=(std::nullptr_t)
    {
        m_callable_wrapper = nullptr;
        return *this;
    }

private:
    class CallableWrapperBase {
    public:
        virtual ~CallableWrapperBase() {}
        virtual Out call(In...) const = 0;
    };

    template<typename CallableType>
    class CallableWrapper final : public CallableWrapperBase {
    public:
        explicit CallableWrapper(CallableType&& callable)
            : m_callable(move(callable))
        {
        }

        CallableWrapper(const CallableWrapper&) = delete;
        CallableWrapper& operator=(const CallableWrapper&) = delete;

        Out call(In... in) const final override { return m_callable(forward<In>(in)...); }

    private:
        CallableType m_callable;
    };

    OwnPtr<CallableWrapperBase> m_callable_wrapper;
};

} // namespace ASL

using ASL::Function;
