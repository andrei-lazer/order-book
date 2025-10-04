#include <gtest/gtest.h>
#include <memory>
#include "Order.hpp"
#include "OrderBook.hpp"

// testing if placeOrder correctly adds a bid order
TEST(OBTest, SimpleBid) {
	OrderBook ob;

	Order order(1, Side::Bid, 12, 1, OrderType::Limit);
	std::shared_ptr<Order> order_sp = std::make_shared<Order>(order);

	ob.placeOrder(order_sp);

	// getting information
	Event e;
	ob.popEvent(e);

	EXPECT_EQ(e.m_type, EventType::Add);
	EXPECT_EQ(e.mp_order, order_sp);
	EXPECT_EQ(e.mp_match, nullptr);
	EXPECT_EQ(e.m_price, 12);
	EXPECT_EQ(e.m_quantity, 1);
}

TEST(OBTest, SimpleAsk) {
	OrderBook ob;

	Order order(1, Side::Ask, 12, 1, OrderType::Limit);
	std::shared_ptr<Order> order_sp = std::make_shared<Order>(order);

	ob.placeOrder(order_sp);

	// getting information
	Event e;
	ob.popEvent(e);

	EXPECT_EQ(e.m_type, EventType::Add);
	EXPECT_EQ(e.mp_order, order_sp);
	EXPECT_EQ(e.mp_match, nullptr);
	EXPECT_EQ(e.m_price, 12);
	EXPECT_EQ(e.m_quantity, 1);
}

TEST(OBTest, UnmatchedOrder)
{
	OrderBook ob;

	Order ask(1, Side::Ask, 13, 1, OrderType::Limit);
	std::shared_ptr<Order> ask_sp = std::make_shared<Order>(ask);
	ob.placeOrder(ask_sp);

	Order bid(2, Side::Bid, 12, 1, OrderType::Limit);
	std::shared_ptr<Order> bid_sp = std::make_shared<Order>(bid);
	ob.placeOrder(bid_sp);

	// getting information
	Event ask_event;
	Event bid_event;

	EXPECT_TRUE(ob.popEvent(ask_event));
	EXPECT_TRUE(ob.popEvent(bid_event));

	EXPECT_EQ(ask_event.m_type, EventType::Add);
	EXPECT_EQ(ask_event.mp_order, ask_sp);
	EXPECT_EQ(ask_event.mp_match, nullptr);
	EXPECT_EQ(ask_event.m_price, 13);
	EXPECT_EQ(ask_event.m_quantity, 1);


	EXPECT_EQ(bid_event.m_type, EventType::Add);
	EXPECT_EQ(bid_event.mp_order, bid_sp);
	EXPECT_EQ(bid_event.mp_match, nullptr);
	EXPECT_EQ(bid_event.m_price, 12);
	EXPECT_EQ(bid_event.m_quantity, 1);
}

TEST(OBTest, MatchedOrder)
{
	OrderBook ob;

	Order ask(1, Side::Ask, 11, 1, OrderType::Limit);
	std::shared_ptr<Order> ask_sp = std::make_shared<Order>(ask);
	ob.placeOrder(ask_sp);

	Order bid(2, Side::Bid, 12, 1, OrderType::Limit);
	std::shared_ptr<Order> bid_sp = std::make_shared<Order>(bid);
	ob.placeOrder(bid_sp);

	// getting information
	Event ask_event;
	Event bid_event;
	Event match_event;

	EXPECT_TRUE(ob.popEvent(ask_event));
	EXPECT_TRUE(ob.popEvent(bid_event));
	EXPECT_TRUE(ob.popEvent(match_event));

	EXPECT_EQ(ask_event.m_type, EventType::Add);
	EXPECT_EQ(ask_event.mp_order, ask_sp);
	EXPECT_EQ(ask_event.mp_match, nullptr);
	EXPECT_EQ(ask_event.m_price, 11);
	EXPECT_EQ(ask_event.m_quantity, 1);

	EXPECT_EQ(bid_event.m_type, EventType::Add);
	EXPECT_EQ(bid_event.mp_order, bid_sp);
	EXPECT_EQ(bid_event.mp_match, nullptr);
	EXPECT_EQ(bid_event.m_price, 12);
	EXPECT_EQ(bid_event.m_quantity, 1);

	EXPECT_EQ(match_event.m_type, EventType::Match);
	EXPECT_EQ(match_event.mp_order, bid_sp);
	EXPECT_EQ(match_event.mp_match, ask_sp);
	EXPECT_EQ(match_event.m_price, 11);
	EXPECT_EQ(match_event.m_quantity, 1);
}

TEST(OBTest, DeletedAsk)
{
	OrderBook ob;

	Order ask(1, Side::Ask, 11, 1, OrderType::Limit);
	std::shared_ptr<Order> ask_sp = std::make_shared<Order>(ask);
	ob.placeOrder(ask_sp);

	ob.cancelOrder(1);

	// getting information
	Event ask_event;
	Event cancel_event;

	EXPECT_TRUE(ob.popEvent(ask_event));
	EXPECT_TRUE(ob.popEvent(cancel_event));

	EXPECT_EQ(ask_event.m_type, EventType::Add);
	EXPECT_EQ(ask_event.mp_order, ask_sp);
	EXPECT_EQ(ask_event.mp_match, nullptr);
	EXPECT_EQ(ask_event.m_price, 11);
	EXPECT_EQ(ask_event.m_quantity, 1);

	EXPECT_EQ(cancel_event.m_type, EventType::Cancel);
	EXPECT_EQ(cancel_event.mp_order, ask_sp);
	EXPECT_EQ(cancel_event.mp_match, nullptr);
	EXPECT_EQ(cancel_event.m_price, 11);
	EXPECT_EQ(cancel_event.m_quantity, 1);
}
