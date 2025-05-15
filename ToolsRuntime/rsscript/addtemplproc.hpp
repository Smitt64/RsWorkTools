#ifndef ADDTEMPLPROC_HPP
#define ADDTEMPLPROC_HPP

#include "rslexecutor.h"
#include "rsscript/registerobjlist.hpp"

/*template <typename Ret, typename... Args>
void rslAddTemplateProc(const QString& name, Ret (*func)(Args...))
{
    AddStdProc(name, [func]()
    {
        if (GetFuncParamCount() < sizeof...(Args))
            ThrowParamCountError(sizeof...(Args));

        std::tuple<Args...> params;
        [&]<size_t... I>(std::index_sequence<I...>)
        {
            ((std::get<I>(params) = GetFuncParam(I).template value<std::tuple_element_t<I, std::tuple<Args...>>>()), ...);
        }(std::make_index_sequence<sizeof...(Args)>{});

        if constexpr (std::is_same_v<Ret, void>)
        {
            std::apply(func, params);
            SetReturnVal(QVariant());
        }
        else
        {
            Ret result = std::apply(func, params);
            SetReturnVal(QVariant::fromValue(result));
        }
    });
}*/
/*template <typename Ret, typename... Args>
void rslAddTemplateProc(const QString& name, Ret (*func)(Args...))
{
    // Обертка без сложных шаблонов в лямбде
    RegisterObjList::inst()->AddStdProc(name.toLocal8Bit().data(), [func]() {
        // Проверка количества параметров
        constexpr size_t argCount = sizeof...(Args);
        if (GetFuncParamCount() < argCount) {
            ThrowParamCountError(argCount);
        }

        // Вызов через вспомогательную функцию
        if constexpr (argCount == 0) {
            callFunc(func);
        } else {
            callFuncWithParams(func, std::make_index_sequence<argCount>{});
        }
    });
}

// Вспомогательные функции
template <typename Ret>
void callFunc(Ret (*func)())
{
    if constexpr (std::is_same_v<Ret, void>) {
        func();
        SetReturnVal(QVariant());
    } else {
        SetReturnVal(QVariant::fromValue(func()));
    }
}

template <typename Ret, typename... Args, size_t... I>
void callFuncWithParams(Ret (*func)(Args...), std::index_sequence<I...>)
{
    // Получаем параметры по одному
    auto params = std::make_tuple(GetFuncParam(I).value<std::tuple_element_t<I, std::tuple<Args...>>>()...);

    if constexpr (std::is_same_v<Ret, void>) {
        std::apply(func, params);
        SetReturnVal(QVariant());
    } else {
        SetReturnVal(QVariant::fromValue(std::apply(func, params)));
    }
}*/

// Базовый интерфейс
/*class IFunctionWrapper {
public:
    virtual void call() = 0;
    virtual ~IFunctionWrapper() = default;
};

// Глобальное хранилище
static QMap<std::string, std::unique_ptr<IFunctionWrapper>> g_functionRegistry;

// Шаблонная реализация
template<typename Ret, typename... Args>
class FunctionWrapper : public IFunctionWrapper {
    std::string m_name;
    Ret (*m_func)(Args...);

public:
    FunctionWrapper(std::string name, Ret (*func)(Args...))
        : m_name(std::move(name)), m_func(func) {}

    void call() override {
        if (GetFuncParamCount() < sizeof...(Args)) {
            ThrowParamCountError(sizeof...(Args));
        }
        callImpl(std::make_index_sequence<sizeof...(Args)>{});
    }

    static void staticCall() {
        auto it = g_functionRegistry.find(m_name);
        if (it != g_functionRegistry.end()) {
            it.value()->call();
        }
    }

private:
    template<size_t... I>
    void callImpl(std::index_sequence<I...>) {
        if constexpr (std::is_same_v<Ret, void>) {
            m_func(GetFuncParam(I).value<std::tuple_element_t<I, std::tuple<Args...>>>()...);
            SetReturnVal(QVariant());
        } else {
            SetReturnVal(QVariant::fromValue(
                m_func(GetFuncParam(I).value<std::tuple_element_t<I, std::tuple<Args...>>>()...)
                ));
        }
    }
};

// Шаблонная функция регистрации
template<typename Ret, typename... Args>
void rslAddTemplateProc(const char* name, Ret (*func)(Args...)) {
    auto wrapper = std::make_unique<FunctionWrapper<Ret, Args...>>(name, func);
    RegisterObjList::inst()->AddStdProc(name, &FunctionWrapper<Ret, Args...>::staticCall);
    g_functionRegistry[name] = std::move(wrapper);
}*/

static QMap<std::string, std::unique_ptr<class IFunctionWrapper>> g_functionRegistry;

// Базовый интерфейс
class IFunctionWrapper {
public:
    virtual void call() = 0;
    virtual ~IFunctionWrapper() = default;
};

// Шаблонная реализация
template<typename Ret, typename... Args>
class FunctionWrapper : public IFunctionWrapper {
    Ret (*m_func)(Args...);

public:
    FunctionWrapper(Ret (*func)(Args...)) : m_func(func) {}

    void call() override {
        if (GetFuncParamCount() < sizeof...(Args)) {
            ThrowParamCountError(sizeof...(Args));
        }
        callImpl(std::make_index_sequence<sizeof...(Args)>{});
    }

private:
    template<size_t... I>
    void callImpl(std::index_sequence<I...>) {
        if constexpr (std::is_same_v<Ret, void>) {
            m_func(GetFuncParam(I).value<std::tuple_element_t<I, std::tuple<Args...>>>()...);
            SetReturnVal(QVariant());
        } else {
            SetReturnVal(QVariant::fromValue(
                m_func(GetFuncParam(I).value<std::tuple_element_t<I, std::tuple<Args...>>>()...)
                ));
        }
    }
};

// Шаблон для статических вызывателей
template<const char* name>
struct FunctionCaller {
    static void call() {
        auto it = g_functionRegistry.find(name);
        if (it != g_functionRegistry.end()) {
            it.value()->call();
        }
    }
};

// Шаблонная функция регистрации
template<typename Ret, typename... Args>
void RegisterScriptFunctionImpl(const char* name, Ret (*func)(Args...)) {
    // Сохраняем обертку
    g_functionRegistry[name] = std::make_unique<FunctionWrapper<Ret, Args...>>(func);

    // Регистрируем функцию
    RegisterObjList::inst()->AddStdProc(name, &FunctionCaller<name>::call);
}





#define rslAddTemplateProc(name, func) \
do { \
        static const char _name[] = name; \
        RegisterScriptFunctionImpl(_name, func); \
} while(0)












/*#define REGISTER_FUNCTION(name, func) \
do { \
        static auto wrapper = []() -> IFunctionWrapper* { \
            using FuncType = decltype(func); \
            using Ret = std::invoke_result_t<FuncType, Args...>; \
            return new FunctionWrapper<Ret, Args...>(func); \
    }(); \
        g_functionRegistry[QLatin1String(name)] = wrapper; \
RegisterObjList::inst()->AddStdProc(name.toLocal8Bit.data(), []() { \
                auto it = g_functionRegistry.find(QLatin1String(name)); \
                if (it != g_functionRegistry.end()) { \
                    it.value()->call(); \
            } \
        }); \
} while(0)
template<typename Ret, typename... Args>
void rslAddTemplateProc(const char* name, Ret (*func)(Args...)) {
    // Создаем обертку
    auto wrapper = std::make_unique<FunctionWrapper<Ret, Args...>>(func);

    // Сохраняем в реестре
    g_functionRegistry[name] = std::move(wrapper);

    // Создаем обычную функцию-обертку
    static struct Wrapper {
        static void caller() {
            auto it = g_functionRegistry.find(GetCurrentFuncName());
            if (it != g_functionRegistry.end()) {
                it.value()->call();
            }
        }
    };

    // Регистрируем в скриптовой системе
    RegisterObjList::inst()->AddStdProc(name, &Wrapper::caller);
}*/

#endif // ADDTEMPLPROC_HPP
