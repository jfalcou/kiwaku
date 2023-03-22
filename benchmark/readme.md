# Benchmark

## Nanobench

We use Nanobench as a comparison to std::chrono for the cycles measurements :
```
ankerl::nanobench::Bench bcopy;

bcopy.epochIterations(num_times).epochs(1).run("Copy", [&]{
  copy(a, c);
});

std::vector<ankerl::nanobench::Result> vres;
vres = bcopy.results();
double cyc_op = vres.begin()->median(ankerl::nanobench::Result::Measure::cpucycles);
std::cout << "Resultat nanobench mult: " << cyc_op << std::endl;
```