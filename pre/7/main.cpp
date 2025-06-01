#include "template.h"

int main() {
    // dummyFunc和memberFunc虽然在当前翻译单元看不到定义，但是可以在链接过程中找到符号
    // 或者，在当前翻译单元对其定义
    dummyFunc(99);
    Dummy<int> dummy;
    dummy.memberFunc(100);
}