#include "decompression_coordinator.hpp"

#include <fmt/core.h>
#include <fmt/ranges.h>

#include <boost/filesystem.hpp>
#include <fstream>
#include <functional>
#include <iomanip>
#include <iostream>
#include <stdexcept>

#include "../coder/decoder.hpp"

namespace fs = boost::filesystem;

void decompression_coordinator::perform_decompression(const std::string& input_, const std::string& output_,
                                                      bool ignore_empty_, bool verbose_) {
  if (verbose_) std::cout << "Validating options..." << std::endl;
  validate_options(input_, output_);
  if (verbose_) std::cout << "Validation passed!" << std::endl << std::endl;

  this->input = input_;
  this->output = output_;
  this->ignore_empty = ignore_empty_;
  this->verbose = verbose_;

  if (verbose) {
    std::cout << "Your configuration:" << std::endl;
    std::cout << "input source: " << input << std::endl;
    std::cout << "output source: " << output << std::endl;
    std::cout << "ignore empty data: " << std::boolalpha << ignore_empty << std::endl;
    std::cout << "verbose: " << std::boolalpha << verbose << std::endl;
    std::cout << std::endl;
  }

  std::vector<uint8_t> data = read_data_from_input();
  if (verbose) std::cout << "Total data bytes: " << data.size() << std::endl << std::endl;
  if (data.empty()) {
    if (verbose) {
      std::cout << "Input data is empty!" << std::endl;
    }
    if (ignore_empty) {
      return;
    } else {
      throw std::runtime_error("Error: input data is empty, consider using --ignore-empty to exit peacefully with 0");
    }
  }

  if (verbose) std::cout << "Decoding data..." << std::endl;
  decoder decoder;
  std::vector<uint8_t> decoded_data = decoder.decode_data(data);
  if (verbose) {
    std::cout << fmt::format("Decoded data size: {}", decoded_data.size()) << std::endl;
    std::cout << fmt::format("Decompressed {:.2f}%",
                             100.0 * (static_cast<double>(decoded_data.size()) - static_cast<double>(data.size())) /
                                 static_cast<double>(data.size()))
              << std::endl;
  }

  if (verbose) std::cout << "Outputing data..." << std::endl;
  output_decoded_data(decoded_data);
}

std::vector<uint8_t> decompression_coordinator::read_data_from_input() {
  std::vector<uint8_t> data;
  if (input == "stdin") {
    char c;
    while (!std::cin.eof() && std::cin.get(c)) {
      data.push_back(c);
    }
  } else {
    std::ifstream file(input);
    file.seekg(0, std::ios::end);
    if (file.tellg() > 0) {
      file.clear();
      file.seekg(0, std::ios::beg);
      char c;
      while (!file.eof() && file.get(c)) {
        data.push_back(c);
      }
    }
  }
  return data;
}

void decompression_coordinator::output_decoded_data(const std::vector<uint8_t>& data) {
  if (output == "stdout") {
    for (const auto byte : data) {
      std::cout << byte;
    }
  } else {
    std::ofstream out(output);
    for (const auto byte : data) {
      out << byte;
    }
  }
}

void decompression_coordinator::validate_options(const std::string& input_, const std::string& output_) {
  if (input_ != "stdin" && !fs::exists(input_)) {
    throw std::runtime_error(fmt::format("Error: input file {} doesn't exist", input_));
  }
  if (output_ != "stdout" && fs::exists(output_)) {
    throw std::runtime_error(fmt::format("Error: output file {} already exists", output_));
  }
}