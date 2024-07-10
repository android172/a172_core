#include "common/error_types.hpp"
#include "common/types.hpp"
#include "event.hpp"
#include "logger.hpp"
#include "multithreading/parallel.hpp"
#include "property.hpp"
#include "common/type_traits.hpp"

#include "container/forward_list.hpp"
#include "container/list.hpp"
#include "container/map.hpp"
#include "container/set.hpp"
#include "container/unordered_map.hpp"
#include "container/vector.hpp"

#include "files/file_system.hpp"
#include "result.hpp"
#include "string.hpp"

#include <array>
#include <cstddef>
#include <functional>
#include <iostream>
#include <type_traits>

using namespace CORE_NAMESPACE;

int main() {
    String a = " BlaBlaBla  ";
    auto   b = String::trim(a).to_upper();
    Logger::log("|", a, "|");
    Logger::log("|", b, "|");
}