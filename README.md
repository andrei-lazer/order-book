# 📈 Order Book Matching Engine

This is a price-time priority limit order book implemented in C++.

## 🚀 Features
- Add, Cancel and Match events are published to a lock-free ring buffer, enabling non-blocking
  queries.
- Unit testing with Google Test.
- Benchmarking obtained rates of up to *580,000 orders/second* including matches and concurrent
  querying.

## Benchmark Demo

## 📒 To-Do
- Market orders

## Building details
This uses CMake for the build system.
