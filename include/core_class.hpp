#pragma once

#include <iostream>

class CoreClass {
  public:
    CoreClass();
    ~CoreClass();

    static void test_method() {
        std::cout << "Method is working as intended." << std::endl;
    }

  private:
};
