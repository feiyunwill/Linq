/*=============================================================================
    Copyright (c) 2012 Paul Fultz II
    function_object.h
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef LINQ_GUARD_DETAIL_FUNCTION_OBJECT_H
#define LINQ_GUARD_DETAIL_FUNCTION_OBJECT_H

#include <linq/utility.h>
#include <boost/optional.hpp>
#include <utility>

namespace linq { 

// Lambdas aren't very nice, so we use this wrapper to make them play nicer. This
// will make the function_object default constructible, even if it doesn't have a
// default constructor. This helpful since these function objects are being used
// inside of iterators.
template<class Fun>
struct function_object
{
    boost::optional<Fun> f;

    function_object()
    {}
    function_object(Fun f): f(f)
    {}

    function_object(const function_object & rhs) : f(rhs.f)
    {}

    // Assignment operator is just a copy construction, which does not provide
    // the strong exception guarentee.
    function_object& operator=(const function_object& rhs)
    {
        if (this != &rhs)
        {
            this->~function_object();
            new (this) function_object(rhs);
        }
        return *this;
    }

    template<class F>
    struct result
    {};

    template<class F, class T>
    struct result<F(T)>
    {
        typedef decltype(linq::declval<Fun>()(linq::declval<T>())) type;
    };

    template<class T>
    auto operator()(T && x) const LINQ_RETURNS((*f)(std::forward<T>(x)));

    template<class T>
    auto operator()(T && x) LINQ_RETURNS((*f)(std::forward<T>(x)));
};

template<class F>
function_object<F> make_function_object(F f)
{
    return function_object<F>(f);
}

}

#endif