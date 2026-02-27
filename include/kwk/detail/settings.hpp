//======================================================================================================================
/*
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Project Contributors
  SPDX-License-Identifier: BSL-1.0
*/
//======================================================================================================================
#pragma once

#include <kumi/kumi.hpp>
#include <iosfwd>

namespace kwk::_
{
  /*
    A flag acts as both an identifier and a boolean value. It is identified by its name and a type.
    It can be used in a kumi::record to represent an option that can be either enabled or disabled.
  */
  template<kumi::str Name, typename T> struct flag : kumi::identifier<T>
  {
    using type = T;
    using identifier_type = T;

    constexpr bool operator()(identifier_type) const { return true; }

    static constexpr auto name() { return Name; }
  };

  /*
    An enumerated is a flag that has an associated integer index. It can be used in a kumi::record to represent an
    option that can be either enabled or disabled and which has a specific position in a bitfield. The enumerated_value
    is used to compute the bitmask for the flag.
  */
  template<kumi::str Name, typename T, int ID> struct enumerated : flag<Name, T>
  {
    static constexpr int enumerated_value = ID;
  };

  /*
    Checks if a type is an enumerated by checking if it has the enumerated_value member.
  */
  template<typename T> struct is_enumerated
  {
    static constexpr bool value = requires { T::enumerated_value; };
  };

  /*
    Converts a field to its corresponding flag value. If the field is an enumerated, it returns the bitmask
    for the flag, otherwise it returns 0.
  */
  template<kumi::concepts::field O>
  inline constexpr int as_flags = []() {
    if constexpr (is_enumerated<O>::value) return (1 << O::type::enumerated_value);
    else return 0;
  }();

  /*
    Constructs a kumi::record containing only the values of the fields that are not enumerated. This can be useful to
    separate the flags from the other fields in a record, for example to pass only the non-flag fields to a function
    that does not need to know about the flags.
  */
  template<kumi::concepts::field... Os> constexpr auto as_values(Os const&... os)
  {
    kumi::record r{os...};
    return kumi::filter_not<is_enumerated>(r);
  }
}
