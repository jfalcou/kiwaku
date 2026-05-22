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
  namespace __
  {
    struct blob_id : kumi::identifier<blob_id>
    {
      friend constexpr auto to_str(blob_id) { return kumi::str{"Blob"}; }
    };

    inline constexpr blob_id blob_type{};

    /*
      @brief Blob is also it s self keyword for simpler manipulation inside collection
      A blob is composed of two simple parts : the part that handles the type and a backend
      that holds or not the memory and gives us access to raw bytes.
    */
    template<typename T, typename Storage> struct blob : Storage
    {
      using Storage::Storage;

      using element_type = T;
      using type = blob;
      using identifier_type = __::blob_id;
      using label_type = kumi::str;

      using Q = std::remove_cvref_t<T>;

      constexpr auto operator()(identifier_type const&) const { return *this; }

      static constexpr label_type label() { return kumi::str{"Blob"}; }

      template<typename Self> constexpr decltype(auto) operator[](this Self& self, auto i)
      {
        auto* addr = &self.get()[static_cast<std::size_t>(i) * sizeof(T)];
        return *std::launder(reinterpret_cast<T*>(addr));
      }

      template<typename Self> constexpr auto initialize(this Self& self, std::size_t size)
      {
        for (std::size_t i = 0; i < size; i += sizeof(Q))
        {
          auto* addr = &self.get()[i];
          if constexpr (std::is_implicit_lifetime_v<Q>) kwk::__::start_lifetime_as<Q>(addr);
          else ::new (addr) Q;
        }
      }

      template<typename Self> constexpr auto copy(this Self& self, T const* ptr, std::size_t size)
      {
        auto* addr = &self.get()[0];
        if constexpr (std::is_trivially_copyable_v<Q>) std::memcpy(addr, ptr, size * sizeof(T));
        else
        {
          for (std::size_t i = 0; i < size; ++i)
          {
            auto* cpy = ::new (addr) Q;
            *cpy = ptr[i];
            addr += i * sizeof(Q);
          }
        }
      }

      template<typename Self> constexpr void destroy(this Self& self, std::size_t size)
      {
        if constexpr (!std::is_implicit_lifetime_v<Q>)
        {
          for (std::size_t i = 0; i < size; ++i) kwk::__::destroy_at(&self[i]);
        }
      }

      template<typename CharT, typename Traits>
      friend std::basic_ostream<CharT, Traits>& operator<<(std::basic_ostream<CharT, Traits>& os, blob const&) noexcept
      {
        return os << "Blob type";
      }
    };
  }
}

#include <kwk/utility/memory/blob/heap_blob.hpp>
#include <kwk/utility/memory/blob/stack_blob.hpp>
#include <kwk/utility/memory/blob/shallow_blob.hpp>
