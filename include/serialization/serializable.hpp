/**
 * @file serializable.hpp
 * @author Android172 (android172unity@gmail.com)
 * @brief Holds information on generic serializable object and ways serializing
 * non-serializable objects
 * @version 0.1
 * @date 2024-07-12
 *
 * @copyright Copyright (c) 2024
 *
 */
#pragma once

#include "files/file_system.hpp"
#include "files/file_types.hpp"
#include "files/path.hpp"
#include "result.hpp"
#include "string.hpp"

namespace CORE_NAMESPACE {
class String;
class Serializer;
class RuntimeError;

/**
 * @brief An abstract class that provides an interface for objects to be
 * serialized and deserialized.
 * The Serializable class defines two pure virtual functions: serialize() and
 * deserialize(). These functions convert object to and from their serialized
 * state.
 */
class Serializable {
  public:
    /**
     * @brief Converts the object into a serialized format using the provided
     * serializer.
     *
     * This pure virtual function must be implemented by derived classes to
     * define the serialization logic for the object. Since it usually only
     * defines order of the used attributes use of `serializable_attributes`
     * macro is recommended. Example:
     *
     * @code
     * class C : public Serializable {
     *  int m1;
     *  int m2;
     *  int m3;
     *
     *  serializable_attributes(m1, m2, m3);
     * }
     * @endcode
     *
     * @param serializer A pointer to a Serializer object used for
     * serialization.
     * @return String Object representing the serialized data.
     */
    virtual String serialize(const Serializer* const serializer) const = 0;
    /**
     * @brief Restores the object's original state by deserializing the data
     * using the provided serializer.
     *
     * This pure virtual function must be implemented by derived classes to
     * define the deserialization logic for the object. Since it usually only
     * defines order of the used attributes use of `serializable_attributes`
     * macro is recommended. Example:
     *
     * @code
     * class C : public Serializable {
     *  int m1;
     *  int m2;
     *  int m3;
     *
     *  serializable_attributes(m1, m2, m3);
     * }
     * @endcode
     *
     * @param serializer A pointer to a Serializer object used for
     * deserialization.
     * @param data The String object containing the serialized data.
     * @param from_pos The optional starting position for deserialization
     * (default is 0).
     * @return uint32 The position after deserialization
     * @throw RuntimeError If deserialization failed
     */
    virtual Result<uint32, RuntimeError> deserialize(
        const Serializer* const serializer,
        const String&           data,
        const uint32            from_pos = 0
    ) = 0;

    /**
     * @brief Converts object into serialized format and saves it to a file.
     * Uses `serialize` method internally.
     *
     * @param file_path Path of output file
     * @param serializer A pointer to a Serializer object used for
     * serialization.
     * @throw RuntimeError If file can not be opened or created.
     */
    Result<void, RuntimeError> serialize_to_file(
        const Path& file_path, const Serializer* const serializer
    ) const {
        // Open file
        auto result = FileSystem::create_or_open<TextOut>(file_path);
        if (result.has_error()) return Failure(result.error());
        const auto file { std::move(result.value()) };

        // Write all data
        const auto str = this->serialize(serializer);
        file->write(str);

        // Close file
        file->close();
        return {};
    }

    /**
     * @brief Restores the object's original state by deserializing the data
     * from file. Uses `deserialize` method internally.
     *
     * @param file_path Input file's path
     * @param serializer A pointer to a Serializer object used for
     * deserialization.
     * @return uint32 The position after deserialization
     * @throw RuntimeError If file read or deserialization fails
     */
    Result<uint32, RuntimeError> deserialize_from_file(
        const Path& file_path, const Serializer* const serializer
    ) {
        // Read file
        auto result = FileSystem::open<TextIn>(file_path);
        if (result.has_error()) return Failure(result.error());
        const auto file { std::move(result.value()) };

        // Deserialize read data
        const auto data = file->read_all();
        return this->deserialize(serializer, data, 0);
    }
};

/**
 * Serializes an object of type @b T using the provided serializer. This
 * function is useful for external types which cannot be made serializable
 * trough inheritance. Specifying a specialization of this function for given
 * type @b T enables us to use type @b T as serializable member of a
 * serializable class.
 *
 * @tparam T The type of the object to be serialized.
 * @param obj The object to be serialized.
 * @param serializer The serializer to be used for serialization.
 * @return String The serialized object as a String.
 */
template<typename T>
String serialize_object(const T& obj, const Serializer* const serializer) {
    static_assert(
        std::is_same_v<T, void>,
        "Serialization called for non-serializable type."
    );
}

/**
 * Deserializes an object of type @b T using the provided serializer and data.
 * This function is useful for external types which cannot be made serializable
 * trough inheritance. Specifying a specialization of this function for given
 * type @b T enables us to use type @b T as deserializable member of a
 * serializable class.
 *
 * @tparam T The type of the object to be deserialized.
 * @param obj The object to be deserialized.
 * @param serializer The serializer to be used for deserialization.
 * @param data The data to be deserialized.
 * @param from_pos The starting position in the data for deserialization
 * (default: 0).
 * @return uint32 The position after deserialization
 * @throw RuntimeError If deserialization failed
 */
template<typename T>
Result<uint32, RuntimeError> deserialize_object(
    T&                      obj,
    const Serializer* const serializer,
    const String&           data,
    const uint32            from_pos = 0
) {
    static_assert(
        std::is_same_v<T, void>,
        "Deserialization called for non-serializable type."
    );
}

#define serializable_attributes(attributes...)                                 \
    virtual String serialize(const Serializer* const serializer)               \
        const override {                                                       \
        return serializer->serialize(attributes);                              \
    }                                                                          \
    virtual Result<uint32, RuntimeError> deserialize(                          \
        const Serializer* const serializer,                                    \
        const String&           data,                                          \
        const uint32            from_pos = 0                                   \
    ) override {                                                               \
        return serializer->deserialize(data, from_pos, attributes);            \
    }

} // namespace CORE_NAMESPACE