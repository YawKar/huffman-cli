#include "compression_coordinator.hpp"

#include <fmt/core.h>
#include <fmt/ranges.h>

#include <boost/filesystem.hpp>
#include <fstream>
#include <functional>
#include <iomanip>
#include <iostream>
#include <stdexcept>

#include "../coder/encoder.hpp"
#include "../huffman/huffman.hpp"

namespace fs = boost::filesystem;

void compression_coordinator::perform_compression(const std::string& input_, const std::string& output_,
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

  huffman algorithm;
  if (verbose) std::cout << "Initializing Huffman..." << std::endl;
  algorithm.initialize_data(data);

  if (verbose) std::cout << "Calculating frequencies..." << std::endl;
  algorithm.calculate_frequencies();
  if (verbose) {
    std::cout << "Frequencies (byte, frequency): " << fmt::format("{}", fmt::join(algorithm.get_frequencies(), ","))
              << std::endl;
  }

  if (verbose) std::cout << "Sorting frequencies..." << std::endl;
  algorithm.sort_frequencies();
  if (verbose) {
    std::cout << "Sorted frequencies (byte, frequency): "
              << fmt::format("{}", fmt::join(algorithm.get_sorted_frequencies(), ",")) << std::endl;
  }

  if (verbose) std::cout << "Building Huffman tree..." << std::endl;
  algorithm.build_tree();
  if (verbose) {
    std::cout << "Huffman tree:" << std::endl;
    auto root_copy = algorithm.get_tree_copy();
    int indent_size = std::max<int>(4, static_cast<int>(std::log10(root_copy->frequency_sum)) + 1);
    std::function<void(std::shared_ptr<huffman::node>, uint8_t)> deep_print =
        [&deep_print, indent_size](std::shared_ptr<huffman::node> root, uint8_t depth) {
          if (root->right_child) {
            deep_print(root->right_child, depth + 1);
          }
          if (root->left_child == nullptr && root->right_child == nullptr) {
            std::cout << std::string(indent_size * depth, ' ')
                      << fmt::format("{} ({})", root->frequency_sum, root->byte) << std::endl;
          } else {
            std::cout << std::string(indent_size * depth, ' ') << root->frequency_sum << std::endl;
          }
          if (root->left_child) {
            deep_print(root->left_child, depth + 1);
          }
        };
    deep_print(root_copy, 0);
  }

  if (verbose) std::cout << "Compiling codebook..." << std::endl;
  algorithm.compile_codebook();
  if (verbose) {
    const auto& codebook = algorithm.get_codebook();
    auto subcode = [](const std::bitset<255>& bitset, uint8_t length) -> std::string {
      std::string result;
      result.reserve(length);
      for (uint8_t i = 0; i < length; ++i) {
        result.push_back(bitset.test(i) ? '1' : '0');
      }
      return result;
    };
    std::cout << fmt::format("Codebook (total size is {}):", codebook.size()) << std::endl;
    for (const auto& [original_byte, entry] : codebook) {
      const auto& [length, code] = entry;
      std::cout << std::setw(12) << fmt::format("{} ({})", original_byte, length) << std::setw(0) << std::string(4, ' ')
                << subcode(code, length) << std::endl;
    }
  }

  if (verbose) std::cout << "Encoding data..." << std::endl;
  encoder coder;
  std::vector<uint8_t> encoded_data = coder.encode_data_with_codebook(data, algorithm.get_codebook());
  if (verbose) {
    std::cout << fmt::format("Encoded data size: {}", encoded_data.size()) << std::endl;
    std::cout << fmt::format("Compressed {:.2f}%",
                             100.0 * (static_cast<double>(data.size()) - static_cast<double>(encoded_data.size())) /
                                 static_cast<double>(data.size()))
              << std::endl;
  }

  if (verbose) std::cout << "Outputing data..." << std::endl;
  output_encoded_data(encoded_data);
}

std::vector<uint8_t> compression_coordinator::read_data_from_input() {
  std::vector<uint8_t> data;
  if (input == "stdin") {
    char c;
    while (!std::cin.eof() && std::cin.get(c)) {
      data.push_back(c);
    }
    data.pop_back();
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
      data.pop_back();
    }
  }
  return data;
}

void compression_coordinator::output_encoded_data(const std::vector<uint8_t>& data) {
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

void compression_coordinator::validate_options(const std::string& input_, const std::string& output_) {
  if (input_ != "stdin" && !fs::exists(input_)) {
    throw std::runtime_error(fmt::format("Error: input file {} doesn't exist", input_));
  }
  if (output_ != "stdout" && fs::exists(output_)) {
    throw std::runtime_error(fmt::format("Error: output file {} already exists", output_));
  }
}