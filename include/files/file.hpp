#pragma once

#include <fstream>

#include "common/defines.hpp"
#include "string.hpp"
#include "container/vector.hpp"

namespace CORE_NAMESPACE {

/**
 * @brief Input file
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
 * @brief Output file
 */
template<typename T>
class FileOut : public std::ofstream {
  public:
    using std::ofstream::ofstream;

    virtual void write(const T& data);
};

/**
 * @brief IO File
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
class TextFileIn : public FileIn<String> {
    virtual String read(const uint64 size) override {
        char* buffer = new (BaseMemoryTags.Temp) char(size);
        auto  n      = std::ifstream::readsome(buffer, size);
        auto  result = n ? String(std::string(buffer, n)) : "";
        del(buffer);
        return result;
    }
};
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
typedef File<String> TextFile;

// ////////////// //
// TextLines file //
// ////////////// //
class LinesFileIn : public FileIn<Vector<String>> {
    Vector<String> read(const uint64 size) override {
        Vector<String> lines;
        String         line;
        while (std::getline(*this, line))
            lines.push_back(line);
        return lines;
    }
};
class LinesFileOut : public FileOut<Vector<String>> {
    void write(const Vector<String>& data) override {
        for (const auto& line : data)
            std::ofstream::write(line.data(), line.size());
    }
};
typedef File<Vector<String>> LinesFile;

// /////////// //
// Binary file //
// /////////// //

class BinaryFileIn : public FileIn<Vector<byte>> {
    virtual Vector<byte> read(const uint64 size) override {
        Vector<byte> buffer(size);
        this->seekg(0);
        std::ifstream::read(buffer.data(), size);
        return buffer;
    }
};
class BinaryFileOut : public FileOut<Vector<byte>> {
    virtual void write(const Vector<char>& data) override {
        std::ofstream::write(data.data(), data.size());
    }
};
typedef File<Vector<byte>> BinaryFile;

} // namespace CORE_NAMESPACE