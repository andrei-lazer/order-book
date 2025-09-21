# 📈 Order Book Matching Engine

This is a price-time priority limit order book implemented in C++.

## 🚀 Features
- Add, Cancel and Match events are published to a lock-free ring buffer, enabling non-blocking
  queries.
- Unit testing with Google Test.
- Benchmarking obtained rates of up to **530,000 orders/second** including matches and concurrent
  querying.

## Benchmark Demo

Running the benchmarker gives the following output on my machine:

```
ADD
530842 iterations/second
Latency (ns):
	mean=1883.8
	standard deviation: 2592.94
	p50=1207
	p99=5635

CANCEL
3.69322e+06 iterations/second
Latency (ns):
	mean=270.767
	standard deviation: 615.866
	p50=101
	p99=1107

SUCCESSFUL QUERY
4.6153e+06 iterations/second
Latency (ns):
	mean=216.67
	standard deviation: 760.624
	p50=201
	p99=302

```

## Build
#### Requirements
- `cmake`
- C++23

#### Build benchmarker only
1. clone this repository
2. `cd order-book`
3. `cmake -B build`
4. `cmake --build build --parallel`

Then, running `build/Benchmark` will run the benchmarker.

#### Build benchmarker AND unit tests.
1. Steps 1 and 2 are the same.
2. `cmake -B build -DBUILD_TESTS=ON`
3. `cmake --build build --parallel`

Then, running `build/Benchmark` will run the benchmarker, and `ctest --test-dir build` will run the
unit tests.
