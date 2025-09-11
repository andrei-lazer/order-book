#include <iostream>
#include <memory>
#include <vector>
#include <chrono>
#include <random>
#include <algorithm>
#include "Order.hpp"
#include "OrderBook.hpp"

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

auto report = [](const std::string& name, std::vector<double>& latencies) {
	double mean = std::accumulate(latencies.begin(), latencies.end(), 0.0) / latencies.size();
	double p50 = percentile(latencies, 0.50);
	double p99 = percentile(latencies, 0.99);
	double p999 = percentile(latencies, 0.999);
	double _sd  = sd(latencies, mean);
	std::cout << name  << "\n" << 1e9/mean <<  " iterations/second\nLatency (ns):\n\tmean=" << mean
			  << "\n\tstandard deviation: " << _sd
			  << "\n\tp50=" << p50 << "\n\tp99=" << p99 << std::endl << std::endl;
};

void query(OrderBook& ob, size_t n_iter)
{
	// queries the order book (should be ran on another thread)
	Event event;

	std::vector<double> latencies;
	latencies.reserve(n_iter);
	bool worked;
	for (size_t i = 0; i < n_iter; i++)
	{
		auto start = std::chrono::high_resolution_clock::now();
		worked = ob.popEvent(event);
		auto end = std::chrono::high_resolution_clock::now();

		if (worked)
		{
			std::chrono::duration<double, std::nano> dur = end - start;
			latencies.push_back(dur.count());
		}
	}

	report("SUCCESSFUL QUERY", latencies);
}

void benchmark(OrderBook& ob, size_t n_iter) {
    std::vector<int> order_ids;
    order_ids.reserve(n_iter);

    std::vector<double> add_latencies, cancel_latencies;
    add_latencies.reserve(n_iter);
    cancel_latencies.reserve(n_iter/2);

    std::mt19937 rng(42);
    std::uniform_real_distribution<> price_dist(90, 110);
    std::uniform_int_distribution<int> qty_dist(1, 100);

    // Benchmark add orders
    for (size_t i = 0; i < n_iter; i++) {
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
    for (size_t i = 0; i < n_iter/2; i++) {
        auto start = std::chrono::high_resolution_clock::now();
        ob.cancelOrder(order_ids[i]);
        auto end = std::chrono::high_resolution_clock::now();

        std::chrono::duration<double, std::nano> dur = end - start;
        cancel_latencies.push_back(dur.count());
    }

    report("ADD", add_latencies);
    report("CANCEL", cancel_latencies);
}
