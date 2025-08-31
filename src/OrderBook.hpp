#pragma once

#include "Order.hpp"
#include "Trade.hpp"
#include "Levels.hpp"
#include <thread>
#include <mutex>

class OrderBook
{
	struct Entry
	{
		OrderPtr mp_order;
		// iterator is used since the bid and ask books are stored
		// as lists, so iterators are always valid as long as the list
		// is not destroyed
		OrderPtrList::iterator m_position;
	};

	// ordered map to keep track of lowest ask / highest bid
	std::map<Price, OrderPtrList, std::greater<Price>> m_bids;
	std::map<Price, OrderPtrList, std::less<Price>> m_asks;
	// unordered map to have easy access to orders by ID
	std::unordered_map<OrderId, Entry> m_orders;

	// concurrency stuff
	std::mutex m_lock;
	std::thread m_query_thread; // used to respond to user queries

	TradeVec matchOrders();
	void deleteLevelIfEmpty(Level level);

public:
	TradeVec placeOrder(OrderPtr order);
	void deleteOrder(OrderId orderId);
	void modifyOrder(OrderId orderId, Price newPrice, Quantity newQuantity, Side newSide);
	OrderBookLevels getLevels() const;
};
