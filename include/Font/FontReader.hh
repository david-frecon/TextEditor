#include <fstream>
#include <iostream>
#include <string>

class FontReader {
public:
  FontReader(std::string path) { file = std::ifstream(path, std::ios::binary); }
  ~FontReader() { file.close(); }
  void GoTo(long offset) { file.seekg(offset, std::ios::beg); }
  u_char ReadByte() { return ReadInt<u_char>(); }
  int16_t ReadInt16() { return ReadInt<short>(); }
  u_int16_t ReadUInt16() { return ReadInt<u_int16_t>(); }
  int32_t ReadInt32() { return ReadInt<int32_t>(); }
  u_int32_t ReadUInt32() { return ReadInt<u_int32_t>(); }
  std::string ReadString(int nbByte) {
    std::string tag(nbByte, 'a');
    for (size_t i = 0; i < nbByte; i++) {
      tag[i] = ReadByte();
    }

    return tag;
  }
  void SkipByte() { file.seekg(1, std::ios::cur); }
  void SkipInt16() { file.seekg(2, std::ios::cur); }
  void SkipInt32() { file.seekg(4, std::ios::cur); }
  void SkipBytes(int nbBytes) { file.seekg(nbBytes, std::ios::cur); }
  int Location() { return static_cast<int>(file.tellg()); }

  template <typename T> T ReadInt() {
    constexpr size_t size = sizeof(T);
    unsigned char bytes[size];
    file.read(reinterpret_cast<char *>(bytes), size);
    if (file.gcount() < size)
      throw std::runtime_error("Unexpected end of file");

    T result = 0;
    for (size_t i = 0; i < size; ++i) {
      result = (result << 8) | bytes[i];
    }
    return result;
  }

private:
  std::ifstream file;
};