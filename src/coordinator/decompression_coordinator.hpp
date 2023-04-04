#ifndef DECOMPRESSION_COORDINATOR_HPP
#define DECOMPRESSION_COORDINATOR_HPP
#include <string>
#include <vector>

class decompression_coordinator {
 public:
  void perform_decompression(const std::string& input, const std::string& output, bool ignore_empty, bool verbose);

 private:
  std::string input;
  std::string output;
  bool ignore_empty;
  bool verbose;

  void validate_options(const std::string& input, const std::string& output);
  std::vector<uint8_t> read_data_from_input();
  void output_decoded_data(const std::vector<uint8_t>& data);
};

#endif  // COMPRESSION_COORDINATOR_HPP