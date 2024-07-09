/**
 * @file property.hpp
 * @author Android172 (android172unity@gmail.com)
 * @brief Defines generic property type.
 * @version 0.1
 * @date 2024-05-24
 *
 * @copyright Copyright (c) 2024
 *
 */

#pragma once

#include "common/error_types.hpp"
#include "string.hpp"

#include <functional>
#include <utility>

namespace CORE_NAMESPACE {

#define GET [this]() -> auto const&
#define GET_V [this]() -> auto
#define SET , [this](auto value)

/**
 * @brief Base class for generic property class. Holds some common definitions.
 * Not usable by itself.
 */
class BaseProperty {
  public:
    /**
     * @brief Exception specialized for use with \c Property class. Thrown
     * when modification of immutable properties is attempted.
     */
    class ImmutableException : public RuntimeError {
      public:
        using RuntimeError::RuntimeError;

        ImmutableException()
            : RuntimeError(
                  "Immutable property manipulation failed. Value of this "
                  "property cannot be changed."
              ) {}
    };
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
template<typename T>
class Property : protected BaseProperty {
    using BaseProperty::BaseProperty;

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
        const std::function<const T&()> get_fn,
        const std::function<void(T&&)>  set_fn =
            [](auto&& value) { throw ImmutableException(); }
    ) noexcept
        : _getter { std::move(get_fn) }, _setter { std::move(set_fn) } {}

    Property() = delete;

    // On copy do nothing
    Property<T>& operator=(Property<T> const&) { return *this; };
    Property<T>& operator=(Property<T>&&) { return *this; };
    // TODO: Make copy constructable somehow??
    Property(Property<T> const&)  = delete;
    Property(Property<T> const&&) = delete;

    void operator=(const T& value) { _setter(value); }
    void operator=(T&& value) { _setter(std::move(value)); }

    operator const T&() const { return _getter(); }
    const T& operator()() const { return _getter(); }
    const T* operator->() { return &_getter(); };

  private:
    const std::function<const T&()> _getter;
    const std::function<void(T)>    _setter;
};

/**
 * @brief Property class specialization for reference attribute types.
 *
 * @tparam T Type of the value accessed by \c Property.
 */
template<typename T>
class Property<T&> : protected BaseProperty {
    using BaseProperty::BaseProperty;

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
        const std::function<const T&()> get_fn,
        const std::function<void(T&)>   set_fn = [](auto& value
                                               ) { throw ImmutableException(); }
    ) noexcept
        : _getter { std::move(get_fn) }, _setter { std::move(set_fn) } {}

    Property() = delete;

    // On copy do nothing
    Property<T>& operator=(Property<T> const&) { return *this; };
    Property<T>& operator=(Property<T>&&) { return *this; };
    Property(Property<T> const&)  = delete;
    Property(Property<T> const&&) = delete;

    void operator=(const T& value) { _setter(value); }

    operator const T&() const { return _getter(); }
    const T& operator()() const { return _getter(); }
    const T* operator->() { return &_getter(); };

  private:
    const std::function<const T&()> _getter;
    const std::function<void(T&)>   _setter;
};

/**
 * @brief Property class specialization for returning by value when such
 * functionality is needed.
 *
 * @tparam T Type of the value accessed by \c Property.
 */
template<typename T>
class Property<T&&> : protected BaseProperty {
    using BaseProperty::BaseProperty;

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
        const std::function<T()>       get_fn,
        const std::function<void(T&&)> set_fn =
            [](auto&& value) { throw ImmutableException(); }
    ) noexcept
        : _getter { std::move(get_fn) }, _setter { std::move(set_fn) } {}

    Property() = delete;

    // On copy do nothing
    Property<T>& operator=(Property<T> const&) { return *this; };
    Property<T>& operator=(Property<T>&&) { return *this; };
    Property(Property<T> const&)  = delete;
    Property(Property<T> const&&) = delete;

    void operator=(const T& value) { _setter(value); }
    void operator=(T&& value) { _setter(std::move(value)); }

    operator T() const { return _getter(); }
    T operator()() const { return _getter(); }

  private:
    const std::function<T()>     _getter;
    const std::function<void(T)> _setter;
};

} // namespace CORE_NAMESPACE