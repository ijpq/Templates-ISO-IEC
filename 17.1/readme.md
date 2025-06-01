## 17.1 模板参数


## 2

unqualified-id: 纯粹标识符，没有任何作用域前缀

qualified-id: 带有嵌套名限定符`T::`

typename后面是unqualified-id时，声明了类型参数。后面是qualified-id时，声明非类型参数。

```cpp
template<typename T, typename T::value i>
T func() {
    cout << i << endl;
}
```

## 3
没有跟省略号的类型参数的标识符，把该标识符定义为一个类型(当这个标识符没有使用template进行声明时)，或者一个模板名（使用template声明时）

```cpp
template<typename T>
struct Example {
    // 在这里，T 就相当于 “typedef T” 在模板内部被定义了一次
    T a = 1;
};

template< template<typename C> typename T>
struct Example2 {
    // 类型参数仍然使用typename声明，但是当标识符是一个模板名时，再使用template声明
    T<int> a;
};

```

## 4
非类型模板参数可以使用以下几种类型：

1. 整型、枚举

2. 对象指针、函数指针

3. 对象的左值引用、函数的左值引用

4. 成员的指针

5. std::nullptr_t

```cpp
// 对象指针、函数指针
// 示例 2.1：全局变量作为对象；用它的地址做模板参数
int global_value = 42;

template<int* P>
struct ValueHolder {
    static int get() { return *P; }
};

ValueHolder<&global_value> vh1;  

// 示例 2.2：全局函数作为函数指针；用它的地址做模板参数
int doubleValue(int x) {
    return x * 2;
}

template<int (*Func)(int)>
struct FuncApplier {
    static int apply(int x) { return Func(x); }
};

FuncApplier<&doubleValue> fa1;  
int main () {
    auto global_get = ValueHolder<&global_value>::get();
    auto double_get = FuncApplier<&doubleValue>::apply(5);
    cout << "global get: " << global_get << endl;
    cout << "double get: " << double_get << endl;
}

/*
Program returned: 0
global get: 42
double get: 10
*/
```

&Point::x 本身就是一个“指针到成员”的常量，和具体对象无关

写 int Point::* pm = &Point::x; 时，pm 已经包含了“在 Point 类型内部，成员 x 的偏移量”这类信息；

这个偏移量对任何 Point 实例都适用，不需要知道是哪个具体对象才能决定“指向哪个成员”。

**换句话说，指针到成员不是指向某个对象的地址，而是一种在编译期就能确定的“成员标识”——它告诉编译器“你以后在某个 Point 对象里，用这个偏移量找到 x”。**

如果写 &p.x，得到的是“普通指针”而不是“指针到成员”

假设有 Point p;，表达式 &p.x 的类型是 int*，它表示“取对象 p 中 x 成员在内存中的地址”。**这个地址是运行期才确定的，也不能用作模板参数（模板参数必须是编译期常量）。**

但 &Point::x 的类型正是 int Point::*，编译期就能知道“在 Point 中，x 的偏移是多少”，因此可以成为非类型模板参数。
```cpp
// 示例 4.1：指向成员变量的指针
struct Point {
    int x, y;
};

template<int Point::*MemberPtr>
struct MemberPrinter {
    static int get(const Point& p) {
        return p.*MemberPtr;  // 用 .* 运算符访问成员
    }
};


Point pt{7, 9};


// 示例 4.2：指向成员函数的指针
struct Greeter {
    int value;
    void greet() const {
        cout << "calling greet()" << endl;
    }
};

template<void (Greeter::*MemFn)() const>
struct MethodCaller {
    static void call(const Greeter& g) {
        (g.*MemFn)();  // 调用成员函数
    }
};

MethodCaller<&Greeter::greet> mc1;
// 然后你可以： Greeter g{42}; MethodCaller<&Greeter::greet>::call(g);



int main () {
int v = MemberPrinter<&Point::x>::get(pt);
cout << v ;

Greeter g;
MethodCaller<&Greeter::greet>::call(g);
}

```