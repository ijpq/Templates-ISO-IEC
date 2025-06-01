#include "template.h"

#ifndef DEMONSTRATE_ERROR

template <>
void dummyFunc(int a) { // 方案二
    cout << "print in dummyFunc<int>, a: " << a << endl;
}

template <typename T>
void Dummy<T>::memberFunc(int a) {
    cout << "print in Dummy<T>::memberFunc, a: " << a << endl;
}

template struct Dummy<int>; // 方案二
#endif
