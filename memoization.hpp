#ifndef FIENE_MEMOIZATION_HPP
#define FIENE_MEMOIZATION_HPP

#include <map>
#include <tuple>

namespace fiene {
    namespace {
        template <class function_t>
        struct memo_map_helper;

        template <class return_t, class ...arg_t>
        struct memo_map_helper<return_t(*)(arg_t...)>:memo_map_helper<return_t(arg_t...)>
        {};

        template <class return_t, class ...arg_t>
        struct memo_map_helper<return_t(arg_t...)>
        {
            using type = std::map<std::tuple<arg_t...>, return_t>;
        };
    }

    template <auto f, class map = typename memo_map_helper<decltype(f)>::type>
    class memo_t
    {
        static map memo_map;
    public:
        template <class ...Args>
        bool has(Args&&... args)
        {
            return memo_map.find(std::make_tuple(std::forward<Args>(args)...)) != memo_map.end();
        }

        template <class ...Args>
        auto operator()(Args&&... args)
        {
            auto arg_tuple = std::make_tuple(std::forward<Args>(args)...);
            auto it = memo_map.find(arg_tuple);
            if (it != memo_map.end())
                return it->second;
            auto r = f(args...);
            memo_map.emplace(std::make_pair(arg_tuple, r));
            return r;
        }
    };

    template <auto f, class map>
    map memo_t<f, map>::memo_map;

    template <auto f>
    auto get_memoized()
    {
        return memo_t<f>();
    }
}

#endif
