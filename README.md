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
```
$ ./huffman --help
Description:
Huffman coding CLI is a program that allows you to compress and decompress data using the Huffman coding algorithm.
Use the options listed below to customize the program's behavior and achieve your desired output.

Usage examples:
        echo "Hello world!" | ./huffman -c | ./huffman -d
        echo "Hello file!" | ./huffman -c -o compressed_file && ./huffman -d -i compressed_file
        ./huffman -c -i input_file | ./huffman -d
        ./huffman -c -i input_file -o compressed_file && ./huffman -d -i compressed_file
        ./huffman -c -i input_file -o compressed_file && ./huffman -d -i compressed_file -o decompressed_file


Huffman coding CLI options.:
  --help                                print a help message that explains the program's usage and 
                                        available options
  --version                             print the program's version information

Action options:
  -c [ --compress ]                     compress the input data and output the compressed data to 
                                        stdout by default (see '--output' option)
  -d [ --decompress ]                   decompress the input data and output the decompressed data 
                                        to stdout by default (see '--output' option)

I/O options:
  -i [ --input ] <filename> (=stdin)    input file name (if not specified, stdin will be consumed)
  -o [ --output ] <filename> (=stdout)  specify the output file name for the compressed or 
                                        decompressed data (if not specified, stdout will be used)

Tweaks:
  --ignore-empty                        return 0 if input content is empty (don't do anything)
  -v [ --verbose ]                      print detailed information about the Huffman coding 
                                        process, including the frequency table and codebook

```

## License
This program is licensed under the [WTFPL](http://www.wtfpl.net). See the LICENSE file for details.
