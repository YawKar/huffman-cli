#ifndef HUFFMAN_HPP
#define HUFFMAN_HPP
#include <bitset>
#include <cstdint>
#include <map>
#include <memory>
#include <vector>

/// @brief Huffman algorithm implementation for an alphabet containing 256 variations of 1 byte.
class huffman {
 public:
  /// @brief A structure representing a node in the Huffman tree.
  struct node {
    node(uint8_t byte_, uint64_t frequency_sum_, std::shared_ptr<node> left_child_, std::shared_ptr<node> right_child_)
        : byte(byte_), frequency_sum(frequency_sum_), left_child(left_child_), right_child(right_child_){};
    uint8_t byte;
    uint64_t frequency_sum;
    std::shared_ptr<node> left_child;
    std::shared_ptr<node> right_child;
  };

  /// @brief Maximum possible code length in bits for this implementation.
  static const uint8_t MAX_CODE_LENGTH = 255;

  /// @brief Constructs an instance of the Huffman class.
  huffman();

  /// @brief Initializes the Huffman instance with data stored in a vector.
  /// @param data A vector of bytes to be compressed.
  void initialize_data(const std::vector<uint8_t>& data);

  /// @brief Initializes the Huffman instance with data stored in a move-only vector.
  /// @param data A move-only vector of bytes to be compressed.
  void initialize_data(std::vector<uint8_t>&& data);

  /// @brief Calculates the frequency of each byte in the input data.
  void calculate_frequencies();

  /// @brief Sorts the frequencies in ascending order.
  void sort_frequencies();

  /// @brief Builds the Huffman tree using the sorted frequency table.
  void build_tree();

  /// @brief Compiles a codebook that maps each byte to its corresponding variable-length Huffman code.
  void compile_codebook();

  /// @brief Returns a deep copy of the Huffman tree.
  /// @return A shared pointer to the root node of the copied tree.
  std::shared_ptr<node> get_tree_copy();

  /// @brief Returns a copy of the input data vector.
  /// @return A copy of the input data vector.
  std::vector<uint8_t> get_data();

  /// @brief Returns a map that contains the frequency of each byte in the input data.
  /// @return A map that contains the frequency of each byte in the input data.
  std::map<uint8_t, uint64_t> get_frequencies();

  /// @brief Returns a vector of pairs, where each pair contains a byte and its frequency in the input data.
  /// The vector is sorted in ascending order of frequency.
  /// @return A vector of pairs, sorted in ascending order of frequency.
  std::vector<std::pair<uint8_t, uint64_t>> get_sorted_frequencies();

  /// @brief Returns a map that represents the codebook generated by the Huffman coding algorithm
  /// for the current set of input data. The keys of the map are the original bytes in the input data,
  /// and the values are pairs (length, code), where length is the length of the code in bits, and
  /// code is the binary representation of the corresponding code, stored in little-endian order
  /// (i.e., starting from the least significant bit).
  /// @return A map that represents the codebook.
  std::map<uint8_t, std::pair<uint8_t, std::bitset<255>>> get_codebook();

  /// @brief Clears the internal state. After that it's safe to initialize new data and perform Huffman algorithm
  /// pipeline.
  void clear_state();

  static std::shared_ptr<node> build_tree_from_codebook(
      std::map<uint8_t, std::pair<uint8_t, std::bitset<255>>> codebook);

 private:
  enum class state {
    uninitialized,
    initialized,
    unsorted_frequencies,
    sorted_frequencies,
    built_tree,
    compiled_codebook
  };

  struct node_comparator {
    bool operator()(std::shared_ptr<node> a, std::shared_ptr<node> b) const {
      return a->frequency_sum > b->frequency_sum;
    }
  };

  void validate_desired_state(state next_state);

  state m_state;
  std::shared_ptr<node> m_root;
  std::vector<uint8_t> m_data;
  std::map<uint8_t, uint64_t> m_frequencies;
  std::vector<std::pair<uint8_t, uint64_t>> m_sorted_frequencies;
  std::map<uint8_t, std::pair<uint8_t, std::bitset<255>>> m_codebook;
};

#endif  // HUFFMAN_HPP
