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
    @ingroup settings
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
    template <typename Self>
    constexpr auto&& as_values(this Self&& self) {
      using base_t = std::conditional_t<
          std::is_const_v<std::remove_reference_t<Self>>,
          const Values,
          Values 
      >;
      return std::forward_like<Self>(static_cast<base_t&>(self));
    }

    template<typename T> static constexpr bool validate() { return (Flags & (1 << T::enumerated_value)) != 0; }

  public:
    /**
      @brief Constructs an options object from a list of fields.

      The constructor takes a variadic list of fields, which can be either flags or values. The flags are used
      to compute the bitfield for the options, while the values are stored in the kumi::record.

      @param os A variadic list of fields, which can be either flags or values.
    **/
    template<kumi::concepts::field... Os> constexpr options(Os &&... os) : Values(__::as_values(KWK_FWD(os)...)) {}

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
      return os << o.self();
    }

    /**
      @brief Checks if a flag is activated or if a value is present in the options.

      The contains function checks if a flag is activated or if a value is present in the options.

      @tparam  T An instance of the flag or value to check.
      @return `true` if the flag is activated or if the value is present, `false` otherwise.
     **/
    template<kumi::concepts::identifier T> static constexpr bool contains(T const&) noexcept
    {
      if constexpr (__::is_enumerated<T>::value) return validate<T>();
      else return kumi::result::contains_t<Values, T>{};
    }

    /**
      @brief Returns the content of a flag or value in the options.

      Returns if either:
        + A given flag is activated in the options, in which case it returns `true`.
        + A given identifier is present in the options, in which case it returns its associated value.
        + Otherwise, it returns `kumi::none`.

      @param self The options object on which the lookup is performed.
      @param kw The identifier to look up.
      @return The value associated with the identifier if it is present in the options, `true` if it is an activated
              flag, or `kumi::none` otherwise.
    **/
    template<typename Self, kumi::concepts::identifier T> constexpr decltype(auto) operator[](this Self&& self, T const& kw) noexcept
    {
      if constexpr( std::remove_cvref_t<Self>::contains(T{}) )
      {
        if constexpr (__::is_enumerated_v<T>) return kwk::__::maybe{ std::forward_like<Self>(validate<T>()) };
        else return kwk::__::maybe{ std::forward_like<Self>(self.as_values())[kw] };
      }
      else return kwk::__::maybe<void>{};
    }
  };

  /// Deduction guide for options, allows to construct an options object from a list of fields.
  template<kumi::concepts::field... Os>
  requires(kumi::concepts::uniquely_named<Os...>)
  options(Os &&...) -> options<(__::as_flags<Os> | ... | 0), decltype(__::as_values(std::declval<Os>()...))>;

  /**
    @brief Returns the content of a flag or value in the options or a default value.

    Returns if either:
    + A given flag is activated in the options, in which case it returns `true`.
    + A given identifier is present in the options, in which case it returns its associated value.
    + Otherwise, it returns `other`.

    @tparam T     The identifier to look up.
    @param opts   The option pack to retrieve data from.
    @return The value associated with the identifier if it is present in the options, `true` if it is an activated
    flag, or `other` otherwise.
   **/
  template<kumi::concepts::identifier auto T, int Flags, kumi::concepts::product_type Values> 
  [[nodiscard]] constexpr decltype(auto) get(kwk::options<Flags, Values> & opts) noexcept
  {
    return opts[T];
  }

  template<kumi::concepts::identifier auto T, int Flags, kumi::concepts::product_type Values> 
  [[nodiscard]] constexpr decltype(auto) get(kwk::options<Flags, Values> const& opts) noexcept
  {
    return KWK_FWD(opts)[T];
  }
  
  template<kumi::concepts::identifier auto T, int Flags, kumi::concepts::product_type Values> 
  [[nodiscard]] constexpr decltype(auto) get(kwk::options<Flags, Values> && opts) noexcept
  {
    return static_cast<kwk::options<Flags,Values>&&>(opts)[T];
  }

  template<kumi::concepts::identifier auto T, int Flags, kumi::concepts::product_type Values> 
  [[nodiscard]] constexpr decltype(auto) get(kwk::options<Flags, Values> const && opts) noexcept
  {
    return static_cast<kwk::options<Flags,Values> const&&>(opts)[T];
  }

  /// Catch invalid option construction
  template<typename T>
  requires(!kumi::concepts::record_type<T>)
  struct options<-1, T>
  {
    static_assert(kumi::concepts::record_type<T>, "Duplicate setting in options definition");
    template<typename... Os> options(Os &&...) = delete;
  };

  template<kumi::concepts::field... Os>
  requires(!kumi::concepts::uniquely_named<Os...>)
  options(Os &&...) -> options<-1, kumi::tuple<Os...>>;
}
