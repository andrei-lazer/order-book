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
530627 iterations/second
Latency (ns):
	mean=1884.56
	standard deviation: 2387.76
	p50=1208
	p99=5434

CANCEL
3.86685e+06 iterations/second
Latency (ns):
	mean=258.609
	standard deviation: 462.762
	p50=101
	p99=1107

Total quantity queried: 7572610
```

## Build
#### Requirements
- `cmake`
- C++23

#### Steps
1. clone this repository
2. `cd order-book`
3. `cmake -B build`
4. `cmake --build build --parallel`

Then, running `build/Benchmark` will run the benchmarker,
and `build/Tests` will run the unit tests.
