#ifndef COMPRESSION_COORDINATOR_HPP
#define COMPRESSION_COORDINATOR_HPP
#include <string>
#include <vector>

class compression_coordinator {
 public:
  void perform_compression(const std::string& input, const std::string& output, bool ignore_empty, bool verbose);

 private:
  std::string input;
  std::string output;
  bool ignore_empty;
  bool verbose;

  void validate_options(const std::string& input, const std::string& output);
  std::vector<uint8_t> read_data_from_input();
  void output_encoded_data(const std::vector<uint8_t>& data);
};

#endif  // COMPRESSION_COORDINATOR_HPP