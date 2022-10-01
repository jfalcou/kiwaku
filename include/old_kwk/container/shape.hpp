namespace kwk
{
  struct size_;

  template<auto Shaper>
  struct shape : detail::prefilled_array<Shaper>
  {
    //==============================================================================================
    //! @brief Constructs from a shape with a lower order
    //!
    //! Constructs a kwk::shape from a shape with higher orders, filling the missing sizes with 1
    //!
    //! This constructor does not participate in overload resolution of the shape is not fully
    //! specified at runtime.
    //!
    //! @groupheader{Example}
    //! @godbolt{docs/shape/odd_sized.cpp}
    //!
    //! @param other  Shape to copy
    //==============================================================================================
    template<auto OtherShaper>
    constexpr shape( shape<OtherShaper> const& other ) noexcept
              requires( OtherShaper.size() < static_order && is_fully_dynamic)
    {
      constexpr auto dz = std::min(OtherShaper.size(),static_order);

      detail::constexpr_for<dz>
      ( [&]<std::ptrdiff_t I>(std::integral_constant<std::ptrdiff_t,I> const&)
        {
          storage()[I] = other.template get<I>();
        }
      );

      for(std::size_t i = dz; i < static_order;++i) storage()[i] = 1;
    }

    //==============================================================================================
    //! @brief Constructs from a shape with a higher order
    //!
    //! Constructs a kwk::shape from a shape with higher orders, compacting the extra-dimensions into
    //! the last.
    //!
    //! This constructor does not participate in overload resolution of the shape is not fully
    //! specified at runtime.
    //!
    //! @groupheader{Example}
    //! @godbolt{docs/shape/odd_sized.cpp}
    //!
    //! @param other  Shape to copy
    //==============================================================================================
    template<auto OtherShaper>
    constexpr explicit  shape( shape<OtherShaper> const& o ) noexcept
                        requires( OtherShaper.size() > static_order && is_fully_dynamic)
    {
      constexpr auto dz = std::min(OtherShaper.size(),static_order);
      constexpr auto oz = shape<OtherShaper>::static_order - dz;
      detail::constexpr_for<dz> ( [&]<typename I>(I)
                                { storage()[I{}] = o.template get<I::value>(); }
                                );
      detail::constexpr_for<oz> ( [&]<typename I>(I)
                                { storage().back() *= o.template get<dz+I::value>(); }
                                );
    }

    /// Assignment operator
    template<auto OtherShaper>
    requires( OtherShaper.size() < static_order || Shaper.is_compatible(OtherShaper) )
    constexpr shape& operator=( shape<OtherShaper> const& other ) noexcept
    {
      shape that(other);
      swap(that);
      return *this;
    }

/*
    // Reshaping interface using slicers
    template<typename... Slices> constexpr auto operator()(Slices... slices) const noexcept
    {
      using that_t = shape<sizeof...(Slices)>;
      auto rs = [&]<std::size_t... I>(std::index_sequence<I...> const&, auto const& s)
      {
        return that_t{ detail::reshape(slices,I,s) ... };
      };

      that_t that{*this};
      that = rs(std::make_index_sequence<sizeof...(Slices)>{}, that);

      return that;
    }
*/

    /// Conversion to kwk::stride
    constexpr auto as_stride() const requires(static_order > 0) { return stride_type(*this); }
  };
}
