#include "logger.hpp"
#include "property.hpp"
#include <functional>

using namespace a172;

int global = 3;
class B {
  public:
    B(int a) : _a(a) {}
    B(B& b) : _a(b.a) {} //, _a2(b._a2) {}

    Property<int>   a { GETSET _a };
    Property<int&>  a2 { GETSET global };
    Property<int&&> a3 { _a };

    int _a;
    // int& _a2 = global;

    // ...
};

int main() {
    Logger::log("Hi");

    B b1 { 5 };
    B b2 { 12 };

    Logger::log(b1.a());
    Logger::log(b2.a());
    Logger::log(b1.a3());

    b1   = b2;
    b1.a = 15;
    b2.a = 50;

    Logger::log(b1.a());
    Logger::log(b2.a());

    B b3 { b1 };
    b1.a = 56;

    Logger::log(b1.a());
    Logger::log(b3.a());

    Logger::log(b1.a2());
    Logger::log(b2.a2());

    int c = 91;
    b1.a2 = c;

    Logger::log(b1.a2());
    Logger::log(b2.a2());

    b1.a2 = 92;

    Logger::log(b1.a2());
    Logger::log(b2.a2());

    Logger::log(global);
    Logger::log(b1.a3());

    return 0;
}