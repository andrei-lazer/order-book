#include "OrderBook.hpp"
#include "Order.hpp"
#include <algorithm>

TradeVec OrderBook::placeOrder(OrderPtr order)
{
	TradeVec trades;
	if (m_orders.contains(order->getOrderId()))
		return trades;

	OrderPtrList::iterator position;
	// OrderPtrList& orders;
	OrderPtrList* p_orders;

	if (order->getSide() == Side::Bid)
	{
		p_orders = &m_bids[order->getPrice()];
	}
	else
	{
		p_orders = &m_asks[order->getPrice()];
	}

	// push_back does NOT invalidate iterators according to cpp reference, so this is safe.
	p_orders->push_back(order);
	position = std::next(p_orders->end(), -1);

	Entry entry = {order, position};
	m_orders.insert({order->getOrderId(), entry});

	trades = matchOrders();
	return trades;
}

TradeVec OrderBook::matchOrders()
{
	TradeVec trades;

	while (!m_bids.empty() && !m_asks.empty())
	{
		auto& [bid_price, bids] = *m_bids.begin();
		auto& [ask_price, asks] = *m_asks.begin();
		
		if (bid_price >= ask_price)
		{
			while (!bids.empty() && !asks.empty())
			{
				OrderPtr& bid = bids.front();
				OrderPtr& ask = asks.front();

				Quantity quantity = std::min(bid->getQuantity(), ask->getQuantity());

				bid->fill(quantity);
				ask->fill(quantity);

				// record the trade
				Trade t(bid, ask, quantity);
				trades.push_back(t);

				// remove entire order if fulfilled
				if (bid->isEmpty())
				{
					m_orders.erase(bid->getOrderId());
					bids.pop_front();
				}
				if (ask->isEmpty())
				{
					m_orders.erase(ask->getOrderId());
					asks.pop_front();
				}
				// one of those references (bid or ask) is now invalid
			}
		}
		else
		{
			break;
		}
		// remove this price from either bid or ask map
		if (bids.empty())
		{
			m_bids.erase(bid_price);
		}

		if (asks.empty())
		{
			m_asks.erase(ask_price);
		}
	}

	return trades;
}

OrderBookLevels OrderBook::getLevels() const
{
	LevelVec bid_levels;
	bid_levels.reserve(m_bids.size());

	LevelVec ask_levels;
	ask_levels.reserve(m_asks.size());

	for (auto& [price, orders] : m_bids)
	{
		// creating a level info per price
		Quantity quantity = 0;
		// not sure of the time complexity of this,
		// since retrieval from a list is O(n),
		// but ":" iteration might be different
		for (OrderPtr op : orders)
		{
			quantity += op->getQuantity();
		}
		bid_levels.push_back({price, quantity});
	}

	for (auto& [price, orders] : m_asks)
	{
		Quantity quantity = 0;
		for (OrderPtr op : orders)
		{
			quantity += op->getQuantity();
		}
		ask_levels.push_back({price, quantity});
	}

	return OrderBookLevels{bid_levels, ask_levels};
}

void OrderBook::deleteOrder(OrderId orderId)
{
	if (!m_orders.contains(orderId))
	{
		return;
	}

	const auto& [p_order, position] = m_orders.at(orderId);

	Price price = p_order->getPrice();

	if (p_order->getSide() == Side::Bid)
	{
		OrderPtrList& level_orders = m_bids.at(price);
		// std::list::erase runs in constant time when given an iterator
		level_orders.erase(position);
		if (level_orders.empty())
			m_bids.erase(price);
	}
	else
	{
		OrderPtrList& level_orders = m_asks.at(price);
		level_orders.erase(position);
		if (level_orders.empty())
			m_asks.erase(price);
	}

	m_orders.erase(orderId);
}

void OrderBook::modifyOrder(OrderId orderId, Price newPrice, Quantity newQuantity, Side newSide)
{
	if (!m_orders.contains(orderId))
		return;
	
	auto& [p_order, position] = m_orders.at(orderId);

	Price price = p_order->getPrice();
	Quantity quantity = p_order->getQuantity();
	Side side = p_order->getSide();

	// check if it loses priority
	if ((newPrice != price) ||
		(newSide != side) ||
		(newQuantity > quantity))
	{
		deleteOrder(orderId);
	}
}
