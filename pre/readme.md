## 说明
implicit instantiation (有时也称为implicit specialization)
> 模板在真正使用的时候，隐式地自动生成一个对应的实例（这也称为特化版本）。

explicit specialization
> template<>声明的版本，以替代隐式实例化生成的版本。

explicit instantiation

## 1
A declaration introduced by a template declaration of
a variable is a variable template. 
```cpp
template<typename T>
T a = T(1);
```

A variable template at class scope is a static data member template.



```cpp
struct A{
    template<typename T>
    static T a = 1;
};

```


## 2

一个模板声明只能出现在namesapce/类作用域中
```cpp
namespace {
template<typename T>
void f() {} // ✅ namespace scope
}

struct A {
    template<typename T>
    void f() {} // ✅ class scope
};

void g() {
    template<typename T> void h(); // ❌ 错误，不能在函数体内写 template-declaration
}
```

在函数模板声明中，declarator-id的最后部分不能是template-id

```cpp
template<typename T>
void f(); // ✅ 这里 declarator-id 是 f

// error: template-id 'f<int>' in declaration of primary template
template<typename T>
void f<int>(); // 错误：f<int> 是模板-id，而函数模板声明不允许这样写
```

在类模板声明中，如果类名是一个simple-template-id，那么这个声明就是一个模板特化声明
>A simple-template-id consists of a template name followed by a template-argument-list enclosed in angle brackets (< and >).simple-template-id 
```cpp
TemplateName<Arg1, Arg2, ..., ArgN>
```

## 3


## 4

模板名称有链接性，具有内部链接属性的全特化和隐式实例化的模板，那么它的所有特化（无论是显式写出的还是编译器隐式实例化的）在当前翻译单元里都是彼此独立的，不会与其他翻译单元中同名模板的特化发生冲突

在示例代码中，分别在两个翻译单元中都定义了具有内部链接属性的类模板MyTemplate，并且分别隐式实例化了T=int，但这两个同名模板并不会冲突