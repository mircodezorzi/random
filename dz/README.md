## Dynamic Containers

```c++
dz::Stack<int> s;
s.push(3, 1, 2, 4, 7, 5, 9, 6, 8, 0);
s.map([](const int &i){ i = i * i; });
s.print();
```

## `dz::String` and `dz::print`
```c++
dz::String str = dz::input("> ");
dz::print(str.split("\t"));
dz::print(str.replace(" ", ","));
dz::print(dz::join(" <--> ", str.split(" ")));
```
