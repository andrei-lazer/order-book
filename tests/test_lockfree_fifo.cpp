#include <gtest/gtest.h>
#include <LockFreeFIFO.hpp>
#include <thread>

template class LockFreeFIFO<int>;

// testing if placeOrder correctly adds a bid order
TEST(LFFIFOTest, PushAndPop) {
	LockFreeFIFO<int> lf_fifo(4);

	auto f = [&lf_fifo]()
	{
		for (int i = 0; i < 4; i++)
			lf_fifo.push(i);
	};

	auto g = [&lf_fifo](int& x) -> int&
	{
		for (int i = 0; i < 4; i++)
			lf_fifo.pop(x);
		return x;
	};

	int y;
	std::thread t1(f);
	std::thread t2(g, std::ref(y));

	t1.join();
	t2.join();

	EXPECT_LE(y, 3);
}

TEST(LFFIFOTest, PopWhenEmpty)
{
	LockFreeFIFO<int> lf_fifo(1);
	int x;
	bool response = lf_fifo.pop(x);

	EXPECT_EQ(response, false);
}

TEST(LFFIFOTest, PushWhenFull)
{
	LockFreeFIFO<int> lf_fifo(1);
	bool response_1 = lf_fifo.push(1);
	bool response_2 = lf_fifo.push(1);

	EXPECT_EQ(response_1, true);
	EXPECT_EQ(response_2, false);
}
