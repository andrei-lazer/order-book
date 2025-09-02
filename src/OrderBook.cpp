#include "OrderBook.hpp"
#include "Order.hpp"
#include <algorithm>

OrderBook::OrderBook() : m_event_queue{2048} {}

void OrderBook::placeOrder(OrderPtr order)
{
	if (m_orders.contains(order->getOrderId()))
		return;

	OrderPtrList::iterator position;
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

	// adding to the event queue
	Event event(
		EventType::Add,
		order,
		nullptr,
		order->getPrice(),
		order->getQuantity()
	);
	m_event_queue.push(event);

	matchOrders();
}

void OrderBook::matchOrders()
{
	while (!m_bids.empty() && !m_asks.empty())
	{
		auto& [bid_price, bids] = *m_bids.begin();
		auto& [ask_price, asks] = *m_asks.begin();
		
		if (bid_price >= ask_price)
		{
			while (!bids.empty() && !asks.empty())
			{
				OrderPtr bid = bids.front();
				OrderPtr ask = asks.front();

				Quantity quantity = std::min(bid->getQuantity(), ask->getQuantity());

				bid->fill(quantity);
				ask->fill(quantity);

				// TODO record the trade
				Event event(
					EventType::Match,
					bid,
					ask,
					ask->getPrice(), // orders happen at ask price
					quantity
				);
				m_event_queue.push(event);

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
}

void OrderBook::cancelOrder(OrderId orderId)
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

	Event event(
		EventType::Cancel,
		p_order,
		nullptr,
		p_order->getPrice(),
		p_order->getQuantity()
		);

	m_event_queue.push(event);

	m_orders.erase(orderId);
}

void OrderBook::modifyOrder(OrderId orderId, Price newPrice, Quantity newQuantity, Side newSide)
{
	return;
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
		cancelOrder(orderId);
	}
}

bool OrderBook::popEvent(Event& event)
{
	return m_event_queue.pop(event);
}
