#pragma once

#include <iostream>

class TestClass {
  public:
    TestClass();
    ~TestClass();

    static void test_method() {
        std::cout << "Method is working as intended." << std::endl;
    }

  private:
};
