# Benchmark

## Methods
Using non-deterministic computing architectures, we can not rely on simple measurements.
Including optimized algorithms for memory management in kiwaku make those measurements even more complex. 
As such, we propose to use the boxplot method to evaluate the variability between stantard memory usage and kiwaku within numerous algorithms.

To evaluate the performances in accordance to the Roofline model, we use CPU-X/Z to determine the bandwidth limits in each cache levels.

https://lemire.me/blog/2023/04/06/are-your-memory-bound-benchmarking-timings-normally-distributed/

### Average & Median
https://en.wikipedia.org/wiki/Arithmetic_mean & https://en.wikipedia.org/wiki/Median

The use of the average and median is a basic and instantaneous information on the performance difference between implementations.

### Box plot Method
https://en.wikipedia.org/wiki/Box_plot

We use boxplot as a detailed statistical method to evaluate the difference in bandwidth usage between standard algorithms implementations and kiwaku implementation.


## Benchmarking 

## std::Chrono

## Nanobench

We use Nanobench as a comparison to std::chrono as it gives the cycles measurements.
