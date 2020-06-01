#include "argparse.hpp"
#include "fmt/core.h"

#include "tokenizer/tokenizer.h"
#include "analyser/analyser.h"
#include "fmts.hpp"

#include <iostream>
#include <fstream>

std::vector<c0::Token> _tokenize(std::istream& input) {
	c0::Tokenizer tkz(input);
	auto p = tkz.AllTokens();
	if (p.second.has_value()) {
		fmt::print(stderr, "Tokenization error: {}\n", p.second.value());
		exit(2);
	}
	return p.first;
}

void Tokenize(std::istream& input, std::ostream& output) {
	auto v = _tokenize(input);
	for (auto& it : v)
		output << fmt::format("{}\n", it);
	return;
}

void Analyse(std::istream& input, std::ostream& output){
	auto tks = _tokenize(input);
	c0::Analyser analyser(tks);
	auto p = analyser.Analyse();
	if (p.second.has_value()) {
		fmt::print(stderr, "Syntactic analysis error: {}\n", p.second.value());
		exit(2);
	}
	auto v = p.first;
	output << fmt::format("{}", v);
	return;
}

void Binary(std::istream& input, std::ostream& output) {
	auto tks = _tokenize(input);
	c0::Analyser analyser(tks);
	auto p = analyser.Analyse();
	if (p.second.has_value()) {
		fmt::print(stderr, "Syntactic analysis error: {}\n", p.second.value());
		exit(2);
	}
	auto v = p.first;
	v.genbinary(output);
	return;
}

int main(int argc, char** argv) {
	argparse::ArgumentParser program("c0");
	program.add_argument("input")
		.help("speicify the file to be compiled.");
	program.add_argument("-t")
		.default_value(false)
		.implicit_value(true)
		.help("perform tokenization for the input file.");
	program.add_argument("-s")
		.default_value(false)
		.implicit_value(true)
		.help("output assembly code file.");
	program.add_argument("-c")
		.default_value(false)
		.implicit_value(true)
		.help("output binary file.");
	program.add_argument("-o", "--output")
		.required()
		.default_value(std::string("out"))
		.help("specify the output file.");

	try {
		program.parse_args(argc, argv);
	}
	catch (const std::runtime_error& err) {
		fmt::print(stderr, "{}\n\n", err.what());
		program.print_help();
		exit(2);
	}

	auto input_file = program.get<std::string>("input");
	auto output_file = program.get<std::string>("--output");
	std::istream* input;
	std::ostream* output;
	std::ifstream inf;
	std::ofstream outf;
	inf.open(input_file, std::ios::in);
	if (!inf) {
		fmt::print(stderr, "Fail to open {} for reading.\n", input_file);
		exit(2);
	}
	input = &inf;
	
	/*if (input_file != "-") {
		inf.open(input_file, std::ios::in);
		if (!inf) {
			fmt::print(stderr, "Fail to open {} for reading.\n", input_file);
			exit(2);
		}
		input = &inf;
	}
	else
		input = &std::cin;*/
	/*if (output_file != "-") {
		outf.open(output_file, std::ios::out | std::ios::trunc);
		if (!outf) {
			fmt::print(stderr, "Fail to open {} for writing.\n", output_file);
			exit(2);
		}
		output = &outf;
	}
	else
		output = &std::cout;*/
	if (program["-t"] == true && program["-s"] == true 
		|| program["-s"] == true && program["-c"] == true
		|| program["-t"] == true && program["-c"] == true) {
		fmt::print(stderr, "You can only perform tokenization analysis or output assembly or output binary at one time.");
		exit(2);
	}
	if (program["-t"] == true) {
		outf.open(output_file, std::ios::out | std::ios::trunc);
		if (!outf) {
			fmt::print(stderr, "Fail to open {} for writing.\n", output_file);
			exit(2);
		}
		output = &outf;
		Tokenize(*input, *output);
	}
	else if (program["-s"] == true) {
		outf.open(output_file, std::ios::out | std::ios::trunc);
		if (!outf) {
			fmt::print(stderr, "Fail to open {} for writing.\n", output_file);
			exit(2);
		}
		output = &outf;
		Analyse(*input, *output);
	}
	else if (program["-c"] == true) {
		outf.open(output_file, std::ios::binary | std::ios::out | std::ios::trunc);
		if (!outf) {
			fmt::print(stderr, "Fail to open {} for writing.\n", output_file);
			exit(2);
		}
		output = &outf;
		Binary(*input, *output);
	}
	else {
		fmt::print(stderr, "You must choose tokenization analysis or output assembly or output binary at one time.");
		exit(2);
	}
	return 0;
}