/**
 * @file file_system.hpp
 * @author Android172 (android172unity@gmail.com)
 * @brief Defines platform agnostic file system
 * @version 0.1
 * @date 2024-07-10
 *
 * @copyright Copyright (c) 2024
 *
 */
#pragma once

#include <filesystem>

#include "path.hpp"
#include "result.hpp"
#include "common/error_types.hpp"
#include "file.hpp"

namespace CORE_NAMESPACE {

/**
 * @brief FileSystem. Responsible for managing files in a platform agnostic way.
 * Shouldn't be initialized; treated as collection of functionalities.
 */
class FileSystem {
  public:
    /// @brief File open mode flags
    typedef std::_Ios_Openmode OpenMode;

    /// Seek to end before each write.
    static const OpenMode app = OpenMode::_S_app;

    /// Open and seek to end immediately after opening.
    static const OpenMode ate = OpenMode::_S_ate;

    /// Perform input and output in binary mode (as opposed to text mode).
    /// This is probably not what you think it is; see
    /// https://gcc.gnu.org/onlinedocs/libstdc++/manual/fstreams.html#std.io.filestreams.binary
    static const OpenMode binary = OpenMode::_S_bin;

    /// Truncate an existing stream when opening.  Default for @c ofstream.
    static const OpenMode trunc = OpenMode::_S_trunc;

    /**
     * @brief Checks whether a file on a given path exists
     *
     * @param file_path File path
     * @return true If file exists
     * @return false Otherwise
     */
    static bool exists(const Path& file_path) {
        return std::filesystem::exists(file_path);
    }

    /**
     * @brief Opens file for input and/or output. Will fails if file doesn't
     * exist.
     *
     * @param file_path File path
     * @param mode Active file open modes
     * @return File If successful
     * @throw RuntimeError Otherwise
     */
    template<typename FileT>
    static Result<std::unique_ptr<File<FileT>>, RuntimeError> open(
        const Path& file_path, OpenMode mode = {}
    ) {
        auto file = std::make_unique<File<FileT>>(file_path, mode);
        if (!file->is_open()) return error_cant_open(file_path);
        return file;
    }

    /**
     * @brief Create and open a file. Will fail if file already exists. All
     * required nonexistant directories will also be created.
     *
     * @param file_path File path
     * @param mode Active file open modes
     * @return File If successful
     * @throw RuntimeError Otherwise
     */
    template<typename FileT>
    static Result<std::unique_ptr<File<FileT>>, RuntimeError> create(
        const Path& file_path, OpenMode mode = {}
    ) {
        // Check existence
        if (std::filesystem::exists(file_path))
            return error_pre_existant_path(file_path);

        // Create required directories
        std::filesystem::create_directories(file_path.parent_path());

        // Create & open file
        auto file = std::make_unique<File<FileT>>(file_path, mode);
        if (!file->is_open()) return error_creation_failed(file_path);
        return file;
    }

    /**
     * @brief Create and open a file. Just opens already existing files. All
     * required nonexistant directories will also be created.
     *
     * @param file_path File path
     * @param mode Active file open modes
     * @return File If successful
     * @throw RuntimeError Otherwise
     */
    template<typename FileT>
    static Result<std::unique_ptr<File<FileT>>, RuntimeError> create_or_open(
        const Path& file_path, OpenMode mode = {}
    ) {
        // Check existence
        if (!std::filesystem::exists(file_path))
            // Create required directories
            std::filesystem::create_directories(file_path.parent_path());

        // Create & open file
        auto file = std::make_unique<File<FileT>>(file_path, mode);
        if (!file->is_open()) return error_creation_failed(file_path);
        return file;
    }

    /**
     * @brief Opens and fully reads a given file
     *
     * @param file_path File path
     * @return T All file data if successful
     * @throw RuntimeError Otherwise
     */
    template<typename T, typename FileT>
    static Result<T, RuntimeError> read_all(const Path& file_path) {
        auto       file   = open<FileT>(file_path, std::ios::ate);
        const auto buffer = file.read_all();
        file.close();
        return buffer;
    }

  private:
    FileSystem();
    ~FileSystem();

    static Failure<RuntimeError> error_cant_open(const Path& path) {
        return Failure("Failed to open file:" + path.string());
    }
    static Failure<RuntimeError> error_nonexistant_path(const Path& path) {
        return Failure(
            "Failed to open file:" + path.string() +
            ". This file doesn't exist."
        );
    }
    static Failure<RuntimeError> error_creation_failed(const Path& path) {
        return Failure("Failed to create file:" + path.string());
    }
    static Failure<RuntimeError> error_pre_existant_path(const Path& path) {
        return Failure(
            "Failed to create file:" + path.string() +
            ". This file already exist."
        );
    }
};

} // namespace CORE_NAMESPACE