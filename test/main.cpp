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

Result<int, int> fn(int x) {
    if (x % 3 == 0) return 1;
    return Failure(x % 3 == 1);
}

int main() {
    const auto r = fn(34);
    match_error(r) {
        Err(0) { Logger::log("There was an error : 0"); }
        Err(1) { Logger::log("There was an error : 1"); }
        Ok() { Logger::log("There was no errors"); }
    }
}