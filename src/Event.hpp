#include "Order.hpp"

enum class EventType {Add, Cancel, Match};

struct Event
{
	EventType m_type;
	OrderPtr mp_order;
	OrderPtr mp_match; // for matches - null if not a match
	Price m_price;
	Quantity m_quantity;
};
