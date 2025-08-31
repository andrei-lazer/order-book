#pragma once

#include "Order.hpp"

class Trade
{
	OrderPtr m_bid;
	OrderPtr m_ask;
	Quantity m_quantity;
public:
	Trade(OrderPtr bid, OrderPtr ask, Quantity quantity)
		: m_bid(bid), m_ask(ask), m_quantity(quantity) {}
	OrderPtr getBidOrder() const { return m_bid; }
	OrderPtr getAskOrder() const { return m_ask; }
	Quantity getQuantity() const { return m_quantity; }
};

using TradeVec = std::vector<Trade>;

struct Level
{
	Price m_price;
	Quantity m_quantity;
};

using LevelVec = std::vector<Level>;

// used for presenting information externally
class OrderBookLevels
{
	LevelVec m_bid_levels;
	LevelVec m_ask_levels;
public:
	OrderBookLevels(LevelVec bid_levels, LevelVec ask_levels)
		: m_bid_levels(bid_levels), m_ask_levels(ask_levels) {}
	const LevelVec& getBids() const { return m_bid_levels; };
	const LevelVec& getAsks() const { return m_ask_levels; };
};
