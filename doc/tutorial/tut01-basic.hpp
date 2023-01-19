#error This file is for documentation only - DO NOT INCLUDE
/**

@page tutorial-01 Multi-dimensional views

# Introduction

The first container proposed by **KIWAKU** is @ref kwk::view. kwk::view is a non-owning adapter
that can be constructed from various type of data sources so you can manipulate them as a nD array-like
entity. This page will go over the basic of constructing, configuring, accessing and computing
over @view{s}

# A Quick Example

Let's start with a simple example:

@include tutorial/tutorial-01.cpp

After constructing a `std::vector` to holds some data, we can take a @view over
its data. To do so, we create a @view instance. The definition of our
@view may look strange for multiple reasons:

 - @ref kwk::view is a template class but no template parameter is used because we use the
   CTAD (*Constructor Template Argument Deduction*) feature of C++17.

 - `v` is constructed from **named parameters** instead of more traditional, positional parameters.
   The two parameters we use here are @ref kwk::source to specify where the data are stored and
   @ref kwk::of_size to specify the size of each dimensions of the @view.

Those API choices are very important for **KIWAKU** as we want to make the mental burden of
remembering which parameter (type or value alike) come after which other or if any
parameter has a potentially non-trivial default value. We'll go over each parameters as we continue
this serie of tutorials.

Once done, we can just print the contents of the @view. The result here should be:

```
[ 1.2 3.4 5.6 ]
[ 7.8 9.1 11.12 ]
```

# Working with Views
Displaying data on the standard output is fine but, maybe, we want to do some actual work with our
@view.



# Conclusion
In this first tutorial, we managed to:
  - get familiar with kwk::view, the abstraction for non-owning container
  - learn about the named parameter interface of **KIWAKU** containers
  - worked with the data through a @view

[In the next tutorial](@ref tutorial-02), we will take a look at @ref kwk::table, the owning
multi-dimensional container.

**/

