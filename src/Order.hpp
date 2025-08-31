#pragma once

#include <cstdint>
#include <functional>
#include <iostream>
#include <map>
#include <string>
#include <memory>
#include <vector>
#include <list>

// aliases are used for brevity, clarity,
// and ease of changing

using Price = std::int32_t;
using Quantity = std::uint32_t;
using OrderId = std::uint32_t;

enum class Side
{
	BID,
	ASK
};

class Order
{
	OrderId m_orderId;
	Side m_side;
	Price m_price;
	Quantity m_quantity;
public:
	Order(OrderId orderId, Side side, Price price, Quantity quantity);
	OrderId getOrderId() const { return m_orderId; };
	Side getSide() const { return m_side; };
	Price getPrice() const { return m_price; };
	Quantity getQuantity() const { return m_quantity; };
	bool isEmpty() const { return getQuantity() == 0; }
	void fill(Quantity quantity);
};

// used for pointing to orders
using OrderPtr = std::shared_ptr<Order>;
using OrderPtrList = std::list<OrderPtr>;
using OrderList = std::list<Order>;
