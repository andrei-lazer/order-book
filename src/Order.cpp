#include "Order.hpp"
#include <stdexcept>


Order::Order(OrderId orderId, Side side, Price price, Quantity quantity, OrderType orderType) :
	m_orderId(orderId), m_side(side), m_price(price), m_quantity(quantity), m_order_type(orderType)
{
}

void Order::fill(Quantity quantity)
{
	if (quantity > m_quantity)
	{
		throw std::runtime_error("Quantity must be less than remaining.");
	}

	m_quantity -= quantity;
}

