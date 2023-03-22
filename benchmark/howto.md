# HOW TO BENCH

## CHORONO
```
t1 = std::chrono::high_resolution_clock::now();
// some code
t2 = std::chrono::high_resolution_clock::now();
timings = std::chrono::duration_cast<std::chrono::duration<double, std::micro>>(t2 - t1);
```

## NANOBENCH

```
ankerl::nanobench::Bench Bbench;

Bbench.epochIterations(num_times).epochs(epochs).run("Name", [&]{
// some code
});

std::vector<ankerl::nanobench::Result> vres;
vres = Bbench.results();
double metric = vres.begin()->median(ankerl::nanobench::Result::Measure::cpucycles); // or another Result::Measure metric
std::cout << "Resultat nanobench : " << std::fixed << metric << std::endl;
```