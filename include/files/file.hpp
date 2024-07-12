/**
 * @file file.hpp
 * @author Android172 (android172unity@gmail.com)
 * @brief Holds definitions for generic file object used by file system
 * @version 0.1
 * @date 2024-07-10
 *
 * @copyright Copyright (c) 2024
 *
 */
#pragma once

#include <fstream>

#include "common/types.hpp"

namespace CORE_NAMESPACE {

namespace fs_details {
    template<typename IFS>
    class FileInBase : public virtual IFS {
      public:
        using IFS::IFS;
    };
    template<typename OFS>
    class FileOutBase : public virtual OFS {
      public:
        using OFS::OFS;
    };
} // namespace fs_details

/**
 * @brief Generic input only file type. Concrete input file type needs to extend
 * this class.
 * @tparam T Data type this file type operates on
 */
template<typename T, typename IFS>
class FileIn : public virtual fs_details::FileInBase<IFS> {
  public:
    using fs_details::FileInBase<IFS>::FileInBase;

    /**
     * @brief Read next @p size tokes from stream.
     *
     * @param size Total size read
     * @return T Resulting data
     */
    virtual T read(const uint64 size) = 0;
    /**
     * @brief Read all file content at once
     * @return T Resulting data
     */
    virtual T read_all() {
        IFS::seekg(0, std::ios::end);
        const auto file_size = static_cast<uint64>(IFS::tellg());
        IFS::seekg(0);
        return read(file_size);
    }
};

/**
 * @brief Generic output only file type. Concrete output file type needs to
 * extend this class.
 * @tparam T Data type this file type operates on
 */
template<typename T, typename OFS>
class FileOut : public virtual fs_details::FileOutBase<OFS> {
  public:
    using fs_details::FileOutBase<OFS>::FileOutBase;

    /**
     * @brief Write out provided data
     * @param data Data to be written
     */
    virtual void write(const T& data) = 0;
};

/**
 * @brief Generic IO file type. Concrete IO file type needs to extend or typedef
 * around this class.
 */
template<typename FileIn, typename FileOut>
class FileIO : public virtual FileIn, public virtual FileOut {};

/**
 * @brief File object of certain type. Extends `std::fstream` functionality,
 * together with extensions specified by file type. Behaves like `std::ifstream`
 * (`std::ofstream`) if `FileIn` (`FileOut`) is passed as file type. Otherwise
 * (with `FileIO`) both behaviors are supported.
 *
 * @tparam FileType Type of this file. Describes what data is used for IO and
 * how.
 */
template<typename FileType, typename = void>
class File;

template<typename FileType>
class File<
    FileType,
    typename std::enable_if_t<
        std::is_base_of_v<fs_details::FileInBase<std::ifstream>, FileType> &&
        !std::is_base_of_v<fs_details::FileOutBase<std::ofstream>, FileType>>>
    : public FileType {
  public:
    using FileType::FileType;
};

template<typename FileType>
class File<
    FileType,
    typename std::enable_if_t<
        !std::is_base_of_v<fs_details::FileInBase<std::ifstream>, FileType> &&
        std::is_base_of_v<fs_details::FileOutBase<std::ofstream>, FileType>>>
    : public FileType {
  public:
    using FileType::FileType;
};

template<typename FileType>
class File<
    FileType,
    typename std::enable_if_t<
        std::is_base_of_v<fs_details::FileInBase<std::fstream>, FileType> &&
        std::is_base_of_v<fs_details::FileOutBase<std::fstream>, FileType>>>
    : public FileType {
  public:
    using FileType::FileType;
};

} // namespace CORE_NAMESPACE