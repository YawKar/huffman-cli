#include "encoder.hpp"

encoder::encoder() {}

std::vector<uint8_t> encoder::encode_data_with_codebook(
    const std::vector<uint8_t>& data, const std::map<uint8_t, std::pair<uint8_t, std::bitset<255>>>& codebook) {
  std::vector<uint8_t> encoded_data;

  // total_codes
  encoded_data.push_back(static_cast<uint8_t>(codebook.size() - 1u));

  for (const auto& [original_byte, entry] : codebook) {
    const auto& [length, code] = entry;

    // length
    encoded_data.push_back(length);
    uint8_t byte_pos = 8;

    // code
    for (uint8_t i = 0; i < length; ++i) {
      const bool bit_value = code.test(i);
      if (bit_value) {
        if (byte_pos == 8) {
          encoded_data.push_back(0x00);
          byte_pos %= 8;
        }
        encoded_data.back() |= (1 << byte_pos);
        byte_pos++;
      } else {
        if (byte_pos == 8) {
          encoded_data.push_back(0x00);
          byte_pos %= 8;
        }
        byte_pos++;
      }
    }
  }

  uint8_t byte_pos = 8;
  for (const auto& byte : data) {
    const auto& code = codebook.at(byte).second;
    const auto length = codebook.at(byte).first;

    for (uint8_t i = 0; i < length; ++i) {
      const bool bit_value = code.test(i);
      if (bit_value) {
        if (byte_pos == 8) {
          encoded_data.push_back(0x00);
          byte_pos %= 8;
        }
        encoded_data.back() |= (1 << byte_pos);
        byte_pos++;
      } else {
        if (byte_pos == 8) {
          encoded_data.push_back(0x00);
          byte_pos %= 8;
        }
        byte_pos++;
      }
    }
  }

  uint8_t padding_bits = (8u - byte_pos) % 8u;  // byte_pos cannot be 0
  encoded_data.push_back(padding_bits);         // last byte denotes padding_bits in the last byte of encoded data

  return encoded_data;
}
