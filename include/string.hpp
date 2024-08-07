/**
 * @file string.hpp
 * @author Android172 (android172unity@gmail.com)
 * @brief Holds String related functionality.
 * @version 0.1
 * @date 2024-07-10
 *
 * @copyright Copyright (c) 2024
 *
 */
#pragma once

#include <string>

#include "common/defines.hpp"
#include "result.hpp"
#include "common/error_types.hpp"
#include "container/vector.hpp"

namespace CORE_NAMESPACE {

template<typename T>
class Property;
}

// Additional to_string conversions
namespace std {
string to_string(const CORE_NAMESPACE::uint128& in);
string to_string(const CORE_NAMESPACE::int128& in);
template<typename T>
string to_string(const CORE_NAMESPACE::Property<T>& in);
string to_string(const CORE_NAMESPACE::Vector<char>& in);
string to_string(const CORE_NAMESPACE::Vector<unsigned char>& in);
} // namespace std

namespace CORE_NAMESPACE {

/**
 * @brief String (array of characters). Extends std::string, with some
 * additional methods
 */
class String : public std::string {
  public:
    using std::string::string;
    String() noexcept;
    String(const std::string& __str) noexcept : std::string(__str) {}
    ~String() noexcept;

    // String builder
    /**
     * @brief Concatenates argument list into one string string object.
     * Non-string objects will automaticaly be converted to their string form
     * via std::to_string where applicable.
     * @return String Concatenated string
     */
    template<typename... Args>
    static String build(const Args&... message) noexcept {
        String result = "";
        (add_to_string(result, message), ...);
        return result;
    }

    // Transform
    /// @brief Transform all string characters to lowercase (inplace)
    String& to_lower() noexcept;
    /// @brief Transform all string characters to uppercase (inplace)
    String& to_upper() noexcept;

    /// @brief Get transformed copy of the string, with all characters lowercase
    static String to_lower(const String& str) noexcept;
    /// @brief Get transformed copy of the string, with all characters uppercase
    static String to_upper(const String& str) noexcept;

    // Trim
    /// @brief Removes all white-space characters from left side (Inplace)
    String& trim_left() noexcept;
    /// @brief Removes all white-space characters from right side (Inplace)
    String& trim_right() noexcept;
    /// @brief Removes all white-space characters from both sides (Inplace)
    String& trim() noexcept;

    /// @brief Get transformed copy of the string, with all white-space
    /// characters removed from left side
    static String trim_left(const String& str) noexcept;
    /// @brief Get transformed copy of the string, with all white-space
    /// characters removed from right side
    static String trim_right(const String& str) noexcept;
    /// @brief Get transformed copy of the string, with all white-space
    /// characters removed from both sides
    static String trim(const String& str) noexcept;

    // Comparison
    /**
     *  @brief  Compare two strings; case insensitive.
     *  @param other  String to compare against.
     *  @return int32 Integer < 0, 0, or > 0.
     *
     * Returns an integer < 0 if this string is ordered before @p other, 0 if
     * their values are equivalent, or > 0 if this string is ordered after
     * @p other. Determines the effective length rlen of the strings to compare
     * as the smallest of size() and str.size(). The function then compares the
     * two strings by calling traits::compare(data(), str.data(),rlen). If the
     * result of the comparison is nonzero returns it, otherwise the shorter one
     * is ordered first.
     */
    int32 compare_ci(const String& other) const;

    // Split
    /**
     * @brief Splits string into substrings
     * @param delimiter Token to split by
     * @return Vector<String> Vector of strings
     */
    Vector<String> split(const String delimiter) const;
    /**
     * @brief Splits string into substrings
     * @param delimiter Token to split by
     * @return Vector<String> Vector of strings
     */
    Vector<String> split(const char delimiter) const;

    // Parsing
    /**
     * @brief Parses string as uint8
     * @return uint8
     * @throw InvalidArgument If parse is impossible. Parse is also deemed
     * impossible if overflow is detected.
     */
    Result<uint8, InvalidArgument>    parse_as_uint8();
    /**
     * @brief Parses string as uint16
     * @return uint16
     * @throw InvalidArgument If parse is impossible. Parse is also deemed
     * impossible if overflow is detected.
     */
    Result<uint16, InvalidArgument>   parse_as_uint16();
    /**
     * @brief Parses string as uint32
     * @return uint32
     * @throw InvalidArgument If parse is impossible. Parse is also deemed
     * impossible if overflow is detected.
     */
    Result<uint32, InvalidArgument>   parse_as_uint32();
    /**
     * @brief Parses string as uint64
     * @return uint64
     * @throw InvalidArgument If parse is impossible. Parse is also deemed
     * impossible if overflow is detected.
     */
    Result<uint64, InvalidArgument>   parse_as_uint64();
    /**
     * @brief Parses string as uint128
     * @return uint128
     * @throw InvalidArgument If parse is impossible
     */
    Result<uint128, InvalidArgument>  parse_as_uint128();
    /**
     * @brief Parses string as int8
     * @return int8
     * @throw InvalidArgument If parse is impossible. Parse is also deemed
     * impossible if overflow is detected.
     */
    Result<int8, InvalidArgument>     parse_as_int8();
    /**
     * @brief Parses string as int16
     * @return int16
     * @throw InvalidArgument If parse is impossible. Parse is also deemed
     * impossible if overflow is detected.
     */
    Result<int16, InvalidArgument>    parse_as_int16();
    /**
     * @brief Parses string as int32
     * @return int32
     * @throw InvalidArgument If parse is impossible. Parse is also deemed
     * impossible if overflow is detected.
     */
    Result<int32, InvalidArgument>    parse_as_int32();
    /**
     * @brief Parses string as int64
     * @return int64
     * @throw InvalidArgument If parse is impossible. Parse is also deemed
     * impossible if overflow is detected.
     */
    Result<int64, InvalidArgument>    parse_as_int64();
    /**
     * @brief Parses string as int128
     * @return int128
     * @throw InvalidArgument If parse is impossible
     */
    Result<int128, InvalidArgument>   parse_as_int128();
    /**
     * @brief Parses string as float32
     * @return float32
     * @throw InvalidArgument If parse is impossible
     */
    Result<float32, InvalidArgument>  parse_as_float32();
    /**
     * @brief Parses string as float64
     * @return float64
     * @throw InvalidArgument If parse is impossible
     */
    Result<float64, InvalidArgument>  parse_as_float64();
    /**
     * @brief Parses string as float128
     * @return float128
     * @throw InvalidArgument If parse is impossible
     */
    Result<float128, InvalidArgument> parse_as_float128();

    // NEW
    void* operator new(size_t size) {
        return ::operator new(size, BaseMemoryTags.String);
    }

  private:
    // String builder
    template<typename T>
    static void add_to_string(String& out_string, const T& component) noexcept {
        out_string += std::to_string(component);
    }
    template<typename T>
    static void add_to_string(String& out_string, T* const component) noexcept {
        out_string += std::to_string(component);
    }
    template<typename T>
    static void add_to_string(
        String& out_string, const Property<T> component
    ) noexcept {
        add_to_string<T>(out_string, component());
    }
    template<typename T>
    static void add_to_string(
        String& out_string, const Property<T&> component
    ) noexcept {
        add_to_string<T>(out_string, component());
    }
    template<typename T>
    static void add_to_string(
        String& out_string, const Property<T&&> component
    ) noexcept {
        add_to_string<T>(out_string, component());
    }
};

template<>
void String::add_to_string<char>(
    String& out_string, const char& component
) noexcept;
template<>
void String::add_to_string<char>(
    String& out_string, char* const component
) noexcept;
template<>
void String::add_to_string<const char>(
    String& out_string, const char* const component
) noexcept;
template<>
void String::add_to_string<std::string>(
    String& out_string, const std::string& component
) noexcept;
template<>
void String::add_to_string<String>(
    String& out_string, const String& component
) noexcept;

} // namespace CORE_NAMESPACE

namespace std {
template<>
struct hash<CORE_NAMESPACE::String> {
    size_t operator()(CORE_NAMESPACE::String const& str) const {
        return hash<string>()(str);
    }
};
} // namespace std
