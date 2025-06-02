## 17.9 deduction guide

这里转而看编译器实践

user-defined deduction guide的语法是function with trailing return的声明格式

有两种常见的格式

```cpp
// simple-template-id 形如 templatename<arg1, arg2,...>
explicit (optional) template-name ( parameter-list ) -> simple-template-id ;	(1)	

template <template-parameter-list ﻿> 
explicit (optional) template-name ( parameter-list ) -> simple-template-id ;	(2)	

```

User-defined deduction guides must name a class template and must be introduced within the same semantic scope of the class template (which could be namespace or enclosing class) and, for a member class template, must have the same access, but deduction guides do not become members of that scope.

A deduction guide is not a function and does not have a body. Deduction guides are not found by name lookup and do not participate in overload resolution except for the overload resolution against other deduction guides when deducing class template arguments. Deduction guides cannot be redeclared in the same translation unit for the same class template.


什么时候手写推导指南有用？
当你希望用户直接用某种“构造函数表达式”（如一个 C-数组、一对“指针+长度”组合、一组“特定类型”实参）来构造类模板实例化对象时，
又不想让他们手动写模板实参（如 <int,4>、<double>），
就必须手写一个推导指南，告诉编译器“如何从这些构造实参推导出对应的模板参数”。

```cpp

#include <cstddef>
#include <iostream>

// ———————————— 类模板定义 ————————————
template<typename T, std::size_t N>
struct StaticArray {
    T data[N];

    // 构造函数：接受一个 T[N] 的引用
    StaticArray(const T (&arr)[N]) {
        for (std::size_t i = 0; i < N; ++i) {
            data[i] = arr[i];
        }
    }

    // 打印成员
    void print() const {
        for (std::size_t i = 0; i < N; ++i) {
            std::cout << data[i] << ' ';
        }
        std::cout << '\n';
    }
};

// ———————————— 显式推导指南 ————————————
// 这一行就是重点：教给编译器如何“从 T(&)[N] 推导出 <T, N>”
template<typename T, std::size_t N>
StaticArray(T (&)[N]) -> StaticArray<T, N>;

int main() {
    // 现在，这样就能自动推导：T=int, N=4
    StaticArray arr({ 1, 2, 3, 4 });  
    arr.print();  // 输出：1 2 3 4

    // 还可以直接用一个已有的 C-数组
    int raw[] = { 10, 20, 30 };
    StaticArray arr2 = raw;  
    arr2.print();  // 输出：10 20 30

    // 或者不同类型：
    const char raw2[] = "hi";
    StaticArray arr3 = raw2;  // 推导 T=const char, N=3 （含 '\0'）
    arr3.print();  // 输出：h i \0  （'\0' 可能不显示，但长度为 3）

    return 0;
}
```