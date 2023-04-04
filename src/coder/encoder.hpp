#ifndef ENCODER_HPP
#define ENCODER_HPP
#include <bitset>
#include <cstdint>
#include <map>
#include <vector>

/// @brief Basic encoder.
class encoder {
 public:
  encoder();

  /// @brief
  /// {total_codes:uint8_t}[{original_byte:uint8_t}{length(code[i]):uint8_t}{code:[...uint8_t]}][!encoded_data!][padding_bits:uint8_t]
  /// - total_codes from 0 to 255, but there's at least 1 code, so decoder need to add 1 to the total_codes to get the
  /// actual number of codes.
  /// @param data
  /// @param codebook
  /// @return
  std::vector<uint8_t> encode_data_with_codebook(
      const std::vector<uint8_t>& data, const std::map<uint8_t, std::pair<uint8_t, std::bitset<255>>>& codebook);
};

#endif  // ENCODER_HPP