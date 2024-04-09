#pragma once
#include "Object.h"
class ISphere : public Object
{
public:
    template <typename T, typename ... V>
    static void push_vector(std::vector<T>& vector, V ... args)
    {
        push_vector(vector, std::forward<V>(args)...);
    }
    template <typename T>
    static void push_vector(std::vector<T>& vector, T arg)
    {
        vector.push_back(arg);
    }
    template <typename T, typename ... V>
    static void push_vector(std::vector<T>& vector, T arg, V ... args)
    {
        vector.push_back(arg);
        push_vector(vector, args...);
    }
};