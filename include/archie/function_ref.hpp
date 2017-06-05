#pragma once
#include <utility>
#include <algorithm>
#include <type_traits>
#include <iostream>

namespace archie
{
  constexpr auto const max = [](auto const& t, auto const&... u) {
    auto const* ptr = &t;
    ((ptr = &std::max(*ptr, u)), ...);
    return *ptr;
  };

  template <typename F, typename Return, typename... Args>
  concept bool Callable = requires(F f, Args&&... args)
  {
    {
      f(std::forward<Args>(args)...)
    }
    ->Return;
  };

  template <typename F, typename Return, typename... Args>
  concept bool FunctionPointer = std::is_pointer<F>::value&& Callable<F, Return, Args...>;

  template <typename F, typename Return, typename... Args>
  concept bool ConvertibleToFunctionPointer = (!FunctionPointer<F, Return, Args...>)&&requires(F f)
  {
    {
      +f
    }
    ->FunctionPointer<Return, Args...>;
  };

  template <typename F, typename Return, typename... Args>
  concept bool FunctionObject =
      !FunctionPointer<F, Return, Args...> && !ConvertibleToFunctionPointer<F, Return, Args...> &&
      Callable<F, Return, Args...>;

  template <typename... T>
  using aligned_union = typename std::aligned_storage<max(sizeof(T)...), max(alignof(T)...)>::type;

  template <typename>
  struct function_ref;

  template <typename Return, typename... Args>
  struct function_ref<Return(Args...)> {
    using signature = Return(Args...);
    using callback_t = Return (*)(const void*, Args...);

    function_ref() = delete;
    function_ref(function_ref const&) = default;
    function_ref(function_ref&&) = default;
    function_ref& operator=(function_ref const&) = default;
    function_ref& operator=(function_ref&&) = default;
    function_ref(FunctionPointer<Return, Args...> f) noexcept { assign(f); }
    function_ref(ConvertibleToFunctionPointer<Return, Args...> const& f) noexcept { assign(+f); }
    template <typename R, typename... X>
    function_ref(
        function_ref<R(X...)> const& f) requires Callable<function_ref<R(X...)>, Return, Args...>
        : storage(f.storage), callback(reinterpret_cast<callback_t>(f.callback))
    {
    }
    function_ref(FunctionObject<Return, Args...>& f) noexcept { assign(f); }
    void assign(FunctionObject<Return, Args...>& f) noexcept
    {
      using function_type = std::remove_reference_t<decltype(f)>;
      using ptr_t =
          typename std::conditional<std::is_const<function_type>::value, void const*, void*>::type;
      ::new (&storage) ptr_t(&f);
      callback = +[](const void* memory, Args... args) {
        ptr_t ptr = *static_cast<const ptr_t*>(memory);
        function_type& func = *static_cast<function_type*>(ptr);
        return static_cast<Return>(func(std::forward<Args>(args)...));
      };
    }

    Return operator()(Args... args) const
    {
      return static_cast<Return>(callback(&storage, std::forward<Args>(args)...));
    }

    template <typename R, typename... X>
    void assign(R (*f)(X...)) noexcept
    {
      using pointer_type = R (*)(X...);
      using signature_p = signature*;
      ::new (&storage) signature_p(reinterpret_cast<signature_p>(f));
      callback = +[](const void* memory, Args... args) {
        auto ptr = *static_cast<const pointer_type*>(memory);
        auto func = reinterpret_cast<signature_p>(ptr);
        return static_cast<Return>(func(std::forward<Args>(args)...));
      };
    }

    aligned_union<void*, signature*> storage;
    callback_t callback;
  };

  template <typename R, typename... Args>
  function_ref(R (*)(Args...))->function_ref<R(Args...)>;
}
