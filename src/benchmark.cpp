#include <iostream>
#include <memory>
#include <vector>
#include <chrono>
#include <random>
#include <algorithm>
#include "Order.hpp"
#include "OrderBook.hpp"

// Utility: calculate percentile
double percentile(std::vector<double>& data, double p) {
    if (data.empty()) return 0.0;
    std::sort(data.begin(), data.end());
    size_t idx = static_cast<size_t>(p * data.size());
    if (idx >= data.size()) idx = data.size() - 1;
    return data[idx];
}

double sd(std::vector<double>& data, double mean)
{
	size_t n = data.size();
	double var = std::accumulate(data.begin(), data.end(), 0.0, 
			[&mean, &n](const double& a, const double& b)
			{
				return a + (b-mean)*(b-mean)/(n-1);
			}
			);

	return std::sqrt(var);
}

void benchmark(size_t N) {
    OrderBook ob;
    std::vector<int> order_ids;
    order_ids.reserve(N);

    std::vector<double> add_latencies, cancel_latencies;
    add_latencies.reserve(N);
    cancel_latencies.reserve(N/2);

    std::mt19937 rng(42);
    std::uniform_real_distribution<> price_dist(90, 110);
    std::uniform_int_distribution<int> qty_dist(1, 100);

    // Benchmark add orders
    for (size_t i = 0; i < N; i++) {
        auto side = (i % 2 == 0 ? Side::Bid : Side::Ask);
        int price = price_dist(rng);
        int qty = qty_dist(rng);

		std::shared_ptr<Order> order_ptr = std::make_shared<Order>(i, side, price, qty, OrderType::Limit);

        auto start = std::chrono::high_resolution_clock::now();
        ob.placeOrder(order_ptr);
        auto end = std::chrono::high_resolution_clock::now();

        std::chrono::duration<double, std::nano> dur = end - start;
        add_latencies.push_back(dur.count());
        order_ids.push_back(i);
    }

    // Benchmark cancels (cancel half of them)
    for (size_t i = 0; i < N/2; i++) {
        auto start = std::chrono::high_resolution_clock::now();
        ob.deleteOrder(order_ids[i]);
        auto end = std::chrono::high_resolution_clock::now();

        std::chrono::duration<double, std::nano> dur = end - start;
        cancel_latencies.push_back(dur.count());
    }

    auto report = [](const std::string& name, std::vector<double>& latencies) {
        double mean = std::accumulate(latencies.begin(), latencies.end(), 0.0) / latencies.size();
        double p50 = percentile(latencies, 0.50);
        double p99 = percentile(latencies, 0.99);
        double p999 = percentile(latencies, 0.999);
		double _sd  = sd(latencies, mean);
        std::cout << name << " Latency (ns): mean=" << mean
				  << " standard deviation: " << _sd
                  << " p50=" << p50 << " p99=" << p99 << " p999=" << p999 << std::endl;
    };

	std::vector<double> test_vec = {0.0, 1.0, 2.0, 3.0};
	std::cout << "Test sd: " << sd(test_vec, 1.5) << std::endl;
    report("Add", add_latencies);
    report("Cancel", cancel_latencies);
}

