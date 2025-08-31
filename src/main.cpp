#include <iostream>
#include <memory>
#include "OrderBook.hpp"

int main()
{
	OrderBook ob;

	Order order(1, Side::BID, 12, 1);
	ob.placeOrder(std::make_shared<Order>(order));

	std::cout << "Hello world" << std::endl;
}
