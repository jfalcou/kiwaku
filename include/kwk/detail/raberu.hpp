//==================================================================================================
/*
  RABERU - Fancy Parameters Library
  Copyright : RABERU Project Contributors
  SPDX-License-Identifier: BSL-1.0
*/
//==================================================================================================
#ifndef RABERU_HPP_INCLUDED
#define RABERU_HPP_INCLUDED

#include <array>
#include <cstring>
#include <ostream>
#include <string_view>
#include <type_traits>
#include <utility>

#define RBR_FWD(...) static_cast<decltype(__VA_ARGS__) &&>(__VA_ARGS__)

//==================================================================================================
//! @namespace rbr
//! @brief Main Raberu namespace
//==================================================================================================

//==================================================================================================
//! @defgroup main Main RABERU components
//!
//! @ingroup  main
//! @{
//!   @defgroup kwds Keywords definitions and handling
//!   @brief    Functions and types to handle RABERU keywords
//!
//!   @defgroup stng Settings definitions and handling
//!   @brief    Functions and types to handle RABERU settings
//! @}
//!
//! @defgroup utility   Helper types and function
//! @brief    Tools for interacting with Raberu components
//!
//! @ingroup  utility
//! @{
//!   @defgroup udls   User-defined Literal operators
//!   @brief    UDL operators
//! @}
//==================================================================================================

//==================================================================================================
//! @namespace rbr::concepts
//! @brief Raberu Concepts namespace
//==================================================================================================
namespace rbr::concepts
{
  //================================================================================================
  //! @brief Keyword concept
  //!
  //! A Keyword type is able to be bound to a value as an [Option](@ref rbr::concepts::option)
  //================================================================================================
  template<typename K> concept keyword = requires( K k )
  {
    typename K::tag_type;
    { K::template accept<int>() } -> std::same_as<bool>;
  };

  //================================================================================================
  //! @brief Option concept
  //!
  //! An Option type can be aggregated in a [Settings](@ref rbr::concepts::settings) and be
  //! fetched later
  //================================================================================================
  template<typename O> concept option = requires( O const& o )
  {
    { o(typename std::remove_cvref_t<O>::keyword_type{}) }
    -> std::same_as<typename std::remove_cvref_t<O>::stored_value_type>;
  };

  //================================================================================================
  //! @brief Settings concept
  //!
  //! A Settings is a group of [Options](@ref rbr::concepts::option)
  //================================================================================================
  template<typename S> concept settings = requires( S const& s )
  {
    typename S::rbr_settings;
  };

  //================================================================================================
  //! @brief Exact match concept helper
  //!
  //! rbr::concepts::exactly is to be used to constraint functions template parameter to be an
  //! instantiation of a precise [Keyword](@ref rbr::concepts::keyword)
  //!
  //! @tparam Option  Type to constraint
  //! @tparam Keyword Keyword to match
  //================================================================================================
  template<typename Option, auto Keyword>
  concept exactly = std::same_as< typename Option::keyword_type
                                , std::remove_cvref_t<decltype(Keyword)>
                                >;
}

namespace rbr::detail
{
  // Lightweight container of value in alternatives
  template<concepts::keyword T, typename V> struct type_or_
  {
    V value;

    template<concepts::option... Os>
    constexpr decltype(auto) operator()(Os&&... os) const { return fetch(*this, RBR_FWD(os)...); }
  };

  // Type -> String converter
  template <typename T> constexpr auto type_name() noexcept
  {
    std::string_view name, prefix, suffix;
  #ifdef __clang__
    name = __PRETTY_FUNCTION__;
    prefix = "auto rbr::detail::type_name() [T = ";
    suffix = "]";
  #elif defined(__GNUC__)
    name = __PRETTY_FUNCTION__;
    prefix = "constexpr auto rbr::detail::type_name() [with T = ";
    suffix = "]";
  #elif defined(_MSC_VER)
    name = __FUNCSIG__;
    prefix = "auto __cdecl rbr::detail::type_name<";
    suffix = ">(void)";
  #endif
    name.remove_prefix(prefix.size());
    name.remove_suffix(suffix.size());
    return name;
  }

  // Helpers for working on list of keys as unique lists - needed by merge and some contains_*
  template<typename... Ks> struct keys {};

  template<typename K, typename Ks> struct contains;

  template<typename... Ks, typename K>
  struct  contains<K, keys<Ks...>> : std::bool_constant<(std::same_as<K,Ks> || ...)>
  {};

  template<typename K, typename Ks, bool>  struct append_if_impl;

  template<typename... Ks, typename K> struct append_if_impl<K,keys<Ks...>,true>
  {
    using type = keys<Ks...>;
  };

  template<typename... Ks, typename K> struct append_if_impl<K,keys<Ks...>,false>
  {
    using type = keys<Ks...,K>;
  };

  template<typename K, typename Ks> struct append_if;

  template<typename K, typename Ks>
  struct append_if : append_if_impl<K,Ks, contains<K, Ks>::value>
  {};

  template<typename K1, typename K2> struct uniques;

  template<typename K1s, typename K2, typename... K2s>
  struct  uniques<K1s, keys<K2,K2s...>>
        : uniques< typename append_if<K2,K1s>::type, keys<K2s...> >
  {};

  template<typename K1s> struct  uniques<K1s, keys<>> { using type = K1s; };

  template<typename K1, typename K2> struct contain_all;

  template<typename K1s, typename... K2s>
  struct  contain_all<K1s, keys<K2s...>> : std::bool_constant<(contains<K2s,K1s>::value && ...)>
  {};

  template<typename K1s>  struct  contain_all<K1s   , keys<>>  : std::false_type {};
  template<typename K2s>  struct  contain_all<keys<>, K2s   >  : std::false_type {};
  template<>              struct  contain_all<keys<>,keys<>>   : std::true_type  {};

  template<typename K1, typename K2>
  struct is_equivalent : std::bool_constant<contain_all<K2,K1>::value && contain_all<K2,K1>::value>
  {};
}

namespace rbr
{
  //================================================================================================
  //! @namespace rbr::literals
  //! @brief Raberu literals namespace
  //================================================================================================
  namespace literals
  {
    //==============================================================================================
    //! @ingroup utility
    //! @brief Compile-time static string
    //! @tparam N Size of the embedded string
    //==============================================================================================
    template<std::size_t N> struct str_
    {
      std::array<char,N> data;

      template <std::size_t... Is>
      constexpr str_(const char (&str)[N], std::index_sequence<Is...>) :data{str[Is]...} {}
      constexpr str_(const char (&str)[N]) : str_{str, std::make_index_sequence<N>{}} {}

      static constexpr  auto  size()  { return N; }
      auto value() const { return std::string_view(&data[0],strlen(&data[0])); }
    };

    template<std::size_t N> str_(const char (&str)[N]) -> str_<N>;
  }

  //================================================================================================
  //! @ingroup utility
  //! @brief Compile-time text based ID
  //! @tparam ID Compile-time string for the ID
  //================================================================================================
  template<literals::str_ ID> struct id_
  {
    /// Inserts an rbr::id_ in an output stream
    friend std::ostream& operator<<(std::ostream& os, id_ const&)
    {
      return os << '\'' << ID.value() << '\'';
    }
  };

  namespace literals
  {
    //==============================================================================================
    //! @ingroup udls
    //! @brief Forms an ID constant literal
    //! @return An instance of rbr::id_ for the specified string
    //==============================================================================================
    template<str_ ID> constexpr auto operator""_id() noexcept { return id_<ID>{}; }
  }

  //================================================================================================
  //! @ingroup kwds
  //! @brief Callable object wrapper for functional default value
  //! @tparam Func Callable object to keep
  //================================================================================================
  template<typename Func> struct call
  {
    constexpr call(Func f) : callable(f) {}
    constexpr auto perform() const { return callable(); }
    Func callable;
  };

  //================================================================================================
  //! @ingroup stng
  //! @brief Callable object wrapper for functional default value
  //! @tparam Func Callable object to keep
  //================================================================================================
  template<concepts::keyword Keyword, typename Value> struct option
  {
    using stored_value_type    = Value;
    using keyword_type  = Keyword;

    constexpr stored_value_type operator()(keyword_type const&) const noexcept { return contents; }
    Value contents;
  };

  //================================================================================================
  //! @ingroup kwds
  //! @brief Base class for keyword definition
  //!
  //! rbr::as_keyword provides an CRTP base class for keyword-like types. It is internally used
  //! to provide specific keyword constructors but can be used to define user-defined keyword.
  //!
  //! @tparam Keyword Keyword type being defined
  //================================================================================================
  template<typename Keyword> struct as_keyword
  {
    /// Derived keyword type
    using tag_type  = Keyword;

    /// Keyword comparison
    inline constexpr auto operator<=>(as_keyword const&) const noexcept = default;

    //==============================================================================================
    //! @brief Compile-time validation of value
    //!
    //! When a value is bound to a [Keyword](@ref rbr::concepts::keyword) to form an
    //! [Option](@ref rbr::concepts::option), one can validate this binding by checking arbitrary
    //! properties on the value type. This is done by the accept() member.
    //!
    //! If `Keyword` defines a `check` static template member function, it will be called to provide
    //! the return value of accept(). Otherwise, true is returned, thus automatically validating any
    //! value type.
    //!
    //! @tparam T Type to validate
    //! @return `true` if T is accepted by current keyword, `false` otherwise.
    //!
    //! ## Example:
    //! @snippet doc/accept.cpp Custom Accept
    //==============================================================================================
    template<typename T>
    static constexpr bool accept() requires (!std::same_as<std::remove_cvref_t<T>,as_keyword>)
    {
      if constexpr( requires(Keyword) { Keyword::template check<T>(); } )
        return Keyword::template check<T>();
      else
        return true;
    }

    template<typename T>
    static constexpr bool accept() requires (std::same_as<std::remove_cvref_t<T>,as_keyword>)
    {
      return true;
    }

    //==============================================================================================
    //! @brief Assignment of a value to a keyword
    //!
    //! Bind a value to current [Keyword](@ref rbr::concepts::keyword) and returns an instance of
    //! an [Option](@ref rbr::concepts::option).
    //!
    //! @param v Bound value
    //! @return An rbr::option binding the keyword to `v`.
    //==============================================================================================
    template<typename Type>
    constexpr auto operator=(Type&& v) const noexcept requires( accept<Type>() )
    {
      return option<Keyword,std::remove_cvref_t<Type>>{RBR_FWD(v)};
    }

    //==============================================================================================
    //! @brief Stream insertion function
    //!
    //! Display a textual description of current keyword and bound value over an output stream.
    //!
    //! If `Keyword` defines a `display` member variable, it will be used to perform this display.
    //! Otherwise, the typename name of the keyword and its value will be displayed.
    //!
    //! @param os Output stream
    //! @param v  Value bound to current keyword
    //! @return The up-to-date output stream
    //!
    //! ## Example:
    //! @snippet doc/show.cpp Custom Show
    //==============================================================================================
    template<typename V> std::ostream& show(std::ostream& os, V const& v) const
    {
      if constexpr( requires(Keyword t) { t.display(os,v); } ) return Keyword{}.display(os,v);
      else
      {
        os << '[' << detail::type_name<Keyword>() << ']';
        return os << " : " << v << " (" << detail::type_name<V>() << ')';
      }
    }

    /// Specify a default value for the keyword
    template<typename Type>
    constexpr auto operator|(Type&& v) const noexcept requires( accept<Type>() )
    {
      return detail::type_or_<Keyword,std::remove_cvref_t<Type>>{RBR_FWD(v)};
    }

    /// Specify a Callable object as a default value for the keyword
    template<typename Func> constexpr auto operator|(call<Func>&& v) const noexcept
    {
      return detail::type_or_<Keyword,call<Func>>{RBR_FWD(v)};
    }

    //==============================================================================================
    //! @brief Keyword fetching from options set
    //!
    //! @param o Set of options to inspect
    //! @return f current keyword is present in `o...`, return its bound value. Otherwise,
    //! returns an instance of rbr::unknown_key.
    //!
    //! ## Example:
    //! @include doc/keyword_fetch.cpp
    //==============================================================================================
    template<concepts::option... Os>
    constexpr decltype(auto) operator()(Os&&... o) const { return fetch(Keyword{}, RBR_FWD(o)...); }
  };

  //================================================================================================
  //! @ingroup kwds
  //! @brief Checked keyword
  //!
  //! A Checked keyword is a keyword that verify if a value's type satisfies a predicates before
  //! binding it
  //!
  //! @tparam ID        Unique identifier for the keyword
  //! @tparam Checker   Unary template meta-function acting as predicate
  //================================================================================================
  template<typename ID, template<class> class Checker>
  struct checked_keyword : as_keyword<checked_keyword<ID, Checker>>
  {
    using as_keyword<checked_keyword<ID, Checker>>::operator=;
    template<typename T>  static constexpr bool check() { return Checker<T>::value; }

    template<typename V>
    std::ostream& display(std::ostream& os, V const& v) const
    {
      if constexpr(  requires(ID t) { os << ID{}; } ) os << ID{};
      else os << '[' << detail::type_name<ID>() << ']';

      os << " ::: " << v << " (" << detail::type_name<V>() << ") checked by '";
      return os << detail::type_name<Checker<V>>() << '\'';
    }
  };

  //================================================================================================
  //! @ingroup kwds
  //! @brief Typed keyword
  //!
  //! A Typed keyword is a keyword that verify if a value's type is exactly matching a type.
  //!
  //! @tparam ID    Unique identifier for the keyword
  //! @tparam Type  Type to accept
  //================================================================================================
  template<typename ID, typename Type>
  struct typed_keyword  : as_keyword<typed_keyword<ID, Type>>
  {
    using as_keyword<typed_keyword<ID, Type>>::operator=;
    template<typename T>  static constexpr bool check() { return std::is_same_v<T,Type>; }

    template<typename V>
    std::ostream& display(std::ostream& os, V const& v) const
    {
      if constexpr(  requires(ID t) { os << ID{}; } ) os << ID{};
      else os << '[' << detail::type_name<ID>() << ']';

      return os << " : " << v << " [[" << detail::type_name<V>() << "]]";
    }
  };

  //================================================================================================
  //! @ingroup kwds
  //! @brief Regular keyword
  //!
  //! A Regular keyword is a keyword that accepts any types.
  //!
  //! @tparam ID    Unique identifier for the keyword
  //================================================================================================
  template<typename ID>
  struct any_keyword   : as_keyword<any_keyword<ID>>
  {
    using as_keyword<any_keyword<ID>>::operator=;

    /// ID type associated to the keyword
    using id_type = ID;

    template<typename V>
    std::ostream& display(std::ostream& os, V const& v) const
    {
      if constexpr(requires(ID t) { os << ID{}; })  os << ID{};
      else                                            os << '[' << detail::type_name<ID>() << ']';

      return os << " : " << v << " (" << detail::type_name<V>() << ')';
    }
  };

  //================================================================================================
  //! @ingroup kwds
  //! @brief Flag keyword
  //!
  //! A Flag keyword is a keyword which value is given by its mere presence. It accepts no binding
  //! and return a value convertible to `bool` when set in a rbr::settings.
  //!
  //! By design, a flag is also its own rbr::option.
  //!
  //! @tparam ID    Unique identifier for the keyword
  //================================================================================================
  template<typename ID> struct flag_keyword
  {
    constexpr flag_keyword() {}
    constexpr flag_keyword(ID const&) {}

    /// ID type associated to the keyword
    using id_type = ID;

    template<typename T> static constexpr bool accept()
    {
      return std::is_same_v<std::true_type, T>;
    }

    std::ostream& show(std::ostream& os, bool) const { return os << ID{} << " : set"; }

    using tag_type          = ID;
    using keyword_type      = flag_keyword;
    using stored_value_type = std::true_type;

    template<typename Type>
    constexpr auto operator=(Type&&) const noexcept { return *this; }

    template<typename Type>
    constexpr auto operator|(Type&& v) const noexcept
    {
      return detail::type_or_<flag_keyword,std::remove_cvref_t<Type>>{RBR_FWD(v)};
    }

    template<typename Func> constexpr auto operator|(call<Func>&& v) const noexcept
    {
      return detail::type_or_<flag_keyword,call<Func>>{RBR_FWD(v)};
    }

    constexpr std::true_type operator()(keyword_type const&) const noexcept { return {}; }

    template<typename O0, typename O1, typename... Os>
    constexpr decltype(auto) operator()(O0&&, O1&&, Os&&... ) const
    {
      return    std::same_as<keyword_type, typename std::remove_cvref_t<O0>::keyword_type>
            ||  std::same_as<keyword_type, typename std::remove_cvref_t<O1>::keyword_type>
            || (std::same_as<keyword_type, typename std::remove_cvref_t<Os>::keyword_type> || ...);
    }
  };

  //================================================================================================
  //! @ingroup kwds
  //! @related rbr::flag_keyword
  //! @brief Create a flag keyword for reuse
  //! @param  id  Unique rbr::id_ for the keyword being built
  //! @return An instance of rbr::flag_keyword
  //! ## Example:
  //! @include doc/flag.cpp
  //================================================================================================
  template<typename Tag>
  constexpr flag_keyword<Tag>  flag([[maybe_unused]] Tag id) noexcept { return {}; }

  //================================================================================================
  //! @ingroup kwds
  //! @related rbr::any_keyword
  //! @brief Create a regular keyword for reuse
  //! @param  id  Unique rbr::id_ for the keyword being built
  //! @return An instance of rbr::any_keyword
  //! ## Example:
  //! @include doc/regular.cpp
  //================================================================================================
  template<typename ID>
  constexpr any_keyword<ID> keyword([[maybe_unused]] ID id) noexcept { return {}; }

  //================================================================================================
  //! @ingroup kwds
  //! @related rbr::checked_keyword
  //! @brief Create a checked keyword for reuse
  //! @tparam Checker Unary template meta-function to use for validation
  //! @param  id  Unique rbr::id_ for the keyword being built
  //! @return An instance of rbr::checked_keyword
  //! ## Example:
  //! @include doc/checked.cpp
  //================================================================================================
  template<template<class> class Checker, typename ID>
  constexpr checked_keyword<ID,Checker> keyword([[maybe_unused]] ID id) noexcept { return {}; }

  //================================================================================================
  //! @ingroup kwds
  //! @related rbr::typed_keyword
  //! @brief Create a typed Keyword for reuse
  //! @tparam Type Type accepted by the keyword
  //! @param  id  Unique rbr::id_ for the keyword being built
  //! @return An instance of rbr::checked_keyword
  //! ## Example:
  //! @include doc/checked.cpp
  //================================================================================================
  template<typename Type, typename ID>
  constexpr typed_keyword<ID, Type> keyword([[maybe_unused]] ID id) noexcept { return {}; }

  namespace literals
  {
    //==============================================================================================
    //! @ingroup udls
    //! @related rbr::any_keyword
    //! @brief Forms an instance of rbr::any_keyword from a literal string
    //! @return An instance of rbr::any_keyword using the specified string as ID
    //==============================================================================================
    template<str_ ID>
    constexpr auto operator""_kw() noexcept { return any_keyword<id_<ID>>{}; }

    //==============================================================================================
    //! @ingroup udls
    //! @related rbr::flag_keyword
    //! @brief Forms an instance of rbr::flag_keyword from a literal string
    //! @return An instance of rbr::flag_keyword using the specified string as ID
    //==============================================================================================
    template<str_ ID>
    constexpr auto operator""_fl() noexcept { return flag_keyword<id_<ID>>{}; }
  }

  /// Type indicating that a [Keyword](@ref rbr::concepts::keyword) is not available
  struct unknown_key {};

  // Option calls aggregator
  template<concepts::option... Ts> struct aggregator : Ts...
  {
    constexpr aggregator() noexcept : Ts{}... {}
    constexpr aggregator(Ts const&...t) noexcept requires(sizeof...(Ts)>0) : Ts(t)... {}
    using Ts::operator()...;

    template<concepts::keyword K> constexpr auto operator()(K const &) const noexcept
    {
      return unknown_key {};
    }
  };

  //================================================================================================
  //! @ingroup stng
  //! @brief Defines a group of options for processing
  //!
  //! rbr::settings acts as an aggregation of [Options](@ref rbr::concepts::option) (ie pair of
  //! [Keyword](@ref rbr::concepts::keyword)/value) that provides an user-interface for accessing
  //! or managing said [Options](@ref rbr::concepts::option) and to gather informations.
  //!
  //! @tparam Opts  List of [options](@ref rbr::concepts::option) aggregated
  //================================================================================================
  template<concepts::option... Opts> struct settings
  {
    using rbr_settings = void;
    using base = aggregator<Opts...>;

    /// Default constructor
    constexpr settings()  : content_{} {}

    /// Constructor from a variadic pack of rbr::concepts::option
    constexpr settings(Opts const&... opts) requires(sizeof...(Opts)>0) : content_(opts...) {}

    /// Number of options in current rbr::settings
    static constexpr std::ptrdiff_t size() noexcept { return sizeof...(Opts); }

    //==============================================================================================
    //! @brief Checks if a given rbr::keyword is stored inside rbr::settings
    //! @param kw Keyword to check
    //! @return An instance of `std::true_type` if current setting contains an option based on `kw`.
    //!         Otherwise, return an instance of `std::false_type`.
    //! ## Example:
    //! @include doc/contains.cpp
    //==============================================================================================
    template<concepts::keyword Key>
    static constexpr auto contains([[maybe_unused]] Key const& kw) noexcept
    {
      using found = decltype((std::declval<base>())(Key{}));
      return std::bool_constant<!std::same_as<found, unknown_key> >{};
    }

    //==============================================================================================
    //! @brief Checks if rbr::settings contains at least one of maybe keyword
    //! @param ks Keywords to check
    //! @return An instance of `std::true_type` if current setting contains at least one option
    //!         based on any of the `ks`. Otherwise, return an instance of `std::false_type`.
    //! ## Example:
    //! @include doc/contains_any.cpp
    //==============================================================================================
    template<concepts::keyword... Keys>
    static constexpr auto contains_any(Keys... ks) noexcept { return (contains(ks) || ...); }

    //==============================================================================================
    //! @brief Checks if rbr::settings contains options based only on selected keywords
    //! @param ks Keywords to check
    //! @return An instance of `std::true_type` if current setting contains only options
    //!         based on any of the `ks`. Otherwise, return an instance of `std::false_type`.
    //! ## Example:
    //! @include doc/contains_only.cpp
    //==============================================================================================
    template<concepts::keyword... Keys>
    static constexpr auto contains_only([[maybe_unused]] Keys const&... ks) noexcept
    {
      using current_keys    = detail::keys<typename Opts::keyword_type...>;
      using acceptable_keys = detail::keys<Keys...>;
      using unique_set      = typename detail::uniques<current_keys,acceptable_keys>::type;
      return  detail::is_equivalent<unique_set, acceptable_keys>::value;
    }

    //==============================================================================================
    //! @brief Checks if rbr::settings contains no options based on any of the selected keywords
    //! @param ks Keywords to check
    //! @return An instance of `std::true_type` if current setting contains no options
    //!         based on any of the `ks`. Otherwise, return an instance of `std::false_type`.
    //! ## Example:
    //! @include doc/contains_none.cpp
    //==============================================================================================
    template<concepts::keyword... Keys>
    static constexpr auto contains_none(Keys... ks) noexcept { return !contains_any(ks...); }

    //==============================================================================================
    //! @brief Retrieved a value via a keyword
    //!
    //! Retrieve the value bound to a given keyword `k` inside current rbr::settings instance.
    //! If such a keyword is not present, either an instance of rbr::unknown_key is returned or
    //! a default value or function call will be returned.
    //!
    //! @param k Keywords to check
    //! @return If any, the value bound to `k`.
    //! ## Example:
    //! @include doc/subscript.cpp
    //==============================================================================================
    template<concepts::keyword Key> constexpr auto operator[](Key const& k) const noexcept
    {
      return content_(k);
    }

    //! @overload
    template<typename Keyword>
    constexpr auto operator[](flag_keyword<Keyword> const&) const noexcept
    {
      return contains(flag_keyword<Keyword>{});
    }

    //! @overload
    template<concepts::keyword Key, typename Value>
    constexpr auto operator[](detail::type_or_<Key, Value> const & tgt) const
    {
      if constexpr( contains(Key{}) )                           return (*this)[Key{}];
      else  if constexpr( requires(Value t) { t.perform(); } )  return tgt.value.perform();
      else                                                      return tgt.value;
    }

    //! @related rbr::settings
    //! @brief Output stream insertion
    friend std::ostream& operator<<(std::ostream& os, settings const& s)
    {
      auto show = [&]<typename T, typename V>(T t, V const& v) -> std::ostream&
      {
        return t.show(os,v) << "\n";
      };

      (show(typename Opts::keyword_type{}, s[typename Opts::keyword_type{}]), ...);

      return os;
    }

    base content_;
  };

  /// rbr::settings deduction guide
  template<concepts::option... Opts>
  settings(Opts&&... opts) -> settings<std::remove_cvref_t<Opts>...>;

  //================================================================================================
  //! @ingroup stng
  //! @related rbr::settings
  //! @brief Merge two instances of rbr::settings
  //!
  //! Merge all options of `opts` and `defs`. If an options is present in both arguments, the one
  //! from `opts` is used.
  //!
  //! @param opts rbr::settings to merge
  //! @param defs rbr::settings acting as default value
  //! @return An instance of rbr::settings containing all options from `opts` and any options
  //!         from `defs` not present in `opts`.
  //! ## Example:
  //! @include doc/merge.cpp
  //================================================================================================
  template<concepts::option... K1s, concepts::option... K2s>
  constexpr auto merge(settings<K1s...> const& opts, settings<K2s...> const& defs) noexcept
  {
    auto selector = []<typename K, typename Opts>(K const&, Opts const& o, auto const& d)
                    {
                      constexpr K key;
                      if constexpr( Opts::contains(key) ) return (key = o[key]);
                      else                                return (key = d[key]);
                    };

    auto select = [&]<typename... Ks>(detail::keys<Ks...> const&, auto const& os, auto const& ds)
    {
      return settings(selector(Ks{},os,ds)...);
    };

    return select(typename detail::uniques<detail::keys<typename K1s::keyword_type...>
                                          ,detail::keys<typename K2s::keyword_type...>
                                          >::type{},opts,defs);
  }

  namespace detail
  {
    template<typename K, concepts::keyword... Kept>
    struct filter
    {
      using type = detail::keys<Kept...>;

      template<typename T> constexpr auto operator+(keys<T> const&) const
      {
        using kw_t = typename T::keyword_type;
        if constexpr(!std::same_as<K, typename kw_t::tag_type>)  return filter<K, Kept..., kw_t>{};
        else                                            return *this;
      }
    };

    template< typename K, typename S> struct select_keys;

    template< typename K, concepts::option... Os>
    struct  select_keys<K,rbr::settings<Os...>>
          : decltype((filter<typename K::tag_type>{}  + ... +  detail::keys<Os>{}))
    {
    };
  }

  //================================================================================================
  //! @ingroup stng
  //! @related rbr::settings
  //! @brief Remove an option from a rbr::settings
  //!
  //! Build a rbr::settings containing all options from s except for any option bound to the `k`
  //! rbr::keyword.
  //!
  //! @param k Keyword to remove
  //! @param s Original rbr::settings
  //! @return An instance of rbr::settings containing all options of `s` except those bound to `k`.
  //!
  //! ## Example:
  //! @include doc/drop.cpp
  //================================================================================================
  template<concepts::keyword K, concepts::option... O>
  [[nodiscard]] constexpr auto drop([[maybe_unused]] K const& k, settings<O...> const& s)
  {
    using selected_keys_t = typename detail::select_keys<K,settings<O...>>::type;

    return [&]<typename ... Ks>( detail::keys<Ks...> )
    {
      // Rebuild a new settings by going over the keys that we keep
      return rbr::settings{ (Ks{} = s[Ks{}] )...};
    }(selected_keys_t{});
  }

  //================================================================================================
  //! @ingroup stng
  //! @brief Retrieved a value via a keyword
  //!
  //! Retrieve the value bound to a given keyword `k` inside a variadic list of rbr::options.
  //! If such a keyword is not present, either an instance of rbr::unknown_key is returned or
  //! a default value or function call will be returned.
  //!
  //! @param k Keywords to check
  //! @param os Options to inspect
  //! @return If any, the value bound to `k`.
  //!
  //! ## Helper Types
  //! @code
  //! namespace rbr::result
  //! {
  //!   template<auto Keyword, typename... Sources> struct fetch
  //!
  //!   template<auto Keyword, typename... Sources>
  //!   using fetch_t = typename fetch<Keyword,Sources...>::type;
  //! }
  //! @endcode
  //!
  //! Return the type of a call to rbr::fetch.
  //!
  //! ## Example:
  //! @include doc/fetch.cpp
  //================================================================================================
  template<concepts::keyword K, concepts::option... Os>
  constexpr decltype(auto) fetch(K const& k, Os const&... os)
  {
    auto const opts = settings(os...);
    return opts[k];
  }

  //! @overload
  template<concepts::keyword K, typename V, concepts::option... Os>
  constexpr decltype(auto) fetch(detail::type_or_<K, V> const& k, Os const&... os)
  {
    auto const opts = settings(os...);
    return opts[k];
  }

  //! @overload
  template<typename K, concepts::settings Settings>
  constexpr decltype(auto) fetch(K const& k, Settings const& opts)
  {
    return opts[k];
  }

  //================================================================================================
  //! @namespace rbr::result
  //! @brief Raberu helper traits namespace
  //================================================================================================
  namespace result
  {
    template<auto Keyword, typename... Sources> struct fetch;

    template<auto Keyword, concepts::option... Os>
    struct fetch<Keyword, Os...>
    {
      using type = decltype( rbr::fetch(Keyword, Os{}...) );
    };

    template<auto Keyword, concepts::settings Settings>
    struct fetch<Keyword, Settings>
    {
      using type = decltype( rbr::fetch(Keyword, std::declval<Settings>()) );
    };

    template<auto Keyword, typename... Sources>
    using fetch_t = typename fetch<Keyword,Sources...>::type;
  }

  //================================================================================================
  //! @ingroup utility
  //! @brief Lightweight variadic type list
  //================================================================================================
  template<typename... T> struct types {};

  namespace result
  {
    template<typename Settings, template<typename...> class List = types> struct keywords;
    template<typename Settings, template<typename...> class List = types> struct values;

    template<typename... Opts, template<typename...> class List>
    struct keywords<settings<Opts...>, List>
    {
      using type = List<typename Opts::keyword_type...>;
    };

    template<typename... Opts, template<typename...> class List>
    struct values<settings<Opts...>, List>
    {
      using type = List<typename Opts::stored_value_type...>;
    };

    template<typename Settings, template<typename...> class List = types>
    using keywords_t = typename keywords<Settings,List>::type;

    template<typename Settings, template<typename...> class List = types>
    using values_t = typename values<Settings,List>::type;
  }

  //================================================================================================
  //! @ingroup stng
  //! @brief Retrieved the list of all keywords in a settings
  //!
  //! @tparam List  A n-ary template type to hold the result values
  //! @param s      Settings to inspect
  //! @return An instance of rbr::type containing all the keyword from a rbr::settings.
  //!
  //! ## Helper Types
  //! @code
  //! namespace rbr::result
  //! {
  //!   template<template<typename...> class List, typename Settings> struct keywords;
  //!
  //!   template<template<typename...> class List, typename Settings>
  //!   using keywords_t = typename keywords<List,Settings>::type;
  //! }
  //! @endcode
  //!
  //! Return the type of a call to rbr::keywords.
  //!
  //! ## Example:
  //! @include doc/keywords.cpp
  //================================================================================================
  template<template<typename...> class List, typename... Opts>
  auto keywords([[maybe_unused]]rbr::settings<Opts...> const& s)
  {
    return result::keywords_t<rbr::settings<Opts...>,List>{typename Opts::keyword_type{}...};
  }

  //================================================================================================
  //! @ingroup stng
  //! @brief Retrieved the list of all value stored in a settings
  //!
  //! @tparam List  A n-ary template type to hold the result
  //! @param s      Settings to inspect
  //! @return an instance of rbr::type containing all the values from a rbr::settings.
  //!
  //! ## Helper Types
  //! @code
  //! namespace rbr::result
  //! {
  //!   template<template<typename...> class List, typename Settings> struct values;
  //!
  //!   template<template<typename...> class List, typename Settings>
  //!   using values_t = typename values<List,Settings>::type;
  //! }
  //! @endcode
  //!
  //! Return the type of a call to rbr::values.
  //!
  //! ## Example:
  //! @include doc/values.cpp
  //================================================================================================
  template<template<typename...> class List, typename... Opts>
  auto values(rbr::settings<Opts...> const& o)
  {
    return result::values_t<rbr::settings<Opts...>,List>{ o[typename Opts::keyword_type{}]... };
  }

  //================================================================================================
  //! @ingroup stng
  //! @brief Checks the equivalence of two rbr::settings
  //!
  //! Two rbr::settings are equivalent if they contain the same exact set of keywords irregardless
  //! of their values or value types.
  //!
  //! @tparam S1, S2 rbr::settings to compare
  //!
  //! ## Helper Value
  //! @code
  //! namespace rbr
  //! {
  //!   template<concepts::settings S1, concepts::settings S2>
  //!   inline constexpr bool is_equivalent_v = is_equivalent<S1,S2>::value;
  //! }
  //! @endcode
  //!
  //! Contains the result of a call to rbr::is_equivalent.
  //!
  //! ## Example:
  //! @include doc/is_equivalent.cpp
  //================================================================================================
  template<concepts::settings S1, concepts::settings S2>
  struct  is_equivalent
        : std::bool_constant<   detail::is_equivalent < result::keywords_t<S1,detail::keys>
                                                      , result::keywords_t<S2,detail::keys>
                                                      >::value
                            &&  detail::is_equivalent < result::keywords_t<S2,detail::keys>
                                                      , result::keywords_t<S1,detail::keys>
                                                      >::value
                            >
  {};

  template<concepts::settings S1, concepts::settings S2>
  inline constexpr bool is_equivalent_v = is_equivalent<S1,S2>::value;
}

#undef RBR_FWD
#endif
