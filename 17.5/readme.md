##

### 1

`template-id`是`template-name`和`template-argument-list`组成的声明

`template-argument-list`在主模板声明中不能使用, 在特化声明中可以使用

### 2

从`name lookup`和`实例化`的角度来讲(需要知道这两个概念), 函数模板、类模板中的成员函数声明如果带有默认参数和`noexcept`被认为是定义

每一个默认参数和`noexcept`声明符是一个独立的定义，这个定义和函数模板的定义无关，和其他的默认参数和`noexcept`声明符无关.

从实例化的角度来讲, `constexpr if`生效的子句被认为是一个定义，而没有生效的子句不被认为是一个定义

```cpp
template<typename T>
void func1(); // 声明

template<typename T>
void func1() {} // 函数模板定义

template<typename T>
void func2(); // 声明

template<typename T>
void func2(); // 还是声明

template<typename T=int> // 模板参数T的定义
void func3();

template<typename T=int> // 模板参数T的重定义
void func3();

/*
<source>:20:10: error: redefinition of default argument for 'class T'
   20 | template<typename T=int> // 模板参数T的重定义
      |          ^~~~~~~~
<source>:17:10: note: original definition appeared here
   17 | template<typename T=int> // 模板参数T的定义
      |          ^~~~~~~~
*/

```

### 3

因为一个别名的声明并没有声明一个`template-id`, 所以一个别名模板不能特化

```cpp
/*
<source>:30:8: error: specialization of alias template 'template<class T> using ALIAS = A<T>'
   30 | struct ALIAS<long> {};
      |        ^~~~~~~~~~~
*/
template<typename T>
struct A{};

template<typename T>
using ALIAS = A<T>;

template<>
struct ALIAS<long> {};

template<>
struct A<int> {};
```

这样的“显式特化”或“部分特化”。编译器根本不把 ALIAS 当作一个独立的“可特化的模板”，而只是每次在编译期把 ALIAS<int> 直接展开成 A<int>。


## 17.5.1

### 3
对于

1. 成员函数

2. 成员类

3. 成员枚举

4. 静态数据成员

5. 模板成员

在类模板的定义之外进行定义时，模板参数就是定义模板类时的模板参数。使用的模板参数名称可不同。但进行定义时的argument-list中参数的顺序应该与parameter-list的顺序一致.
在argument-list中的参数包应该进行展开

## 17.5.1.1
模板类的成员函数的声明方式

## 17.5.1.2
类模板的成员类的声明方式

注意提供成员类的定义
## 17.5.1.3
模板类的静态数据成员的声明方式

## 17.5.1.4
模板类的枚举成员的声明方式

## 17.5.2

成员模板: 在类或类模板中声明的模板

成员模板可以在类、类模板中定义，或在定义外进行定义

### 1
类模板的成员模板在类外定义时，应该先声明类模板的模板参数，然后声明成员模板的模板参数

```cpp
template<typename T1, typename T2>
struct Class {
    void memberFunc();
    static int a;

    template<typename U1, typename U2>
    pair<U1,U2> templateMemberFunc();

    enum RGB {x,y,z};
    RGB rgb;
    enum class RGB2;
};

template<typename U1, typename U2>
template<typename U3, typename U4>
pair<U3,U4> Class<U1,U2>::templateMemberFunc() {
    cout << "template member func" << endl;
    return make_pair(U3{0}, U4{0});
}
```

### 2

当模板成员函数和非模板成员函数的函数签名相同时，可以同时在类中进行这样的声明。当产生调用时，即便模板参数实例化后的成员函数与非模板成员函数完全一致，那么会优先调用非模板成员函数，除非进行显示调用模板成员函数

## 17.5.3 

可变模板

### 1 & 2 & 3

参数包是接受0和或任意数量argument的 parameter

只存在模板函数的参数包和类模板的参数包

### 4

`参数包展开`由`模式`和`...`组成，参数包的实例化产生0个或者任意数量个`模式`的实例。

模式的格式取决于展开发生的上下文:

**函数参数包**：模式 = 参数声明（去掉 ...）。

**using-declarator**：模式 = using Base<...>::member 中的 Base<...>::member。

**模板参数包（参数声明版）**：模式 = 参数声明（去掉 ...）。

**模板参数包（类型参数版）**：模式 = template<...> class Foo 中的 template<...> class Foo（去掉 ...）。

**初始化列表**：模式 = 初始化子句（initializer-clause）。

**基类说明符列表**：模式 = Base<T> 这样的基类说明符（去掉 ...）。

**成员初始化列表中的基类初始化**：模式 = Base(args)。

**模板实参列表**：模式 = 单个模板实参（template-argument）。

**属性列表**：模式 = 单个属性（attribute）。

**对齐说明符**：模式 = 单个对齐值（alignment-specifier，去掉 ...）。

**捕获列表**：模式 = 单个捕获子句（capture）。

**sizeof... 表达式**：模式 = 参数包名本身（identifier）。

**fold-expression**：模式 = 包含未展开参数包的表达式（cast-expression）。