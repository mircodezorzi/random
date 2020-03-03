#include "dz.hpp"

#define COMPARE_STRUCTS(type, operator, attribute) \
	[](const type &lhs, const type &rhs) { return lhs.attribute operator rhs.attribute; }

#define STRUCT_LIST(type, name, operator, attribute)	\
	dz::List<type> name { COMPARE_STRUCTS(type, operator, attribute) }

#define STRUCT_ORDERED_LIST(type, name, operator, attribute)	\
	dz::OrderedList<type> name { COMPARE_STRUCTS(type, operator, attribute) }

int main()
{
	using std::cout;
	using std::endl;

	dz::Stack<int> s;
	s.push(5, 6, 7, 8, 9);
	s.print();

	dz::Queue<int> q;
	q.push(5, 6, 7, 8, 9);
	q.print();

	dz::List<int> l;
	l.push(5, 6, 7, 8, 9);
	l.print();

	dz::OrderedList<int> o;
	o.push(5, 6, 7, 8, 9);
	o.print();

	for (auto &i : s) i *= 2;
		s.print();
	for (auto &i : q) i *= 2;
		q.print();
	for (auto &i : l) i *= 2;
		l.print();
	for (auto &i : o) i *= 2;
		o.print();

	struct Foo {
		int bar;
		float baz;
	};

	dz::Stack<Foo> f_s;

	dz::Queue<Foo> f_q;

	// dz::List<Foo>  f_l{
	// 	[](const Foo &lhs, const Foo &rhs) { return lhs.bar == rhs.bar; }
	// };

	// dz::List<Foo>  f_l{
	// 	COMPARE_STRUCTS(Foo, ==, bar)
	// };

	STRUCT_LIST(Foo, f_l, ==, bar);

	dz::OrderedList<Foo> f_o{
		[](const Foo &lhs, const Foo &rhs) { return lhs.bar > rhs.bar;  },
		[](const Foo &lhs, const Foo &rhs) { return lhs.bar == rhs.bar; }
	};

	dz::BST<int> t;
	t.push(5, 6, 7, 8, 9, 1);
	t.print();

	dz::BST<int>::Iterator it = t.begin();
	while (it != t.end()) {
		std::cout << "\n" << *it;
		++it;
	}

	cout << endl;

	dz::Vector<dz::String> hotels   = dz::input("enter hotels separated by comma: ").split(",");
	dz::print(hotels);

	dz::Vector<dz::String> services = dz::input("enter services separated by comma: ").split(",");
	dz::print(services);

	dz::print(dz::input("> ").split(" ").index("hello"));

	dz::print(std::map<int, int>{{5, 6}, {7, 9}, {9, 7}});
	dz::print(std::vector<std::vector<int>>{{5, 9}, {8, 5, 9}});
	dz::print(std::vector<std::map<int, int>>{{{5, 9}, {8, 9}}});
	dz::print(std::vector<std::map<dz::String, int>>{{{"foo", 9}, {"bar", 9}}});
}
