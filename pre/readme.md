## 说明

implicit instantiation(有时也称为implicit specialization) 
 >模板在真正使用的时候，隐式地自动生成一个对应的实例（这也称为特化版本）。

 explicit specialization
> template <>声明的版本，以替代隐式实例化生成的版本。

explicit instantiation

## 1 
A declaration introduced by a template declaration of a variable is a variable template. 
```cpp 
template <typename T>
T a = T(1);
```
A variable template at class scope is a static data member template.



```cpp 
struct A {
    template <typename T> static T a = 1;
};

```

## 2

一个模板声明只能出现在namesapce/类作用域中
```cpp 
namespace {
    template <typename T> void f() {
    } // ✅ namespace scope
}

struct A {
    template <typename T> void f() {
    } // ✅ class scope
};

void g() {
    template <typename T> void h(); // ❌ 错误，不能在函数体内写 template-declaration
}
```

在函数模板声明中，declarator-id的最后部分不能是template-id

```cpp 
template <typename T>
void f(); // ✅ 这里 declarator-id 是 f

// error: template-id 'f<int>' in declaration of primary template
template <typename T> 
void f<int>(); // 错误：f<int> 是模板-id，而函数模板声明不允许这样写
```

在类模板声明中，如果类名是一个simple-template-id，那么这个声明就是一个模板特化声明 

A simple-template-id consists of a template name followed by a template-argument-list enclosed in angle brackets 

```cpp 
TemplateName<Arg1, Arg2, ..., ArgN>
```

## 3

## 4

模板名称有链接性，具有内部链接属性的全特化和隐式实例化的模板，那么它的所有特化（无论是显式写出的还是编译器隐式实例化的）在当前翻译单元里都是彼此独立的，不会与其他翻译单元中同名模板的特化发生冲突

在示例代码中，分别在两个翻译单元中都定义了具有内部链接属性的类模板MyTemplate，并且分别隐式实例化了T=int，但这两个同名模板并不会冲突

## 5 
同一个类scope/命名空间中，模板名是唯一的。
```cpp
    /*
    <source>:11:8: error: redefinition of 'struct B::A<T>'
       11 | struct A{};
          |        ^
    <source>:8:8: note: previous definition of 'struct B::A<T>'
        8 | struct A{};
          |        ^
    <source>:13:8: error: 'void B::A()' redeclared as different kind of entity
       13 | void A() {}
          |        ^
    <source>:8:8: note: previous declaration 'template<class T> struct B::A'
        8 | struct A{};
          |        ^
    <source>:15:5: error: 'int B::A' redeclared as different kind of entity
       15 | int A = 1;
          |     ^
    <source>:8:8: note: previous declaration 'template<class T> struct B::A'
        8 | struct A{};
          |        ^
    <source>:17:8: error: class template 'A' redeclared as non-template
       17 | struct A{};
          |        ^
    <source>:8:8: note: previous declaration here
        8 | struct A{};
          |        ^
    */
    namespace B {
    template <typename T> struct A {};

    template <typename T> struct A {};

    void A() {
    }

    int A = 1;

    struct A {};
}
```

但是模板函数可以被同名的非模板函数、模板函数重载

```cpp 
namespace B {
    template <typename T> void func() {
    }

    template <typename T> void func(T t) {
    }

    void func() {
    }
}
```

在一个类scope、命名空间中的模板名字是唯一的

## 6 
template entity的定义

## 7

在被隐式实例化的翻译单元中 
1. function template
2. member function of a class template 
3. variable template 
4. static data member of a class template

这些名称要可见定义。

> 方案一：如果你要在某个翻译单元（TU）里对某个模板（函数模板、类模板的成员函数、变量模板或类模板的静态成员等）进行隐式实例化，那个翻译单元就必须能“看到”该模板的定义。
> 
> 方案二：如果你不想让每个 TU 都包含模板定义，又仍想在编译链接时成功得到某个特化的实现，那么就必须“在某个 TU 中对这个特化做显式实例化”，让它在生成的 .o（符号表）里先行产生一份可供链接的符号。