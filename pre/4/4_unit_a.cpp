#include <iostream>

// 放在匿名命名空间里，模板及其所有特化都会具有“内部链接”
// （相当于 static，确保本翻译单元内可见，外部不可见）
namespace {
    // 这是一个简单的类模板，模板名为 MyTemplate<T>。
    template <typename T>
    struct MyTemplate {
        // 用一个静态成员来记录值
        static int value;
    };

    // 定义静态成员变量的“零初始化”。
    template <typename T>
    int MyTemplate<T>::value = 0;

    // 本单元中，将 MyTemplate<int>::value 置为 42
    void initA() {
        MyTemplate<int>::value = 42;
    }

    // 返回本单元里 MyTemplate<int>::value
    int getValueA() {
        return MyTemplate<int>::value;
    }
} // namespace

// 下面这两个函数对外可见（有外部链接），
// main.cpp 可以通过这两个函数来访问 A.cpp 中的模板实例化结果。
extern "C" int call_getValue_A() {
    initA();            // 先把本单元里 MyTemplate<int>::value 设为 42
    return getValueA(); // 返回 42
}
