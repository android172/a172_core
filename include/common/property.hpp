#pragma once

#include "error_types.hpp"
#include "string.hpp"

#include <functional>
#include <utility>

namespace CORE_NAMESPACE {

#define GET [this]() -> auto const&
#define GET_V [this]() -> auto
#define SET , [this](auto value)

/**
 * @brief Exception specialized for use with \c Property class. Thrown when
 * modification of immutable properties is attempted.
 */
class PropertyImmutableException : public RuntimeError {
  public:
    using RuntimeError::RuntimeError;

    PropertyImmutableException()
        : RuntimeError("Immutable property manipulation failed. Value of this "
                       "property cannot be changed.") {}
};

/**
 * @brief General property class. Represents a wrapper for class getter and
 * setter functionality over some attribute. Inspired by C# properties. Should
 * be declared as class attribute like so:
 *
 *      Property<T> attribute {
 *          GET { return _attribute; }
 *          SET { _attribute = value; } // Optional
 *      }
 *
 * Most commonly this property is used as public facing interface for private
 * \a _attribute, but other configurations are also supported.
 * Setter is optional, if undeclared value is considered immutable. In default
 * configuration getter returns const& to desired attribute, while setter
 * accepts \b T&& value. If \b T is a reference type ( \b &T ) setter will be
 * set by reference instead. If retrieval of \b T by value, instead of reference
 * can be accomplished like so:
 *
 *      Property<T&&> attribute {
 *          GET_V { return ...local_value...; }
 *          SET { _attribute = value; } // Optional
 *      }
 *
 * @tparam T Type of the value accessed by \c Property.
 */
template<class T>
class Property {
  public:
    /**
     * @brief Construct a new \c Property object. Construction of property
     * should be called like so:
     *
     *      Property<T> attribute {
     *          GET { return _attribute; }
     *          SET { _attribute = value; } // Optional
     *      }
     *
     */
    Property(
        const std::function<T const&()> get_fn,
        const std::function<void(T&&)>  set_fn =
            [](auto&& value) { throw PropertyImmutableException(); }
    ) noexcept
        : _getter { std::move(get_fn) }, _setter { std::move(set_fn) } {}

    Property()                                 = delete;
    Property<T>& operator=(Property<T> const&) = delete;
    // TODO: Probable error causes, should deal with them.
    // Property(Property<T> const&) = delete;
    // Property(Property<T> const&&) = delete;
    // Property<T>& operator = (Property<T>&&) = delete;

    operator T const&() const { return _getter(); }
    void     operator=(T const& value) { _setter(value); }
    void     operator=(T&& value) { _setter(std::move(value)); }
    T const& operator()() const { return _getter(); }

  private:
    const std::function<T const&()> _getter;
    const std::function<void(T)>    _setter;
};

/**
 * @brief Property class specialization for reference attribute types.
 *
 * @tparam T Type of the value accessed by \c Property.
 */
template<typename T>
class Property<T&> {
  public:
    /**
     * @brief Construct a new \c Property object. Construction of property
     * should be called like so:
     *
     *      Property<T&> attribute {
     *          GET { return _attribute; }
     *          SET { _attribute = value; } // Optional
     *      }
     *
     */
    Property(
        const std::function<T const&()> get_fn,
        const std::function<void(T&)>   set_fn =
            [](auto& value) { throw PropertyImmutableException(); }
    ) noexcept
        : _getter { std::move(get_fn) }, _setter { std::move(set_fn) } {}

    Property()                                 = delete;
    Property<T>& operator=(Property<T> const&) = delete;

    operator T const&() const { return _getter(); }

    void     operator=(T const& value) { _setter(value); }
    T const& operator()() const { return _getter(); }

  private:
    const std::function<T const&()> _getter;
    const std::function<void(T&)>   _setter;
};

/**
 * @brief Property class specialization for returning by value when such
 * functionality is needed.
 *
 * @tparam T Type of the value accessed by \c Property.
 */
template<typename T>
class Property<T&&> {
  public:
    /**
     * @brief Construct a new \c Property object. Construction of property
     * should be called like so:
     *
     *      Property<T&&> attribute {
     *          GET_V { return ...; }
     *          SET { _attribute = value; } // Optional
     *      }
     *
     */
    Property(
        const std::function<T()>      get_fn,
        const std::function<void(T&)> set_fn =
            [](auto& value) { throw PropertyImmutableException(); }
    ) noexcept
        : _getter { std::move(get_fn) }, _setter { std::move(set_fn) } {}

    Property()                                 = delete;
    Property<T>& operator=(Property<T> const&) = delete;

    operator T() const { return _getter(); }

    void operator=(T const& value) { _setter(value); }
    T    operator()() const { return _getter(); }

  private:
    const std::function<T()>      _getter;
    const std::function<void(T&)> _setter;
};

} // namespace CORE_NAMESPACE