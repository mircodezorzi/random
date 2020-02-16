#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>

#include "dz.hpp"

TEST_CASE("Stack") {
	dz::Stack<int> s;
	s.push(3, 1, 2, 4, 7, 5, 9, 6, 8, 0);
	std::stringstream ss;
	s.stream(ss, ", ");
	REQUIRE(ss.str() == "0, 8, 6, 9, 5, 7, 4, 2, 1, 3");
}

TEST_CASE("Queue") {
	dz::Queue<int> q;
	q.push(3, 1, 2, 4, 7, 5, 9, 6, 8, 0);
	std::stringstream ss;
	q.stream(ss, ", ");
	REQUIRE(ss.str() == "3, 1, 2, 4, 7, 5, 9, 6, 8, 0");
}

TEST_CASE("List") {
	dz::List<int> l;
	l.push(3, 1, 2, 4, 7, 5, 9, 6, 8, 0);
	std::stringstream ss;
	l.stream(ss, ", ");
	REQUIRE(ss.str() == "0, 8, 6, 9, 5, 7, 4, 2, 1, 3");
}

TEST_CASE("OrderedList") {
	SECTION("Default Compare") {
		dz::OrderedList<int> l;
		l.push(3, 1, 2, 4, 7, 5, 9, 6, 8, 0);
		std::stringstream ss;
		l.stream(ss, ", ");
		REQUIRE(ss.str() == "0, 1, 2, 3, 4, 5, 6, 7, 8, 9");
	}
	SECTION("Custom Compare") {
		dz::OrderedList<int> l{[](const int &lhs, const int &rhs) { return lhs < rhs; }};
		l.push(3, 1, 2, 4, 7, 5, 9, 6, 8, 0);
		std::stringstream ss;
		l.stream(ss, ", ");
		REQUIRE(ss.str() == "9, 8, 7, 6, 5, 4, 3, 2, 1, 0");
	}
}

TEST_CASE("String") {
	SECTION("Join") {
		std::vector<dz::String> v{"Hello,", "World!"};
		REQUIRE(dz::join(" ", v) == "Hello, World!");
	}

	SECTION("Split") {
		dz::String str{"Hello,  World!"};
		std::vector<dz::String> excepted{"Hello,", "World!"};
		REQUIRE(str.split() == excepted);
	}

	SECTION("Trim") {
		dz::String str{"  Hello, World!   "};
		str.trim();
		REQUIRE(str == "Hello, World!");
	}
}
