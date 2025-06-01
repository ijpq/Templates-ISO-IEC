// main.cpp
#include <iostream>

// 声明 A.cpp 和 B.cpp 中导出的两个函数
extern "C" int call_getValue_A();
extern "C" int call_getValue_B();

int main() {
    int a = call_getValue_A(); // 应该是 42
    int b = call_getValue_B(); // 应该是 99

    std::cout << "A.cpp 中 MyTemplate<int>::value = " << a << std::endl;
    std::cout << "B.cpp 中 MyTemplate<int>::value = " << b << std::endl;

    // 如果两个单元中的“相同名字模板特化”并非相互独立，
    // 那么这里的输出不会同时是 42 和 99。但由于它们都在匿名命名空间，属于各自内部链接，
    // 最终输出应当分别是 42 和 99，互不干扰。
    return 0;
}
