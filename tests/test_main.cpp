#include <gtest/gtest.h>
#include <memory>
// #include "../src/Order.hpp"
// #include "../src/Order.cpp"
#include "Order.hpp"
#include "OrderBook.hpp"

// testing if placeOrder correctly adds a bid order
TEST(OBTest, SimpleBid) {
	OrderBook ob;

	Order order(1, Side::Bid, 12, 1, OrderType::Limit);
	std::shared_ptr<Order> order_sp = std::make_shared<Order>(order);

	ob.placeOrder(order_sp);

	OrderBookLevels levels = ob.getLevels();

	LevelVec bid_levels = levels.getBids();

	Price bid_price = bid_levels.at(0).m_price;
	Quantity bid_quantity = bid_levels.at(0).m_quantity;

	EXPECT_EQ(bid_price, 12);
}

TEST(OBTest, SimpleAsk) {
	OrderBook ob;

	Order order(1, Side::Ask, 12, 1, OrderType::Limit);
	std::shared_ptr<Order> order_sp = std::make_shared<Order>(order);

	ob.placeOrder(order_sp);

	OrderBookLevels levels = ob.getLevels();

	LevelVec ask_levels = levels.getAsks();

	Price ask_price = ask_levels.at(0).m_price;
	Quantity ask_quantity = ask_levels.at(0).m_quantity;

	EXPECT_EQ(ask_price, 12);
}


TEST(OBTest, UnmatchedOrder)
{
	OrderBook ob;

	// place ask at 12
	Order ask(1, Side::Ask, 14, 1, OrderType::Limit);
	std::shared_ptr<Order> ask_sp = std::make_shared<Order>(ask);

	ob.placeOrder(ask_sp);

	OrderBookLevels levels1 = ob.getLevels();
	EXPECT_EQ(levels1.getAsks().size(), 1);

	// place bid at 13
	Order bid(2, Side::Bid, 13, 1, OrderType::Limit);
	std::shared_ptr<Order> bid_sp = std::make_shared<Order>(bid);

	TradeVec trades = ob.placeOrder(bid_sp);
	OrderBookLevels levels = ob.getLevels();

	EXPECT_EQ(levels.getAsks().size(), 1);
	EXPECT_EQ(levels.getBids().size(), 1);

	// tradevec should also have 0 trades
	EXPECT_EQ(trades.size(), 0);
}

TEST(OBTest, MatchedOrder)
{
	OrderBook ob;

	// place ask at 12
	Order ask(1, Side::Ask, 12, 1, OrderType::Limit);
	std::shared_ptr<Order> ask_sp = std::make_shared<Order>(ask);

	ob.placeOrder(ask_sp);

	OrderBookLevels levels1 = ob.getLevels();
	EXPECT_EQ(levels1.getAsks().size(), 1);

	// place bid at 13
	Order bid(2, Side::Bid, 13, 1, OrderType::Limit);
	std::shared_ptr<Order> bid_sp = std::make_shared<Order>(bid);

	TradeVec trades = ob.placeOrder(bid_sp);

	OrderBookLevels levels = ob.getLevels();

	EXPECT_EQ(levels.getAsks().size(), 0);
	EXPECT_EQ(levels.getBids().size(), 0);

	// tradevec should also have 1 trade recorded w quantity 1!
	EXPECT_EQ(trades.size(), 1);
	EXPECT_EQ(trades.at(0).getQuantity(), 1);
}

TEST(OBTest, DeletedAsk)
{
	OrderBook ob;

	// place ask at 12
	Order ask(1, Side::Ask, 12, 1, OrderType::Limit);
	std::shared_ptr<Order> ask_sp = std::make_shared<Order>(ask);

	ob.placeOrder(ask_sp);

	EXPECT_EQ(ob.getLevels().getAsks().size(), 1);

	ob.deleteOrder(1);

	EXPECT_EQ(ob.getLevels().getAsks().size(), 0);
}
