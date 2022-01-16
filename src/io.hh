#ifndef IO_HH
#define IO_HH

#include <string>
#include <sstream>
#include <fstream>

std::string read_file(std::string filename) {
    std::ifstream i(filename, std::fstream::in);
    std::ostringstream oss;
    std::string line;
    while (std::getline(i, line)) {
        oss << line << '\n';
    }
    i.close();
    return oss.str();
}

void write_file(std::string filename, std::string contents, bool newline = false) {
    std::ofstream o(filename, std::fstream::out);
    o << contents;
    if (newline) o << '\n';
}

#endif
