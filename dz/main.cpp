// include entire dz library
#include "dz.hpp"

int main()
{
	using namespace dz::containers; // Stack, Queue, List, OrderedList
	using namespace dz;             // Tree
	using std::cout;
	using std::endl;

	cout << "pushing (3, 1, 2, 4, 7, 5, 9, 6, 8, 0) into containers" << endl;

	Stack<int> s;
	s.push(3, 1, 2, 4, 7, 5, 9, 6, 8, 0);

	cout << "stack: ";
	s.map([](int i) { cout << i << ' '; });
	cout << endl;

	Queue<int> q;
	q.push(3, 1, 2, 4, 7, 5, 9, 6, 8, 0);

	cout << "queue: ";
	q.map([](int i) { cout << i << ' '; });
	cout << endl;

	List<int> l1;
	l1.push(3, 1, 2, 4, 7, 5, 9, 6, 8, 0);

	cout << "list: ";
	l1.map([](int i) { cout << i << ' '; });
	cout << endl;

	OrderedList<int> l2{};
	l2.push(3, 1, 2, 4, 7, 5, 9, 6, 8, 0);

	cout << "ordered list: ";
	l2.map([](int i) { cout << i << ' '; });
	cout << endl;

	OrderedList<int> l3{[](const int &lhs, const int &rhs) { return lhs < rhs; }};
	l3.push(3, 1, 2, 4, 7, 5, 9, 6, 8, 0);

	cout << "ordered list (custom compare): ";
	l3.map([](int i) { cout << i << ' '; });
	cout << endl;

	Tree<int> t;
	t.push(10, 5, 20, 25, 60, 15, 70, 55, 100, 45, 65, 85, 90);

	t.stream(cout, dz::DisplayMethod::InOrder);
	cout << endl;

	t.map([](int &i) { if(!(i % 10)) i *= 2; });

	t.sort();

	t.stream(cout, dz::DisplayMethod::InOrder);
	cout << endl;

	t.erase(20);
	t.erase(45);
	t.erase(15);
	t.erase(15);
	t.erase(55);
	t.erase(5);

	t.stream(cout, dz::DisplayMethod::InOrder);
	cout << endl;

	return 0;
}
