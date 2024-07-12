#include "common/error_types.hpp"
#include "common/types.hpp"
#include "event.hpp"
#include "files/file.hpp"
#include "files/path.hpp"
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
#include "serialization/binary_serializer.hpp"
#include "serialization/serializable.hpp"
#include "string.hpp"

#include <array>
#include <cmath>
#include <cstddef>
#include <fstream>
#include <functional>
#include <ios>
#include <iostream>
#include <iterator>
#include <type_traits>

using namespace CORE_NAMESPACE;

class Dummy : public Serializable {
  public:
    String x1 = "Some Text";
    int    x2 = 12;
    float  x3 = 12.31;

    serializable_attributes(x1, x2, x3);

    String to_string() { return String::build(x1, " | ", x2, " | ", x3); }
};

template<typename IFStream>
class A1 : public virtual IFStream {
  public:
    using IFStream::IFStream;
};
template<typename OFStream>
class A2 : public virtual OFStream {
  public:
    using OFStream::OFStream;
};

template<typename T, typename IFStream>
class B1 : public virtual A1<IFStream> {
  public:
    using A1<IFStream>::A1;
    virtual void m1(const T& a = {}) = 0;
};
template<typename T, typename OFStream>
class B2 : public virtual A2<OFStream> {
  public:
    using A2<OFStream>::A2;
    virtual void m2(const T& a = {}) = 0;
};

template<typename B1, typename B2>
class B3 : public virtual B1, public virtual B2 {
    using B1::B1;
};

template<typename IFStream>
class D1B : public virtual B1<String, IFStream> {
  public:
    using B1<String, IFStream>::B1;

    virtual void m1(const String& a = {}) override {
        Logger::log(IFStream::is_open());
        Logger::log("M1");
    }
};
template<typename OFStream>
class D2B : public virtual B2<String, OFStream> {
  public:
    using B2<String, OFStream>::B2;

    virtual void m2(const String& a = {}) override {
        Logger::log(OFStream::is_open());
        Logger::log("M2");
    }
};
typedef D1B<std::ifstream>                       D1;
typedef D2B<std::ofstream>                       D2;
typedef B3<D1B<std::fstream>, D2B<std::fstream>> D3;

template<typename D, typename = void>
class C;

template<typename D>
class C<
    D,
    typename std::enable_if_t<
        std::is_base_of_v<A1<std::ifstream>, D> &&
        !std::is_base_of_v<A2<std::ofstream>, D>>> : public D {
  public:
    using D::D;
};

template<typename D>
class C<
    D,
    typename std::enable_if_t<
        !std::is_base_of_v<A1<std::ifstream>, D> &&
        std::is_base_of_v<A2<std::ofstream>, D>>> : public D {
  public:
    using D::D;
};

template<typename D>
class C<
    D,
    typename std::enable_if_t<
        std::is_base_of_v<A1<std::fstream>, D> &&
        std::is_base_of_v<A2<std::fstream>, D>>> : public D {
  public:
    using D::D;
};

int main() {
    const auto path = Path("./ef.txt");

    // C<D1> c1 { path };
    // C<D2> c2 {};
    // C<D3> c3 {};

    // c1.m1();
    // c2.m2();
    // c3.m1();
    // c3.m2();
    // Logger::log(c3.is_open());

    auto du = Dummy();

    const auto bs = BinarySerializer();

    const auto res = du.serialize(&bs);
    Logger::log(res);

    Logger::log(path.string());

    const auto result = du.deserialize_from_file(path, &bs);
    Logger::log(result.has_error());
    if (result.has_error()) Logger::log(result.error().what());
}