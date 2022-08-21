#ifndef FIENE_MEMOIZATION_HPP
#define FIENE_MEMOIZATION_HPP

#include <functional>
#include <map>
#include <tuple>

namespace fiene{
    template <class Function, class ...Args>
    class memoize_wrapper
    {
        Function m_function;
        std::map<std::tuple<Args...>, decltype(m_function(std::declval<Args>()...))> m_memo;
    public:
        memoize_wrapper(Function function)
        : m_function(function), m_memo() {}

        auto operator()(Args&& ...args)
        {
            auto args_tuple = std::make_tuple<Args...>(std::move(args)...);
            auto it = m_memo.find(args_tuple);
            if(it != m_memo.end())
                return it->second;
            else {
                auto result = m_function(std::forward<Args>(args)...);
                m_memo.emplace(std::move(args_tuple), result);
                return result;
            }
        }
    };

    template <class ...Args, class Function>
    auto memoize(Function&& function)
    {
        return memoize_wrapper<Function, Args...>(std::forward<Function>(function));
    }
}

#if false

#include <iostream>

int main()
{
    std::function<int(int)> fibo_raw;
    auto fibo = fiene::memoize<int>(fibo_raw);
    fibo_raw = [&](int n){
        std::cout << n << " ";
        if(n <= 1) return n;
        else return fibo(n-1) + fibo(n-2);
    };

    std::cout << fibo(10) << std::endl; // 10 9 8 7 6 5 4 3 2 1 0 55

    class A{
    public:
        int data;
        A(): data(2) {}
        int get_data() {
            std::cout << "Get Data! ";
            return ++data;
        }
    };

    A a;
    std::function<int()> get_data_raw = std::bind(&A::get_data, &a);
    auto get_data = fiene::memoize<>(get_data_raw);
    std::cout << get_data() << " " << get_data(); // Get Data! 3 3
}

#endif
#endif
