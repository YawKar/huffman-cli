#ifndef DECODER_HPP
#define DECODER_HPP
#include <bitset>
#include <cstdint>
#include <map>
#include <vector>

/// @brief Basic decoder.
class decoder {
 public:
  decoder();

  /// @brief {total_codes:uint8_t}[length(code[i]):uint8_t][code:[...uint8_t]][!encoded_data!][padding_bits:uint8_t] -
  /// total_codes from 0 to 255, but there's at least 1 code, so decoder need to add 1 to the total_codes to get the
  /// actual number of codes.
  /// @param data
  /// @return
  std::vector<uint8_t> decode_data(const std::vector<uint8_t>& data);
};

#endif  // DECODER_HPP