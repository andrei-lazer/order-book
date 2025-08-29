#include <cstdint>
#include <functional>
#include <map>
#include <string>
#include <memory>
#include <vector>
#include <list>

// aliases are used for brevity, clarity,
// and ease of changing

using Price = std::int32_t;
using Quantity = std::uint32_t;
using OrderId = std::string;

enum class Side
{
	Buy,
	Sell
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
	void fill(Quantity quantity);
};

using pOrder = std::shared_ptr<Order>;
using OrderList = std::list<Order>;

class Trade
{

};

class OrderBook
{
	struct Entry
	{
		pOrder mp_order;
		OrderList::iterator m_position;
	};

	std::map<Price, Entry, std::greater<Price>> bids;
};
