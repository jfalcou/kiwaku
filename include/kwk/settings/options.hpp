//======================================================================================================================
/*
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Project Contributors
  SPDX-License-Identifier: BSL-1.0
*/
//======================================================================================================================
#pragma once

namespace kwk
{
  //====================================================================================================================
  /**
    @ingroup options
    @class options
    @brief A set of options represented as a kumi::record containing a combination of flags and values.

    The options class is a convenient way to represent a set of settings and flags. It is designed to store the values
    of the options and store the activated flags as a bitfield.

    The options class provides a way to check if a flag is activated and to access the values of the options.

    @tparam Flags A compile-time integer representing the activated flags as a bitfield.
    @tparam Values A kumi::record containing the values of the options.
  **/
  //====================================================================================================================
  template<int Flags, kumi::concepts::product_type Values> struct options : Values
  {
  private:
    auto& self() { return static_cast<Values&>(*this); }

    auto const& self() const { return static_cast<Values const&>(*this); }

    template<typename T> static constexpr bool validate() { return (Flags & (1 << T::enumerated_value)) != 0; }

  public:
    /**
      @brief Constructs an options object from a list of fields.

      The constructor takes a variadic list of fields, which can be either flags or values. The flags are used
      to compute the bitfield for the options, while the values are stored in the kumi::record.

      @param os A variadic list of fields, which can be either flags or values.
    **/
    template<kumi::concepts::field... Os> constexpr options(Os const&... os) : Values(__::as_values(os...)) {}

    /**
      @brief Inserts the activated flags and the values of the options in an output stream.

      The operator<< function is used to print the activated flags and the values of the options. It iterates
      over all the available flags and checks if they are activated, if so it prints their name. Then it prints
      the values of the options.

      @param os An output stream to insert the activated flags and the values of the options.
      @param o An options object to print.
    **/
    template<typename C, typename T>
    friend std::basic_ostream<C, T>& operator<<(std::basic_ostream<C, T>& os, options const& o) noexcept
    {
      // kumi::for_each(
      //   [&]<typename M>(M m) {
      //     if constexpr (validate<M>()) os << m.name() << " ";
      //   },
      //   kwk_available_flags{});
      return os << o.self();
    }

    /**
      @brief Checks if a flag is activated or if a value is present in the options.

      The contains function checks if a flag is activated or if a value is present in the options.

      @param  kw An instance of the flag or value to check.
      @return `true` if the flag is activated or if the value is present, `false` otherwise.
    **/
    template<kumi::concepts::identifier T> static constexpr bool contains(T const& kw) noexcept
    {
      if constexpr (__::is_enumerated<T>::value) return validate<T>();
      else return kumi::contains(Values{}, kw);
    }

    /**
      @brief Returns the content of a flag or value in the options.

      Returns if either:
        + A given flag is activated in the options, in which case it returns `true`.
        + A given identifier is present in the options, in which case it returns its associated value.
        + Otherwise, it returns `kumi::none`.

      @param kw The identifier to look up.
      @return The value associated with the identifier if it is present in the options, `true` if it is an activated
              flag, or `kumi::none` otherwise.
    **/
    template<kumi::concepts::identifier T> constexpr auto operator[](T const& kw) const noexcept
    {
      if constexpr (__::is_enumerated<T>::value) return validate<T>();
      else if constexpr (contains(T{})) return self()[kw];
      else return kumi::none;
    }
  };

  /// Deduction guide for options, allows to construct an options object from a list of fields.
  template<kumi::concepts::field... Os>
  requires(kumi::concepts::uniquely_named<Os...>)
  options(Os const&...) -> options<(__::as_flags<Os> | ... | 0), decltype(__::as_values(Os{}...))>;

  /// Catch invalid option construction
  template<typename T>
  requires(!kumi::concepts::record_type<T>)
  struct options<-1, T>
  {
    static_assert(kumi::concepts::record_type<T>, "Duplicate setting in options definition");
    template<typename... Os> options(Os const&...) = delete;
  };

  template<kumi::concepts::field... Os>
  requires(!kumi::concepts::uniquely_named<Os...>)
  options(Os const&...) -> options<-1, kumi::tuple<Os...>>;
}
