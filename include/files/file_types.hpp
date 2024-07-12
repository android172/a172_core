/**
 * @file file_types.hpp
 * @author Android172 (android172unity@gmail.com)
 * @brief Holds some builtin base file types like Text, Line and Binary. File
 * types define what file data is used during IO operations and how.
 * @version 0.1
 * @date 2024-07-12
 *
 * @copyright Copyright (c) 2024
 *
 */
#pragma once

#include "file.hpp"
#include "string.hpp"
#include "container/vector.hpp"

namespace CORE_NAMESPACE {

// ///////// //
// Text file //
// ///////// //
template<typename IFS>
class TextInBase : public virtual FileIn<String, IFS> {
  public:
    using FileIn<String, IFS>::FileIn;

    virtual String read(const uint64 size) override {
        char* buffer = new (BaseMemoryTags.Temp) char[size];
        auto  n      = std::ifstream::readsome(buffer, size);
        auto  result = n ? String(std::string(buffer, n)) : "";
        del(buffer);
        return result;
    }
};
template<typename OFS>
class TextOutBase : public virtual FileOut<String, OFS> {
  public:
    using FileOut<String, OFS>::FileOut;

    virtual void write(const String& data) override {
        OFS::write(data.data(), data.size());
    }

    template<typename... Args>
    void write(const Args&... data) {
        const auto all_data = String::build(data...);
        OFS::write(all_data.data(), all_data.size());
    }

    template<typename... Args>
    void write_ln(const Args&... data) {
        write(data..., "\n");
    }
};

/**
 * @brief Text input file type. Decodes stream into text.
 */
typedef TextInBase<std::ifstream>                                   TextIn;
/**
 * @brief Text output file type. Encodes text for stream.
 */
typedef TextOutBase<std::ofstream>                                  TextOut;
/**
 * @brief Text IO file type. Encodes / Decodes text from / to stream.
 */
typedef FileIO<TextInBase<std::fstream>, TextOutBase<std::fstream>> TextIO;

// ////////////// //
// TextLines file //
// ////////////// //
/**
 * @brief Per line text input file. Decodes input stream as text, one line
 * at a time.
 *
 */
template<typename IFS>
class LinesInBase : public FileIn<Vector<String>, IFS> {
  public:
    using FileIn<Vector<String>, IFS>::FileIn;

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
template<typename OFS>
class LinesOutBase : public FileOut<Vector<String>, OFS> {
  public:
    using FileOut<Vector<String>, OFS>::FileOut;

    void write(const Vector<String>& data) override {
        for (const auto& line : data)
            OFS::write(line.data(), line.size());
    }
};

/**
 * @brief Per line text input file type. Decodes stream into text, line by line.
 */
typedef LinesInBase<std::ifstream>                                    LinesIn;
/**
 * @brief Per line text output file type. Encodes text lines for stream.
 */
typedef LinesOutBase<std::ofstream>                                   LinesOut;
/**
 * @brief Per line text IO file type. Encodes / Decodes text lines from / to
 * stream.
 */
typedef FileIO<LinesInBase<std::fstream>, LinesOutBase<std::fstream>> LinesIO;

// /////////// //
// Binary file //
// /////////// //
template<typename IFS>
class BinaryInBase : public FileIn<Vector<byte>, IFS> {
  public:
    using FileIn<Vector<byte>, IFS>::FileIn;

    virtual Vector<byte> read(const uint64 size) override {
        Vector<byte> buffer(size);
        this->seekg(0);
        IFS::read(buffer.data(), size);
        return buffer;
    }
};
template<typename OFS>
class BinaryOutBase : public FileOut<Vector<byte>, OFS> {
  public:
    using FileOut<Vector<byte>, OFS>::FileOut;

    virtual void write(const Vector<char>& data) override {
        OFS::write(data.data(), data.size());
    }
};

/**
 * @brief Binary input file type. Reads input stream byte by byte
 */
typedef BinaryInBase<std::ifstream>  BinaryIn;
/**
 * @brief Binary output file type. Writes arbitrary binary data to stream.
 */
typedef BinaryOutBase<std::ofstream> BinaryOut;
/**
 * @brief Binary IO file type. Operates on binary data, one byte at a time.
 */
typedef FileIO<BinaryInBase<std::fstream>, BinaryOutBase<std::fstream>>
    BinaryIO;

}; // namespace CORE_NAMESPACE