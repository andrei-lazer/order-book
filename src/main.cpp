#include "benchmark.cpp"
#include <thread>

int main()
{
	size_t n_iter = 1e5;
	OrderBook ob;
	std::thread t_bench(benchmark, std::ref(ob), n_iter);
	// benchmark(ob, n_iter);
	std::thread t_query(query, std::ref(ob), n_iter*1e3);
	t_bench.join();
	t_query.join();
}
