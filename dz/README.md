## Dynamic Containers

```c++
dz::Stack<int> s;
s.push(3, 1, 2, 4, 7, 5, 9, 6, 8, 0);
s.map([](const int &i){ i = i * i; });
s.print();
```

### Complex data structures and containers

```c++
struct Foo {
	int bar;
	float baz;
};
	
dz::OrderedList<Foo> f_o{
	[](const Foo &lhs, const Foo &rhs) { return lhs.bar > rhs.bar;  },
	[](const Foo &lhs, const Foo &rhs) { return lhs.bar == rhs.bar; }
};
```

## I/O

```c++
dz::String str = dz::input("> ");
dz::print(str);
```

### String manipulation

``` c++
dz::print(str.split("\t"));
dz::print(str.replace(" ", ","));
dz::print(dz::join(" <--> ", str.split(" ")));
dz::print(dz::input("> ").split(",").index("hello"));
```

### Printing STL containers

``` c++
using std::map;
using std::vector;

dz::print(map<int, int>{{5, 6}, {7, 9}, {9, 7}});
dz::print(vector<vector<int>>{{5, 9}, {8, 5, 9, 8}});
dz::print(vector<map<int, int>>{{{5, 9}, {8, 9}}});
dz::print(vector<map<dz::String, int>>{{{"foo", 9}, {"bar", 9}}});
```
