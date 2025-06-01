// B.cpp
#include <iostream>

// 这里同样把模板放在匿名命名空间内。
// 虽然名字与 A.cpp 中相同，但因为处于各自的匿名命名空间，它们是两个独立实体。
namespace {
    template<typename T>
    struct MyTemplate {
        static int value;
    };

    template<typename T>
    int MyTemplate<T>::value = 0;

    // 本单元中，将 MyTemplate<int>::value 置为 99
    void initB() {
        MyTemplate<int>::value = 99;
    }

    // 返回本单元里 MyTemplate<int>::value
    int getValueB() {
        return MyTemplate<int>::value;
    }
}

// 对外暴露的函数
extern "C" int call_getValue_B() {
    initB();              // 先把本单元里 MyTemplate<int>::value 设为 99
    return getValueB();   // 返回 99
}
