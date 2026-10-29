//======================================================================================================================
/*
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Project Contributors
  SPDX-License-Identifier: BSL-1.0
*/
//======================================================================================================================
#pragma once

namespace kwk::__
{
  /*
    A flag acts as both an identifier and a boolean value. It is identified by its name and a type.
    It can be used in a kumi::record to represent an option that can be either enabled or disabled.
  */
  template<kumi::str Name, typename T> struct flag : kumi::identifier<T>
  {
    using type            = T;
    using identifier_type = T;

    template<typename Self> constexpr bool operator()(this Self&&, identifier_type) { return true; }

    static constexpr auto                  name() { return Name; }
  };

  /*
    An enumerated is a flag that has an associated integer index. It can be used in a kumi::record
    to represent an option that can be either enabled or disabled and which has a specific position
    in a bitfield. The enumerated_value is used to compute the bitmask for the flag.
  */
  template<kumi::str Name, typename T, int ID> struct enumerated : flag<Name, T>
  {
    static constexpr int enumerated_value = ID;
  };

  /*
    Checks if a type is an enumerated by checking if it has the enumerated_value member.
  */
  template<typename T> inline constexpr bool is_enumerated_v = requires { T::enumerated_value; };

  template<typename T> using is_enumerated = std::bool_constant<is_enumerated_v<T>>;

  /*
    Converts a field to its corresponding flag value. If the field is an enumerated, it returns the
    bitmask for the flag, otherwise it returns 0.
  */
  template<kumi::concepts::field O>
  inline constexpr int as_flags = []()
  {
    if constexpr(is_enumerated_v<O>) return (1 << O::type::enumerated_value);
    else return 0;
  }();

  /*
    Constructs a kumi::record containing only the values of the fields that are not enumerated. This
    can be useful to separate the flags from the other fields in a record, for example to pass only
    the non-flag fields to a function that does not need to know about the flags.
  */
  template<kumi::concepts::field... Os> constexpr auto as_values(Os&&... os)
  {
    return kumi::filter_not<is_enumerated>(kumi::record {KWK_FWD(os)...});
  }

  template<typename T> struct maybe_binder_t
  {
    T value;
  };

  template<> struct maybe_binder_t<void>
  {
  };

  template<bool HasValue, typename T> struct maybe_properties
  {
    using element_type = void;
  };
  template<typename T> struct maybe_properties<true, T>
  {
    using element_type = typename std::remove_cvref_t<T>::element_type;
  };

  // This is the Maybe monad, a kwk::option is simply a record that has been lifted into the maybe
  // monad
  template<typename T>
  struct maybe
      : maybe_binder_t<T>
      , maybe_properties<!std::is_void_v<T>, T>
  {
    static constexpr auto                                        has_value = !std::is_void_v<T>;

    template<typename Self, typename U> constexpr decltype(auto) value_or(this Self&& self, U&& u)
    {
      if constexpr(has_value) return std::forward_like<Self>(self.value);
      else return KWK_FWD(u);
    }

    // Lazy fallback : return the held value when present, otherwise call `f()` for the fallback.
    // (Counterpart of `value_or` for callable fallbacks; the shape / kind / coordinate pipelines
    // pick the first present backend with `value_or`, the base/accessor folds with `or_else`.)
    template<typename Self, typename F> constexpr decltype(auto) or_else(this Self&& self, F&& f)
    {
      if constexpr(has_value) return std::forward_like<Self>(self.value);
      else return KWK_FWD(f)();
    }

    // Map : apply `f` to the held value and lift the (raw) result back into a `maybe`. Absent stays
    // `maybe<void>`. Keywords' `and_then` historically doubled as the map; the two are now split so
    // a map verb is idiomatic and a bind returns the function's own maybe untouched.
    template<typename Self, typename F> constexpr auto transform(this Self&& self, F&& f)
    {
      if constexpr(has_value)
        return kwk::__::maybe {KWK_FWD(f)(std::forward_like<Self>(self.value))};
      else return kwk::__::maybe<void> {};
    }

    // Bind : apply `f` to the held value; `f` is expected to return a `maybe` (or a tuple, folded
    // downstream), which is returned as-is. Absent stays `maybe<void>`.
    template<typename Self, typename F> constexpr auto and_then(this Self&& self, F&& f)
    {
      if constexpr(has_value) return KWK_FWD(f)(std::forward_like<Self>(self.value));
      else return kwk::__::maybe<void> {};
    }

    // Presence filter : project a present value into a concatenable, homogeneous 0-or-1 element
    // tuple. Used by the builder to fold the (optional) accessor stages onto a fibre chain with a
    // plain `kumi::cat` — an absent stage simply contributes the empty tuple.
    template<typename Self> constexpr auto as_tuple(this Self&& self)
    {
      if constexpr(has_value) return kumi::tuple {std::forward_like<Self>(self.value)};
      else return kumi::tuple<> {};
    }

    constexpr operator bool() noexcept { return has_value; }

    // Compares a value-carrying maybe against the (unwrapped) option it holds. Non-template hidden
    // friend (one per instantiation) — avoids the self-referential constraint / redefinition issues
    // of a constrained template friend. `comparable_t` stays a well-formed type even for
    // `maybe<void>` (the `requires has_value` guard removes the operator in that case).
    using comparable_t = std::conditional_t<has_value, std::remove_reference_t<T>, int>;
    friend constexpr bool operator==(maybe const& self, comparable_t const& other)
      requires has_value
    {
      return self.value == other;
    }
    friend constexpr bool operator!=(maybe const& self, comparable_t const& other)
      requires has_value
    {
      return !(self.value == other);
    }
  };

  template<typename T> maybe(T&& t) -> maybe<T>;

  template<typename T>
  inline constexpr bool is_missing_v = std::same_as<std::remove_cvref_t<T>, kwk::__::maybe<void>>;

  template<typename T> using is_missing = std::bool_constant<is_missing_v<T>>;
}
