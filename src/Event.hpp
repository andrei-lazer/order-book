#include "Order.hpp"

struct Event
{
	enum class Type {Add, Cancel, Match};
	OrderId order_id;
	Side side;
	Price price;
	Quantity quantity;
};
