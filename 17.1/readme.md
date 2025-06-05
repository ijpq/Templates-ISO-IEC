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
    // typename C中, C一般应该省略. 因为template<typename C> typename T 表明类型T是一个模板,而不是一个具体的类型. 并且,模板类型T中的模板参数C并无法被使用. 只起到占位和匹配的意图,就是告诉编译器模板类型T是接受一个模板参数的模板类型
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


## 5

对于非类型模板参数,顶层的cv限定符会被忽略
```cpp

namespace B {
    // 第二个声明是对同一个模板的定义，ok
    template<int M>
    struct Example;

    template<int N>
    struct Example {};
}

namespace C{
    // 由于同一命名空间中，模板名称是唯一的，因此第二个声明是重复声明，error
    template<void(*f)()>
    struct Example;
    
    template<int M>
    struct Example;
}

namespace A{
    template<const int N>
    struct Example;
    
    // 第二次声明：定义 “template<int N>”
    // 这行并不会被当作“新模板”，而是跟上面那条合并为“同一个模板”的声明。视为对同一个模板的定义
    template<int N>
    struct Example {
        static constexpr int value = N;
    };
}

/*
<source>:16:23: error: template parameter 'void (* f)()'
   16 |     template<void(*f)()>
      |                       ^
<source>:19:18: note: redeclared here as 'int M'
   19 |     template<int M>
      |                  ^
Compiler returned: 1
*/
```

## 6
非类型非引用模板参数是纯右值,不能被赋值,不能修改它的值,不能取地址.

## 7
非类型参数不能使用浮点数/类型/void进行声明

> 原因在于，类类型的对象通常在编译期无法彻底归一化为字面常量——它可能包含非静态数据成员、虚函数表指针、对齐规则、构造／析构逻辑等，编译器很难（或不允许）将其作为一个单一的整型／指针那样的“模板参数值”进行比较和区分。
```cpp
template<float  F> struct A1 {};   // ❌ 编译错误：浮点类型不能做非类型模板参数
template<double D> struct A2 {};   // ❌ 同上
template<long double L> struct A3 {}; // ❌ 同上

struct Point {  // 普通的类类型
    int x, y;
};

// 错误示例：以下在 C++17 里都会被拒绝
template<Point P> struct B1 {};    // ❌ 报错：Point 不是允许的非类型模板参数类型
template<const Point P> struct B2 {}; // ❌ 即使加 const 也不行

template<void V> struct C1 {};   // ❌ 报错：void 不是允许的非类型模板参数
template<void* Ptr> struct C2 {}; // ok

/*
void* 虽然 “指向的对象类型未知”，但它依然是“一个对象指针类型”（pointer to object），编译器在编译期可以把它当作常量（例如 nullptr 或者某个全局 void* 变量的地址）进行处理，所以也被标准允许。
*/

```


## 8

在非类型模板参数中,如果该参数的类型是array of T, 或者是函数类型,那么都会被自动转换为指向T的指针.

```cpp
namespace A {
    template<int Func(int)>
    struct X2;

    template<int (*f)(int)> 
    struct X2 {};

    template<int* p>
    struct X3;

    template<int m[]> 
    struct X3 {};
}
```

在这个代码中,展示了该规则是有效的,因为只有被视为同一个模板的补充定义,才会通过编译.

## 9

类型/非类型/模板作为模板参数时,只有不是参数包,都可以使用参数包.

默认模板参数可以在模板声明时指定.

默认模板参数不能在模板类外定义成员时使用

```cpp
// 默认模板参数不能在模板类外定义成员时使用
template<typename T=int>
struct A{
    void func();
};

template<typename T>
void A<T>::func() {}

template<typename T=int>
struct B{
    void func();
};

template<typename T=int> // 此处不能使用默认参数
void B<T>::func() {}

/*
<source>:19:17: error: default argument for template parameter for class enclosing 'void B<T>::func()' [-Wtemplate-body]
   19 | void B<T>::func() {}
      |                 ^
*/
```

## 10

当同一个模板在程序中出现多次声明（前向声明或分开写的声明/定义）时，所有这些声明中为模板参数给出的默认实参会被“合并”起来，就像函数的默认参数那样。编译器会从最早的声明开始，依次收集、补全每个参数的默认值。

```cpp
// 第一次声明：为 T 提供默认参数 int，为 U 暂时不写
template<typename T = int, typename U>
struct X;
// 第二次声明：补全 U 的默认参数为 double
template<typename T, typename U = double>
struct X {
    // 这既是“声明”，又是“定义”
};
/*
编译器首先看到第一个声明 template<typename T = int, typename U> struct X;，就记录下来：T 的默认是 int，U 还没有默认。

当它再看到第二个声明 template<typename T, typename U = double> struct X 时，它会发现“T 在之前已经有默认了（int），因此不能再改；U 之前没有默认，这里给出了默认 double，就把 U 的默认设为 double”。
*/

template<typename T = int, typename U = double> struct X { … };
```


## 11

1. 提供默认参数的模板参数之后,都需要有默认参数,或者模板参数是参数包
```cpp
// 对于类模板,变量模板,别名模板
template<typename T = int , typename U>
struct A;
template<typename T = int , typename U = long>
struct B;
template<typename T = int , typename ...U>
struct C;
/*

<source>:6:8: error: no default argument for 'U'
    6 | struct A;
      |        ^
*/
```

2. 参数包必须是parameter-list中的最后一个模板参数
```cpp
// 对于主类模板,主变量模板,别名模板
template<typename ...U>
struct C;
template<typename ...U, typename T>
struct B;
template<typename ...U, typename T=int>
struct A;
template<typename ...U, typename ...T>
struct D {};
/*
<source>:8:10: error: parameter pack 'U' must be at the end of the template parameter list
    8 | template<typename ...U, typename T>
      |          ^~~~~~~~
<source>:10:10: error: parameter pack 'U' must be at the end of the template parameter list
   10 | template<typename ...U, typename T=int>
      |          ^~~~~~~~
<source>:24:22: error: template parameter pack must be the last template parameter
   24 | template<typename ...U, typename ...T>
      |                      ^
*/
```

3. 对于函数模板,也适用规则2的要求,除非参数包后面的模板参数可以被函数的参数列表推导出来,或者具有默认值

```cpp
struct A{
    A(int a) {}
};

template<typename ...U>
void func1(U&&... u) {}; // 参数包是最后一个模板参数，不存在问题

template<typename ...U, typename T>
void func2() {}; // 不符合标准，因为无法从**函数的参数列表**中推导

template<typename ...U, typename T>
T func3() {return T{0};}; // 不符合标准，因为无法从**函数的参数列表**中推导

template<typename ...U, typename T>
void func4(U&&... u, T) {}; // 符合标准的做法：在参数包后面的模板参数可以从 函数的参数列表 中推导；但参数包会吸很多参数

template<typename ...U, typename T>
void func4(T, U&&... u) {}; // 符合标准的做法：在参数包后面的模板参数可以从 函数的参数列表 中推导；

template<typename ...U, typename T = A>
T func5(U&&... u) {return T{0};}; // 符合标准的做法：在参数包后面的模板参数有默认值

int main () {

func1(1);

func2<A>();

A a2 = func3<A>();

A a(99);
func4(a); 

func5(1);
}
```

### C++17引入的模板参数deduction guide

见17.9

## 12

不能在两个不同的声明中给出模板参数的默认值

## 13
关于在parameter list中使用`>`
当该符号不是list的结尾时，加个括号


## 14
在parameter-list中使用模板时，提供默认值的限制。有些复杂..

## 15

1. 任何在参数列表里带有 前置`...` 的模板参数都称为“参数包”。

2. 如果一个模板参数包本身的 声明 使用了另一个“未展开的”参数包＋...，那么它就是一个“包扩展”形式。

3. 参数包不能在同一个parameterlist中进行声明的同时，进行展开

```cpp
template <class... Types> class Tuple; // Types is a template type parameter pack
// but not a pack expansion
template <class T, int... Dims> struct multi_array; // Dims is a non-type template parameter pack
// but not a pack expansion
template<class... T> struct value_holder {
template<T... Values> struct apply { }; // Values is a non-type template parameter pack
// and a pack expansion
};
template<class... T, T... Values> struct static_array;// error: Values expands template type parameter
// pack T within the same template parameter list
```

template<typename ...U, typename T = A>
T func5(U&&... u) {return T{0};}; // 符合标准的做法：在参数包后面的模板参数有默认值

int main () {

func1(1);

func2<A>();

A a2 = func3<A>();

A a(99);
func4(a); 

func5(1);
}
```

### C++17引入的模板参数deduction guide

见17.9

## 12

不能在两个不同的声明中给出模板参数的默认值

## 13
关于在parameter list中使用`>`
当该符号不是list的结尾时，加个括号


## 14
在parameter-list中使用模板时，提供默认值的限制。有些复杂..

## 15

1. 任何在参数列表里带有 前置`...` 的模板参数都称为“参数包”。

2. 如果一个模板参数包本身的 声明 使用了另一个“未展开的”参数包＋...，那么它就是一个“包扩展”形式。

3. 参数包不能在同一个parameterlist中进行声明的同时，进行展开

```cpp
template <class... Types> class Tuple; // Types is a template type parameter pack
// but not a pack expansion
template <class T, int... Dims> struct multi_array; // Dims is a non-type template parameter pack
// but not a pack expansion
template<class... T> struct value_holder {
template<T... Values> struct apply { }; // Values is a non-type template parameter pack
// and a pack expansion
};
template<class... T, T... Values> struct static_array;// error: Values expands template type parameter
// pack T within the same template parameter list
```