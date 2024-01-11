#include <benchmark/benchmark.h>
#include <cstring>
#include <vector>

static void BM_StringCreation(benchmark::State& state) {
  for (auto _ : state)
    std::string empty_string;
}
// Register the function as a benchmark
BENCHMARK(BM_StringCreation);

// Define another benchmark
static void BM_StringCopy(benchmark::State& state) {
  std::string x{"hello"};
  for (auto _ : state)
    std::string copy{x};
}

BENCHMARK(BM_StringCopy);

static void BM_memcpy(benchmark::State& state) {
    char* src{new char[state.range(0)]};
    char* dst{new char[state.range(0)]};
    for (auto _: state) {
        memcpy(dst, src, state.range(0));
    }
    delete[] src;
    delete[] dst;
}

// test memcpy for 8 -> 8MB
// BENCHMARK(BM_memcpy)->Range(8, 8<<20);

static void BM_InsertString(benchmark::State& state) {
    std::string x{"hello"};
    std::vector<std::string> v;
    for (auto _: state) {
        v.push_back(x);
    }
}

BENCHMARK(BM_InsertString);



static void BM_EmplaceString(benchmark::State& state) {
    std::string x{"hello"};
    std::vector<std::string> v;
    for (auto _: state) {
        v.emplace_back("hello");
    }
}

BENCHMARK(BM_EmplaceString);


BENCHMARK_MAIN();