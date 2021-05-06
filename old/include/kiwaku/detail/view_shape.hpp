//================================================================================================
// Default case is for when everything is static
//================================================================================================
template<auto Shape, auto Stride>
struct view_shape
{
  template<typename... Indexer > constexpr auto reshape( Indexer const&... is ) const noexcept
  {
    return Shape(is...);
  }

  template<typename... Indexer > constexpr auto restride( Indexer const&... is ) const noexcept
  {
    return Stride(is...);
  }
};

//================================================================================================
// Optimisation : 1D shape + unit stride
//================================================================================================
template<auto Shape, auto Stride>
requires( Shape.is_dynamic_option && Shape.static_size == 1 && Stride.is_unit_stride )
struct  view_shape<Shape, Stride>
{
  template<typename... Indexer > constexpr auto reshape( Indexer const&... is ) const noexcept
  {
    return shape_(is...);
  }
};

//================================================================================================
// 2D shape + unit stride
// We use EBO to not consume memory if the extra stride data is not required
//================================================================================================
template<auto Shape, auto Stride>
requires( Shape.is_dynamic_option && Shape.static_size == 2 && Stride.is_unit_stride )
struct  view_shape<Shape, Stride>
      : inherits_if<Stride.is_explicit, typename decltype(Stride)::stride_type, empty>
{
  template<typename... Indexer > constexpr auto reshape( Indexer const&... is ) const noexcept
  {
    return shape_(is...);
  }

};

//================================================================================================
// Dynamic nD shape with unit stride
//================================================================================================
template<auto Shape, auto Stride>
requires(   Shape.is_dynamic_option &&  Shape.static_size > 2
        &&  Stride.is_unit_stride   &&  !Stride.is_explicit
        )
struct  view_shape<Shape, Stride>
      : std::array<std::ptrdiff_t, Shape.static_size-2>
{
  template<typename... Indexer > constexpr auto reshape( Indexer const&... is ) const noexcept
  {
    return shape_(is...);
  }

  template<typename... Indexer> constexpr auto offset(Indexer const&... is) const noexcept
  {
    return index(origin(is)...);
  }

  private:

  shape_type      shape_;
  std::ptrdiff_t  numel_;

  template<typename... Is, std::size_t... Idx>
  constexpr auto linearize( std::ptrdiff_t base
                          , std::index_sequence<Idx...> const&, Is... is
                          ) const noexcept
  {
    return (base + ... + (is*parent::operator[](Idx)) );
  };
};

//================================================================================================
// Dynamic nD shape with non-unit stride ?
//================================================================================================
template<auto Shape, auto Stride>
requires( !Stride.is_unit_stride )
struct  view_shape<Shape, Stride>
{
  using shape_type  = typename decltype(Shape)::shape_type;
  using stride_type = typename decltype(Stride)::stride_type;

  constexpr view_shape( shape_type const& shp, stride_type const& str )
          : shape_(shp), stride_(str), numel_{shp.numel()}
  {}

  constexpr view_shape( shape_type const& shp )  : view_shape(shp,shp) {}

  constexpr std::size_t     size()          const noexcept  { return numel_;  }
  constexpr std::size_t     size(int dim)   const noexcept  { return shape_[dim]; }

  constexpr std::ptrdiff_t  count()         const noexcept  { return numel_;  }
  constexpr std::ptrdiff_t  count(int dim)  const noexcept  { return shape_[dim]; }

  constexpr auto shape()  const noexcept  { return shape_;  }
  constexpr auto stride() const noexcept  { return stride_; }

  template<typename... Is>
  constexpr std::ptrdiff_t index(Is... is) const noexcept { return stride_.index(is...); }

  template<typename... Indexer > constexpr auto reshape( Indexer const&... is ) const noexcept
  {
    return shape_(is...);
  }

  private:

  shape_type      shape_;
  stride_type     stride_;
  std::ptrdiff_t  numel_;
};
