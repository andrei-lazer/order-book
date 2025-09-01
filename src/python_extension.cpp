#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/functional.h>
#include "OrderBook.hpp"

namespace py = pybind11;

PYBIND11_MODULE(OrderBookPy, m) {
	m.doc() = "Limit order book in C++";
	py::class_<OrderBook>(m, "OrderBook")
		.def(py::init<>())
		.def("placeOrder", &OrderBook::placeOrder)
		.def("deleteOrder", &OrderBook::deleteOrder)
		.def("modifyOrder", &OrderBook::modifyOrder)
		.def("getLevels", &OrderBook::getLevels);


	py::class_<Trade>(m, "Trade")
		.def(py::init<)
}


