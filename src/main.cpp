#include <fmt/core.h>

#include <boost/program_options.hpp>
#include <iostream>

namespace po = boost::program_options;

std::string compile_help_message_header();
std::string compile_version_message();
void compress(bool is_stdin, std::string input_filename, bool is_stdout, std::string output_filename, bool ignore_empty,
              bool verbose, bool dump, std::string dump_filename);
void decompress(bool is_stdin, std::string input_filename, bool is_stdout, std::string output_filename,
                bool ignore_empty, bool verbose, bool dump, std::string dump_filename);

int main(int argc, char* argv[]) {
  po::options_description all_options("Huffman coding CLI options.", 100);
  auto o = all_options.add_options();
  o("help", "print a help message that explains the program's usage and available options");
  o("version", "print the program's version information");
  {
    po::options_description action_options("Action options", 100);
    auto ao = action_options.add_options();
    ao("compress,c",
       "compress the input data and output the compressed data to stdout by default (see '--output' option)");
    ao("decompress,d",
       "decompress the input data and output the decompressed data to stdout by default (see '--output' option)");
    all_options.add(action_options);
  }
  {
    po::options_description io_options("I/O options", 100);
    auto io = io_options.add_options();
    io("input,i", po::value<std::string>()->value_name("<filename>"), "input file name");
    io("input-stdin", "get input from stdin (e.g., through pipe)");
    io("output,o", po::value<std::string>()->value_name("<filename>"),
       "specify the output file name for the compressed or decompressed data");
    all_options.add(io_options);
  }
  {
    po::options_description tweaks_options("Tweaks", 100);
    auto tw = tweaks_options.add_options();
    tw("ignore-empty", "return 0 if input content is empty (don't do anything)");
    tw("verbose,v",
       "print detailed information about the Huffman coding process, including the frequency table and codebook");
    all_options.add(tweaks_options);
  }
  {
    po::options_description debug_options("Debug", 100);
    auto db = debug_options.add_options();
    db("dump-file", po::value<std::string>()->value_name("<filename>"),
       "dump detailed information about the Huffman coding process to the <filename>");
    all_options.add(debug_options);
  }

  po::variables_map vm;
  try {
    auto parsed_options = po::parse_command_line(argc, argv, all_options);
    po::store(parsed_options, vm);
  } catch (const po::unknown_option& e) {
    std::cout << fmt::format("Error: unknown option ({})", e.get_option_name()) << std::endl;
    std::cout << "Use --help to print a help message with the list of available options" << std::endl;
    return 0;
  }

  if (vm.count("help")) {
    std::cout << compile_help_message_header() << std::endl << std::endl << all_options << std::endl;
  } else if (vm.count("version")) {
    std::cout << compile_version_message() << std::endl;
  } else if (!(vm.count("compress") || vm.count("decompress"))) {
    std::cout << fmt::format("Error: action wasn't specified") << std::endl;
    std::cout << "Use --help to print a help message with the list of available options" << std::endl;
    return 0;
  } else if (vm.count("input") && vm.count("input-stdin")) {
    std::cout << fmt::format("Error: only one input is allowed, you specified both (--input, --input-stdin)")
              << std::endl;
    std::cout << "Use --help to print a help message with the list of available options" << std::endl;
    return 0;
  } else if (!(vm.count("input") || vm.count("input-stdin"))) {
    std::cout << fmt::format("Error: you didn't specify input") << std::endl;
    std::cout << "Use --help to print a help message with the list of available options" << std::endl;
    return 0;
  } else if (vm.count("compress")) {
    try {
      bool is_stdin = vm.count("input-stdin");
      std::string input_filename = is_stdin ? "" : vm["input"].as<std::string>();
      bool is_stdout = !vm.count("output");
      std::string output_filename = is_stdout ? "" : vm["output"].as<std::string>();
      bool ignore_empty = vm.count("ignore-empty");
      bool verbose = vm.count("verbose");
      bool dump = vm.count("dump-file");
      std::string dump_filename = dump ? vm["dump-file"].as<std::string>() : "";
      compress(is_stdin, input_filename, is_stdout, output_filename, ignore_empty, verbose, dump, dump_filename);
    } catch (const std::runtime_error& e) {
      std::cout << e.what() << std::endl;
      std::cout << "Use --help to print a help message with the list of available options" << std::endl;
      return 0;
    }
  } else if (vm.count("decompress")) {
    try {
      bool is_stdin = vm.count("input-stdin");
      std::string input_filename = is_stdin ? "" : vm["input"].as<std::string>();
      bool is_stdout = !vm.count("output");
      std::string output_filename = is_stdout ? "" : vm["output"].as<std::string>();
      bool ignore_empty = vm.count("ignore-empty");
      bool verbose = vm.count("verbose");
      bool dump = vm.count("dump-file");
      std::string dump_filename = dump ? vm["dump-file"].as<std::string>() : "";
      decompress(is_stdin, input_filename, is_stdout, output_filename, ignore_empty, verbose, dump, dump_filename);
    } catch (const std::runtime_error& e) {
      std::cout << e.what() << std::endl;
      std::cout << "Use --help to print a help message with the list of available options" << std::endl;
      return 0;
    }
  }
  return 0;
}

std::string compile_help_message_header() {
  std::string brief_description =
      "Huffman coding CLI is a program that allows you to compress and decompress data using the Huffman coding "
      "algorithm.\n"
      "Use the options listed below to customize the program's behavior and achieve your desired output.";
  std::string usage_example = "echo \"Hello world!\" | huffman --input-stdin -c";
  std::string compilation = "Description:\n" + brief_description + "\n\nUsage example: " + usage_example;
  return compilation;
}

std::string compile_version_message() { return "huffman version 0.1.0"; }

void compress(bool is_stdin, std::string input_filename, bool is_stdout, std::string output_filename, bool ignore_empty,
              bool verbose, bool dump, std::string dump_filename) {
  std::cout << is_stdin << input_filename << is_stdout << output_filename << ignore_empty << verbose << dump
            << dump_filename << std::endl;
}

void decompress(bool is_stdin, std::string input_filename, bool is_stdout, std::string output_filename,
                bool ignore_empty, bool verbose, bool dump, std::string dump_filename) {
  std::cout << is_stdin << input_filename << is_stdout << output_filename << ignore_empty << verbose << dump
            << dump_filename << std::endl;
}
