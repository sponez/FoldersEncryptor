#include "EncryptingWriter.h"

namespace fe {
    void EncryptingWriter::writeSalt(const unsigned char* salt) {
        Chunk saltChunk = Chunk::salt(salt);
        serializeAndWrite(saltChunk);
    }

    void EncryptingWriter::writeHeader(const unsigned char* header) {
        Chunk headerChunk = Chunk::header(header);
        serializeAndWrite(headerChunk);
    }

    void EncryptingWriter::writeFile(const std::filesystem::path rootPath, const std::filesystem::path& filePath, const std::size_t& bufferSize) {
        std::ifstream in(filePath, std::ios::binary);
        if (!in) {
            throw std::runtime_error("Error occurs while opening file");
        }

        std::filesystem::path relativeFilePath = std::filesystem::relative(filePath, rootPath);
        Chunk filePathChunk = Chunk::filePath(relativeFilePath);
        serializeAndWrite(filePathChunk);

        while (in.peek() != EOF) {
            std::vector<unsigned char> plain(bufferSize);

            in.read(reinterpret_cast<char*>(plain.data()), bufferSize);
            std::streamsize readLen = in.gcount();
            if (readLen <= 0) {
                break;
            } else if (readLen > bufferSize) {
                throw std::runtime_error("Read more bytes than expected — potential overflow");
            }

            plain.resize(readLen);
            Chunk contentBlock = Chunk::fileBlock(plain);
            serializeAndWrite(contentBlock);
        }

        Chunk fileEnd = Chunk::END_OF_FILE;
        serializeAndWrite(fileEnd);

        in.close();
    }

    void EncryptingWriter::addEndTag() {
        Chunk streamEnd = Chunk::END_OF_STREAM;
        serializeAndWrite(streamEnd);
    }

    void EncryptingWriter::serializeAndWrite(Chunk& chunk) {
        SerializedChunk serializedChunk = chunkSerializer.serialize(chunk);
        outStream.write(reinterpret_cast<const char*>(serializedChunk.data()), serializedChunk.size());
    }
}