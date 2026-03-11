#error This file is for documentation only - DO NOT INCLUDE
/**
@page options Options in kiwaku

In kiwaku, instead of having different container types, one has a single type that differs in behavior depending on the 
options that are given at construction time. In the litterature this is often reffered to as building a **data 
descriptor**. Below is the list of options one can specify in kiwaku and how they interact with each others.


|Option   | Allocator | Axis  | Generator | Indexing  | Kind  | Size  | Source  | Stride
|:--------|:----------|:------|:----------|:--------- |:----- |:------|:--------|:------
Allocator |           | V     | X         | V         | V     | V     | V       | V
Axis      |   V       |       | V         | V         | V     | V     | V       | V
Generator |   X       | V     |           | X         | V     | V     | X       | V
Indexing  |   V       | V     | X         |           | V     | V     | V       | V
Kind      |   V       | V     | V         | V         |       | V     | V       | V
Size      |   V       | V     | V         | V         | V     |       | V       | V
Source    |   V       | V     | V         | V         | V     | V     |         | V
Stride    |   V       | V     | V         | V         | V     | V     | V       |

+ Allocator : Takes any object modeling kwk::allocator to allocate the data space

+ Generator : Takes a callable that can be of two different types. If this option is set, a table actually holds no data
              as they can be computed "on the fly".
              1) A callable that returns a value based on a position or a position and the shape of the container.
              2) A callable that takes no parameter and returns a constant

+ Indexing : Takes in a callable with a fixed signature type. If this optios is set, one cannot specify a generator.
              The callable takes in either a position or a position and the shape of the container and returns the value
              at the given position according to the desired behavior. (As an example on could chose an idnexing 
              strategy where we wrap around when the index goes out of bounds).

+ Kind : Specifies the type that is stored inside a container

+ Size : Specifies to total number of elements inside the container.

+ Source : A kiwaku container can be built from any other type that models kwk::contiguous_container or a raw pointer. 
          Source is the option that handles the way elements are repesented internally keeping as much information about
          the source as necessary.

+ Stride : Specifies the explicit strides of a container.

**/
