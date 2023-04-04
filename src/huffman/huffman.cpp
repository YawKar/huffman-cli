#include "huffman.hpp"

#include <fmt/core.h>

#include <algorithm>
#include <functional>
#include <queue>
#include <stack>
#include <stdexcept>

huffman::huffman() : m_state(state::uninitialized) {}

void huffman::initialize_data(const std::vector<uint8_t>& data) {
  validate_desired_state(state::initialized);
  this->m_data = data;
  m_state = state::initialized;
}

void huffman::initialize_data(std::vector<uint8_t>&& data) {
  validate_desired_state(state::initialized);
  this->m_data = std::move(data);
  m_state = state::initialized;
}

void huffman::calculate_frequencies() {
  validate_desired_state(state::unsorted_frequencies);
  for (const auto byte : m_data) {
    ++m_frequencies[byte];
  }
  m_state = state::unsorted_frequencies;
}

void huffman::sort_frequencies() {
  validate_desired_state(state::sorted_frequencies);
  for (const auto& entry : m_frequencies) {
    m_sorted_frequencies.emplace_back(entry.first, entry.second);
  }
  std::sort(m_sorted_frequencies.begin(), m_sorted_frequencies.end(),
            [](const auto& pair1, const auto& pair2) { return pair1.second < pair2.second; });
  m_state = state::sorted_frequencies;
}

void huffman::build_tree() {
  validate_desired_state(state::built_tree);
  std::priority_queue<std::shared_ptr<node>, std::vector<std::shared_ptr<node>>, node_comparator> all_nodes;
  for (const auto& entry : m_sorted_frequencies) {
    all_nodes.push(std::shared_ptr<node>(new node(entry.first, entry.second, nullptr, nullptr)));
  }
  while (all_nodes.size() > 1) {
    auto first = all_nodes.top();
    all_nodes.pop();
    auto second = all_nodes.top();
    all_nodes.pop();
    auto combined = std::shared_ptr<node>(new node(0u, first->frequency_sum + second->frequency_sum, first, second));
    all_nodes.push(combined);
  }
  m_root = all_nodes.top();
  m_state = state::built_tree;
}

std::shared_ptr<huffman::node> huffman::get_tree_copy() {
  std::function<std::shared_ptr<node>(std::shared_ptr<node>)> deep_copy =
      [&deep_copy](std::shared_ptr<node> root) -> std::shared_ptr<node> {
    if (root == nullptr) {
      return nullptr;
    }
    return std::shared_ptr<node>(
        new node(root->byte, root->frequency_sum, deep_copy(root->left_child), deep_copy(root->right_child)));
  };
  return deep_copy(m_root);
}

void huffman::compile_codebook() {
  validate_desired_state(state::compiled_codebook);
  std::bitset<255> current_code;
  std::function<void(std::shared_ptr<node>, uint8_t)> dive_compile_codebook =
      [&current_code, &dive_compile_codebook, &m_codebook = this->m_codebook](std::shared_ptr<node> root, uint8_t pos) {
        if (root->left_child) {
          current_code[pos] = 0;
          dive_compile_codebook(root->left_child, pos + 1u);
        }
        if (root->right_child) {
          current_code[pos] = 1;
          dive_compile_codebook(root->right_child, pos + 1u);
        }
        if (root->left_child == nullptr && root->right_child == nullptr) {
          m_codebook[root->byte] = std::pair<uint8_t, std::bitset<255>>(pos, current_code);
        }
      };
  dive_compile_codebook(m_root, 0);
  m_state = state::compiled_codebook;
}

void huffman::clear_state() {
  m_state = state::uninitialized;
  m_root = nullptr;
  m_data.clear();
  m_data.shrink_to_fit();
  m_frequencies.clear();
  m_sorted_frequencies.clear();
  m_sorted_frequencies.shrink_to_fit();
  m_codebook.clear();
}

void huffman::validate_desired_state(state next_state) {
  if (m_state > next_state) {
    throw std::logic_error(
        fmt::format("Error: attempt to change Huffman state from {} to {}. Consider calling clear_state() perhaps.",
                    static_cast<int>(m_state), static_cast<int>(next_state)));
  } else if (m_state == next_state) {
    throw std::logic_error(
        fmt::format("Error: attempt to re-enter the current Huffman state {}. Consider calling clear_state() perhaps.",
                    static_cast<int>(m_state)));
  }
}

std::vector<uint8_t> huffman::get_data() { return m_data; }

std::map<uint8_t, uint64_t> huffman::get_frequencies() { return m_frequencies; }

std::vector<std::pair<uint8_t, uint64_t>> huffman::get_sorted_frequencies() { return m_sorted_frequencies; }

std::map<uint8_t, std::pair<uint8_t, std::bitset<255>>> huffman::get_codebook() { return m_codebook; }
