// create_c++_template.cpp : このファイルには 'main' 関数が含まれています。プログラム実行の開始と終了がそこで行われます。
//

#include <string>
#include <algorithm>
#include <chrono>
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cassert>

std::string str_toupper(std::string str);
bool fileExists(std::string str);

int main(int argc, char *argv[])
{
	std::string file_name;
	std::string file_dir;

	if (argc == 2) {
		file_name = argv[1];
		file_dir = "./";
	}
	else if(argc == 3){
		file_name = argv[1];
		file_dir = argv[2];
	}
	else {
		std::cerr << "command error\n";
		std::cout
			<< "command" << " [file name]" << std::endl
			<< "command" << " [file name] [directory]" << std::endl;
		return 1;
	}

	for (const auto &c : file_name) {
		if (c == '\\' || c == '/' || c == ':' || c == '*' || c == '?' || c == '\"' || c == '<' || c == '>' || c == '|') {
			std::cerr << "The following characters cannot be used in file names:\n" << "\\ / : * ? \" < > |\n";
			return 2;
		}
	}

	if (file_dir.back() != '/') {
		file_dir += '/';
	}

	if (fileExists(file_dir + file_name) || fileExists(file_dir + file_name + ".hpp") || fileExists(file_dir + file_name + ".cpp")) {
		std::cout << file_name << " that already exists cannot be specified.\n";
		return 3;
	}

	std::string include_guard = str_toupper(file_name + "_HPP");

	time_t t = time(NULL);
	tm now;
	errno_t error;
	error = localtime_s(&now, &t);

	if (error != 0) { 
		std::cout << "Could not get current date.\n";
		return 4;
	}

	std::string date = std::to_string(now.tm_year) + std::to_string(now.tm_mon) + std::to_string(now.tm_mday);

	std::string hpp_template =
		"#ifndef " + include_guard + "\n"
		"#define " + include_guard + "\n"
		"/*******************************************\n"
		" * " + file_name + "ver1.0 " + date + "\n"
		" * This program is ...\n"
		" *\n"
		" * [Dependency]\n"
		" *\n"
		" * [Note]\n"
		" *\n"
		" * [Author]\n"
		" * Yuta Uehara\n"
		" *\n"
		" * [Change history]\n"
		" * ver1.0 " + date + "The first version\n"
		" ******************************************/\n";

	std::ofstream writing_hpp_file;
	std::ofstream writing_cpp_file;

	writing_hpp_file.open(file_dir + file_name + ".hpp", std::ios::out);
	writing_cpp_file.open(file_dir + file_name + ".cpp", std::ios::out);

	if (!writing_hpp_file.is_open() || !writing_cpp_file.is_open()) {
		std::cerr << "Could not create file. The directory(" << file_dir << ") that does not exist may have been specified.\n";
		return 5;
	}
	else {
		writing_hpp_file << hpp_template;
		writing_cpp_file << "#include " + file_name + ".hpp //ver1.0\n";
	}

	writing_cpp_file.close();
	writing_cpp_file.close();

	return 0;
}

std::string str_toupper(std::string str) {
	std::transform(str.begin(), str.end(), str.begin(), [](unsigned char c) { return std::toupper(c); });
	return str;
}

bool fileExists(std::string str) {
	std::ifstream fs(str);
	return fs.is_open();
}