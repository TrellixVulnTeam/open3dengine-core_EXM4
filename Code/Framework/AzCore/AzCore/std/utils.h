/*
 * Copyright (c) Contributors to the Open 3D Engine Project.
 * For complete copyright and license terms please see the LICENSE at the root of this distribution.
 *
 * SPDX-License-Identifier: Apache-2.0 OR MIT
 *
 */
#ifndef AZSTD_UTILS_H
#define AZSTD_UTILS_H 1

#include <AzCore/std/base.h>
#include <AzCore/std/typetraits/integral_constant.h>
#include <AzCore/std/typetraits/intrinsics.h>
#include <AzCore/std/typetraits/conditional.h>
#include <AzCore/std/typetraits/remove_reference.h>
#include <AzCore/std/typetraits/remove_const.h>
#include <AzCore/std/typetraits/is_enum.h>
#include <AzCore/std/typetraits/underlying_type.h>
#include <AzCore/std/typetraits/add_reference.h>
#include <AzCore/std/reference_wrapper.h>
#include <AzCore/std/typetraits/remove_reference.h>
#include <AzCore/std/typetraits/is_convertible.h>
#include <AzCore/std/typetraits/is_lvalue_reference.h>
#include <AzCore/std/typetraits/void_t.h>
#include <AzCore/std/utility/declval.h>
#include <AzCore/std/utility/move.h>

#include <utility>

namespace AZStd
{
    template <class T>
    struct no_delete
    {
        template <class U>
        void operator()(U*) const {}
    };
    //////////////////////////////////////////////////////////////////////////

    //////////////////////////////////////////////////////////////////////////
    // Swap
    using std::swap;

    template<class ForwardIterator1, class ForwardIterator2>
    AZ_FORCE_INLINE void                    iter_swap(ForwardIterator1 a, ForwardIterator2 b)
    {
        AZStd::swap(*a, *b);
    }

    template<class ForwardIterator1, class ForwardIterator2>
    AZ_FORCE_INLINE ForwardIterator2       swap_ranges(ForwardIterator1 first1, ForwardIterator1 last1, ForwardIterator2 first2)
    {
        for (; first1 != last1; ++first1, ++first2)
        {
            AZStd::iter_swap(first1, first2);
        }

        return first2;
    }
    //////////////////////////////////////////////////////////////////////////

    //////////////////////////////////////////////////////////////////////////

    //////////////////////////////////////////////////////////////////////////
    // Ref wrapper
    template<class T>
    constexpr T* get_pointer(reference_wrapper<T> const& r)
    {
        return &r.get();
    }
    // end reference_wrapper
    //////////////////////////////////////////////////////////////////////////

    //////////////////////////////////////////////////////////////////////////
    // get_pointer
    // get_pointer(p) extracts a ->* capable pointer from p
    template<class T>
    constexpr T* get_pointer(T* p)    { return p; }
    //template<class T> T * get_pointer(std::auto_ptr<T> const& p)
    //{
    //  return p.get();
    //}
    //////////////////////////////////////////////////////////////////////////


    template<class T, bool isEnum = AZStd::is_enum<T>::value>
    struct RemoveEnum
    {
        typedef typename AZStd::underlying_type<T>::type type;
    };

    template<class T>
    struct RemoveEnum<T, false>
    {
        typedef T type;
    };

    template<class T>
    using RemoveEnumT = typename RemoveEnum<T>::type;

    template<class T>
    struct HandleLambdaPointer;

    template<class R, class L, class... Args>
    struct HandleLambdaPointer<R(L::*)(Args...)>
    {
        typedef R(type)(Args...);
    };

    template<class R, class L, class... Args>
    struct HandleLambdaPointer<R(L::*)(Args...) const>
    {
        typedef R(type)(Args...);
    };

    template<class T>
    struct RemoveFunctionConst
    {
        typedef typename HandleLambdaPointer<decltype(&T::operator())>::type type; // lambda matching handling
    };

    template<class R, class... Args>
    struct RemoveFunctionConst<R(Args...)>
    {
        typedef R(type)(Args...);
    };

#if __cpp_noexcept_function_type
    // C++17 makes exception specifications as part of the type in paper P0012R1
    // Therefore noexcept overloads must distinguished from non-noexcept overloads
    template<class R, class... Args>
    struct RemoveFunctionConst<R(Args...) noexcept>
    {
        typedef R(type)(Args...) noexcept;
    };
#endif

    template<class R, class C, class... Args>
    struct RemoveFunctionConst<R(C::*)(Args...)>
    {
        using type = R(C::*)(Args...);
    };

#if __cpp_noexcept_function_type
    // C++17 makes exception specifications as part of the type in paper P0012R1
    // Therefore noexcept overloads must distinguished from non-noexcept overloads
    template<class R, class C, class... Args>
    struct RemoveFunctionConst<R(C::*)(Args...) noexcept>
    {
        using type = R(C::*)(Args...) noexcept;
    };
#endif

    template<class R, class C, class... Args>
    struct RemoveFunctionConst<R(C::*)(Args...) const>
    {
        using type = R(C::*)(Args...);
    };

#if __cpp_noexcept_function_type
    // C++17 makes exception specifications as part of the type in paper P0012R1
    // Therefore noexcept overloads must distinguished from non-noexcept overloads
    template<class R, class C, class... Args>
    struct RemoveFunctionConst<R(C::*)(Args...) const noexcept>
    {
        using type = R(C::*)(Args...) noexcept;
    };
#endif

    using std::in_place_t;
    using std::in_place;

    using std::in_place_type_t;
    using std::in_place_type;

    using std::in_place_index_t;
    using std::in_place_index;

    namespace Internal
    {
        template <typename T>
        struct is_in_place_index
            : false_type
        {};

        template <size_t Index>
        struct is_in_place_index<in_place_index_t<Index>>
            : true_type
        {};
        template <typename T>
        using is_in_place_index_t = typename is_in_place_index<T>::type;
    }
}

#endif // AZSTD_UTILS_H
#pragma once
