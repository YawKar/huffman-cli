# Huffman coding CLI
Huffman coding CLI is a command-line program that implements the Huffman coding algorithm for data compression and decompression. You can use it to reduce the size of your data files while preserving their content.

## How to build and run
To build and run Huffman coding CLI, you need to have a C++ compiler and the CMake build system installed on your system as well as Conan 2.0. Then, follow these steps:

1. Clone this repository
2. `conan install . --output-folder=build --build=missing -s build_type=Release`
3. `cd build`
4. `cmake .. -DCMAKE_TOOLCHAIN_FILE=conan_toolchain.cmake -DCMAKE_BUILD_TYPE=Release`
5. `cmake --build . --config Release`
6. `./huffman --help`

## Usage
To use Huffman coding CLI, run the huffman executable with the desired options. Here are some usage examples:
```bash
# Compress and decompress data from stdin to stdout
echo "Hello world!" | ./huffman -c | ./huffman -d

# Compress data from stdin to a file and decompress it to stdout
echo "Hello file!" | ./huffman -c -o compressed_file && ./huffman -d -i compressed_file

# Compress data from a file and decompress it from stdin to stdout
./huffman -c -i input_file | ./huffman -d

# Compress data from a file and decompress it to another file
./huffman -c -i input_file -o compressed_file && ./huffman -d -i compressed_file
```
For a complete list of options, run `./huffman --help`.

## License
This program is licensed under the [WTFPL](http://www.wtfpl.net). See the LICENSE file for details.
