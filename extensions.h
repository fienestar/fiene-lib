namespace fiene{
#if 0
    #define comprehension(type,expression,statement) \
    ([]{\
        vector<type> v;\
        statement v.push_back(expression);\
        return v;\
    }())
#endif
    template <class T>
    class Chain
    {
        const T& _value;
        bool _rslt;
        Chain(const T& value, bool result):_value(value), _rslt(result) {}
        public:
        Chain(const T& value):_value(value), _rslt(true) {}
        Chain(const Chain<T>& chain):_value(chain._value),_rslt(chain._rslt) {}
        Chain()=delete;
        
        #define make_compare_operator_and_friend(op) \
        template <class S>\
        Chain<T> operator op(const S& rhs) const\
        {\
            return Chain<T>(_value, _rslt && _value op rhs);\
        }\
        template <class D, class S>\
        friend Chain<D> operator op(const S& lhs, Chain<D> c);
        
        make_compare_operator_and_friend(<)
        make_compare_operator_and_friend(<=)
        make_compare_operator_and_friend(>)
        make_compare_operator_and_friend(>=)
        make_compare_operator_and_friend(==)
        make_compare_operator_and_friend(!=)
        
        #undef make_compare_opearator
        
        operator bool() const { return _rslt; }
        
        Chain<T> copied() const { return Chain<T>(T(_value),_rslt); }
    };

    template <class T>
    Chain<T> make_chain(const T& value)
    { return Chain<T>(value); }
    
    #define make_compare_operator(op) \
    template <class T, class S>\
    Chain<T> operator op(const S& lhs, Chain<T> c)\
    {\
         return Chain<T>(c._value, c._rslt && lhs op c._value);\
    }
    
    make_compare_operator(<)
    make_compare_operator(<=)
    make_compare_operator(>)
    make_compare_operator(>=)
    make_compare_operator(==)
    make_compare_operator(!=)
    
    #undef make_compare_opearator
}
