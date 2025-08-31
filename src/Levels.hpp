#pragma once
#include "Order.hpp"

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
