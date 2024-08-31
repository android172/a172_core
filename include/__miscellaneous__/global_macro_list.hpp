/**
 * @brief This file contains a list of globally used macros. None of this names
 * can be given to any class, method, macro or other.
 */

/*
// === Engine specific ===
//  Supported platforms
    -   PLATFORM        Note: Check current platform (ex. PLATFORM == LINUX)
    -   LINUX
    -   WINDOWS32
    -   WINDOWS64
//  For logs
    -   fatal           Note: Used only in Logger::fatal, not alone
    -   LOG_LOCATION    Node: Current file, line and function for output
//  Property
    -   GET
    -   SET
    -   GETSET
//  Result
    -   match_error(result)
    -   match_error_code(result)
    -   Err(error)
    -   Ok()
    -   check(result)
// Serialization
    -   serializable_attributes     Note: Defines default serialization and
                                          deserialization behaviour for selected
                                          attributes of serializable object
//  Multithreading
    -   for_each
    -   for_range
    -   for_loop_1 & for_loop_2
    -   GET_FOR_LOOP_MACRO
//  General for use
    -   CORE_NAMESPACE
    -   KB & MB & GB
    -   MEMORY_PADDING      Note: Memory padding used by allocators
    -   CONST_STRING        Note: `constexpr static const char* const` type
    -   STRING_ENUM(str)    Note: Used for custom "String" enum classes
*/
