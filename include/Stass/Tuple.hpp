#pragma once

#include "Main.hpp"

namespace Stass {
	// A helper struct which contains a type of an item by index in tuple
	template <int index, typename Head, typename... Tail>
	struct TupleElement {
		using Type = typename TupleElement<index - 1, Tail...>::Type;
	};

	// A helper struct which contains a type of the first item in tuple
	template <typename Head, typename... Tail>
	struct TupleElement<0, Head, Tail...> {
		using Type = Head;
	};

	// A tuple declaration
	template <typename... Types>
	struct Tuple;

	template <>
	struct Tuple<> {};

	// Returns an item of certain tuple by index
	template <int index, typename... Types>
	auto getTupleItem(Tuple<Types...>& tuple) -> typename TupleElement<index, Types...>::Type& {
		if constexpr (index == 0)
			return tuple.firstItem;
		else
			return getTupleItem<index - 1>(static_cast<Tuple<Types...>&>(tuple));
	}

	// A tuple implementation
	template <typename Head, typename... Tail>
	struct Tuple<Head, Tail...> : Tuple<Tail...> {
		// The first item of the tuple
		Head firstItem;

		// Basic constructor
		Tuple() : firstItem(), Tuple<Tail...>() {};

		// Copy constructor
		Tuple(Tuple& other) : firstItem(other.firstItem), Tuple<Tail...>(static_cast<Tuple<Tail...>&>(other)) {};

		// Item constructor
		Tuple(const Head& head, const Tail&... tail) : firstItem(head), Tuple<Tail...>(tail...) {};

		// Returns an item of tuple by index
		template <int index>
		auto getItem() {
			return getTupleItem<index>(*this);
		}

		// Basic asign operator
		Tuple& operator =(Tuple& other) {
			if constexpr (sizeof...(Tail) == 0) 
			{
				firstItem = other.firstItem;
			}
			else 
			{
				firstItem = other.firstItem;

				Tuple<Tail...>::operator=(static_cast<Tuple<Tail...>&>(other));
			}

			return *this;
		}
	};
}