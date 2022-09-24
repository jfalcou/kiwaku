Indexing and Sub-Views {#indexing}
======================

@tableofcontents

@section indexing-general Position of the Problem

Multi-dimensional containers are designed to provide a semantic-rich API to access their contents.
**KIWAKU** provides two way access data within a container:
  - a scalar value access at a given position
  - an extraction mechanism to build sub-views from existing containers

@section indexing-access Element Access

@subsection indexing-access-absolute Absolute indexing
**KIWAKU**'s containers support an index-based, multi-dimensional access through `operator()`.
This indexing looks like the traditional indexing for C-like arrays.

@code{.cpp}
#include <kwk/kwk.hpp>
#include <iostream>

int main()
{
  // t first valid index is 0 by default
  auto t = kwk::table{ kwk::type<int>, kwk::of_size(3,2)};

  for(int j=0;j < kwk::size<1>(d);++j)
    for(int i=0;i < kwk::size<0>(d);++i)
      t(i,j) = i + 10 *j;

  std::cout << t << '\n';
}
@endcode

However, containers support an option - @ref kwk::bases - to modify the first valid value
for an index. This means that, when using a set of integral index, one must be careful to
only use value between the base index and the final valid index.

@code{.cpp}
#include <kwk/kwk.hpp>
#include <iostream>

int main()
{
  // t first valid index is now 1
  auto t = kwk::table{ kwk::type<int>, kwk::bases<1>, kwk::of_size(3,2)};

  for(int j=1;j <= kwk::size<1>(d);++j)
    for(int i=1;i <= kwk::size<0>(d);++i)
      t(i,j) = i + 10 *j;

  std::cout << t << '\n';
}
@endcode

@subsection indexing-access-automatic Automatic indexing

If using custom base index in those situations is fair, it becomes more complex to do so in more
generic contexts. To performs such tasks in a more generic way, you should use the @ref kwk::positions
generator.

@ref kwk::positions walks through all valid n-dimensional position within a **KIWAKU** container
without the end-user having to take care of base indexes or sizes. Its usage is as follow:

@code{.cpp}
#include <kwk/kwk.hpp>
#include <iostream>

template<typename Data>
void process(Data& d)
{
  for(auto p : kwk::positions(d))
  {
    auto[x,y] = p;
    d(p) = x + y * kwk::size<0>(d);
  }
}

int main()
{
  auto t0 = kwk::table{ kwk::type<float>, kwk::bases<0>, kwk::of_size(3,2)};
  auto t1 = kwk::table{ kwk::type<int>, kwk::bases<1>, kwk::of_size(3,2)};

  process(t0);
  process(t1);

  std::cout << t0 << '\n';
  std::cout << t1 << '\n';
}
@endcode

The `process` generic function uses @ref kwk::positions to iterates over all valid set of indexes
for its parameters. THe access to `d` via `p` is always valid.

The object returned by @ref positions can be used in a structured binding if one wants to extract
its internal indexes.

@section indexing-extract Extracting Sub-view

