/**
 * @file file.hpp
 * @author Android172 (android172unity@gmail.com)
 * @brief Holds definitions for generic files, text files and binary files
 * @version 0.1
 * @date 2024-07-10
 *
 * @copyright Copyright (c) 2024
 *
 */
#pragma once

#include <fstream>

#include "common/defines.hpp"
#include "string.hpp"
#include "container/vector.hpp"

namespace CORE_NAMESPACE {

/**
 * @brief Generic input only file type
 */
template<typename T>
class FileIn : public std::ifstream {
  public:
    using std::ifstream::ifstream;

    virtual T read(const uint64 size);
    virtual T read_all() {
        const auto file_size = static_cast<uint64>(this->tellg());
        return read(file_size);
    }
};

/**
 * @brief Generic output only file type
 */
template<typename T>
class FileOut : public std::ofstream {
  public:
    using std::ofstream::ofstream;

    virtual void write(const T& data);
};

/**
 * @brief Generic IO file type
 */
template<typename T>
class File : public FileIn<T>, FileOut<T> {
  public:
    using FileIn<T>::FileIn;
    using FileOut<T>::FileOut;
};

// ///////// //
// Text file //
// ///////// //
/**
 * @brief Text input file. Decodes stream into text.
 */
class TextFileIn : public FileIn<String> {
    virtual String read(const uint64 size) override {
        char* buffer = new (BaseMemoryTags.Temp) char(size);
        auto  n      = std::ifstream::readsome(buffer, size);
        auto  result = n ? String(std::string(buffer, n)) : "";
        del(buffer);
        return result;
    }
};

/**
 * @brief Text output file. Encodes text for stream.
 *
 */
class TextFileOut : public FileOut<String> {
    virtual void write(const String& data) override {
        std::ofstream::write(data.data(), data.size());
    }

    template<typename... Args>
    void write(const Args&... data) {
        const auto all_data = String::build(data...);
        std::ofstream::write(all_data.data(), all_data.size());
    }

    template<typename... Args>
    void write_ln(const Args&... data) {
        write(data..., "\n");
    }
};

/**
 * @brief Text file. Operates on encoded text stream.
 */
typedef File<String> TextFile;

// ////////////// //
// TextLines file //
// ////////////// //
/**
 * @brief Per line text input file. Decodes input stream as text, one line
 * at a time.
 *
 */
class LinesFileIn : public FileIn<Vector<String>> {
    Vector<String> read(const uint64 size) override {
        Vector<String> lines;
        String         line;
        while (std::getline(*this, line))
            lines.push_back(line);
        return lines;
    }
};
/**
 * @brief Per line text output file. Encodes provided lines for output stream.
 */
class LinesFileOut : public FileOut<Vector<String>> {
    void write(const Vector<String>& data) override {
        for (const auto& line : data)
            std::ofstream::write(line.data(), line.size());
    }
};

/**
 * @brief Line based text file. Operates word lists, interpreted as text lines.
 */
typedef File<Vector<String>> LinesFile;

// /////////// //
// Binary file //
// /////////// //
/**
 * @brief Binary input file. Reads input stream byte by byte
 */
class BinaryFileIn : public FileIn<Vector<byte>> {
    virtual Vector<byte> read(const uint64 size) override {
        Vector<byte> buffer(size);
        this->seekg(0);
        std::ifstream::read(buffer.data(), size);
        return buffer;
    }
};
/**
 * @brief Binary output file. Writes arbitrary binary data to stream.
 */
class BinaryFileOut : public FileOut<Vector<byte>> {
    virtual void write(const Vector<char>& data) override {
        std::ofstream::write(data.data(), data.size());
    }
};

/**
 * @brief Binary file. Operates on binary data, one byte at a time.
 */
typedef File<Vector<byte>> BinaryFile;

} // namespace CORE_NAMESPACE