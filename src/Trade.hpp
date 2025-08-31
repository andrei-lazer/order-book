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
