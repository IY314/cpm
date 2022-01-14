// add-lib.cc
// Created by Isaac Yee

#include "argparse/argparse.hpp"

struct ProgArgs : public argparse::Args {
    std::string &lib_name = arg("The library's name");
    std::string &github = flag()
};

int main() {

}
