#include "decoder.hpp"

#include <fmt/core.h>

#include <bitset>
#include <cmath>
#include <functional>
#include <iostream>
#include <map>

#include "../huffman/huffman.hpp"

decoder::decoder() {}

std::vector<uint8_t> decoder::decode_data(const std::vector<uint8_t>& data) {
  uint32_t total_codes = data[0] + 1u;
  const uint32_t CODEBOOK_START = 1;

  // Reading codebook

  std::map<uint8_t, std::pair<uint8_t, std::bitset<255>>> codebook;

  auto iterator = data.begin() + CODEBOOK_START;
  for (uint32_t i = 0; i < total_codes; ++i) {
    uint8_t original_byte = *(iterator++);
    uint8_t length = *(iterator++);
    std::bitset<255> code;
    uint8_t code_pos = 0;

    for (uint8_t whole_byte_idx = 0; whole_byte_idx < length / 8; ++whole_byte_idx) {
      uint8_t whole_byte = *(iterator++);
      for (uint8_t bit = 0; bit < 8; ++bit) {
        code[code_pos++] = (whole_byte & (1u << bit)) != 0;
      }
    }

    if (length % 8 > 0) {
      uint8_t partial_byte = *(iterator++);
      for (uint8_t bit = 0; bit < length % 8; ++bit) {
        code[code_pos++] = (partial_byte & (1u << bit)) != 0;
      }
    }

    codebook[original_byte] = std::pair<uint8_t, std::bitset<255>>(length, code);
  }

  std::cout << "codebook sz: " << codebook.size() << '\n';

  // Building Huffman tree

  const auto root = huffman::build_tree_from_codebook(codebook);

  std::cout << "Huffman tree:" << std::endl;
  int indent_size = std::max<int>(4, static_cast<int>(std::log10(root->frequency_sum)) + 1);
  std::function<void(std::shared_ptr<huffman::node>, uint8_t)> deep_print =
      [&deep_print, indent_size](std::shared_ptr<huffman::node> rt, uint8_t depth) {
        if (rt->right_child) {
          deep_print(rt->right_child, depth + 1);
        }
        if (rt->left_child == nullptr && rt->right_child == nullptr) {
          std::cout << std::string(indent_size * depth, ' ') << fmt::format("{} ({})", rt->frequency_sum, rt->byte)
                    << std::endl;
        } else {
          std::cout << std::string(indent_size * depth, ' ') << rt->frequency_sum << std::endl;
        }
        if (rt->left_child) {
          deep_print(rt->left_child, depth + 1);
        }
      };
  deep_print(root, 0);

  // Reading data

  std::uint8_t padding_bits = data.back();

  std::vector<uint8_t> decoded_data;
  {
    auto tree_pointer = root;
    uint64_t encoded_data_start_index = iterator - data.begin();
    uint64_t total_encoded_bits = 8u * (data.size() - encoded_data_start_index - 1u) - padding_bits;

    for (uint64_t current_bit_offset = 0; current_bit_offset < total_encoded_bits; ++current_bit_offset) {
      uint8_t in_byte_pos = current_bit_offset % 8u;
      bool bit = (data[encoded_data_start_index + current_bit_offset / 8u] & (1u << in_byte_pos));
      if (bit) {
        tree_pointer = tree_pointer->right_child;
      } else {
        tree_pointer = tree_pointer->left_child;
      }
      if (tree_pointer->left_child == nullptr && tree_pointer->right_child == nullptr) {
        decoded_data.push_back(tree_pointer->byte);
        tree_pointer = root;
      }
    }
  }
  return decoded_data;
}
