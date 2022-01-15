// add-lib.cc
// Created by Isaac Yee

// Standard library headers
#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

// Internal headers
#include "version.hh"

// External library headers
#include "argparse/argparse.hpp"
#include "nlohmann/json.hpp"

using json = nlohmann::json;

std::string read_file(const char *filename) {
    std::ifstream i(filename, std::fstream::in);
    std::ostringstream oss;
    std::string line;
    while (std::getline(i, line)) {
        oss << line << '\n';
    }
    i.close();
    return oss.str();
}

void parse_args(argparse::ArgumentParser *args, int argc, char *argv[]) {
    args->add_argument("libname").help("the name of the library");
    args->add_argument("-p", "--libpath")
        .default_value("")
        .help("the path to the library. Ignored if --github option is set.");
    args->add_argument("-g", "--github")
        .default_value("")
        .help("clones the library from the specified URL");

    try {
        args->parse_args(argc, argv);
    } catch (const std::runtime_error &err) {
        std::cerr << err.what() << '\n';
        std::cerr << *args;
        std::exit(1);
    }
}

int main(int argc, char *argv[]) {
    argparse::ArgumentParser args("add-lib", CPM_VERSION);
    parse_args(&args, argc, argv);

    if (!std::filesystem::exists(std::filesystem::path{"./.cpm/config.json"})) {
        std::cerr << "add-lib: Config file not found\n";
        std::exit(1);
    }

    // Clone library to lib folder
    std::string path;

    if (args.is_used("--github")) {
        path = args.get("--github");
        std::ostringstream clone_cmd;
        clone_cmd << "cd lib && git clone " << path << " ./"
                  << args.get("libname");
        std::system(clone_cmd.str().c_str());
    } else if (args.is_used("--libpath")) {
        path = args.get("--libpath");
        std::ostringstream cp_cmd;
        cp_cmd << "cd lib && cp -R " << path << " ./" << args.get("libname");
        std::system(cp_cmd.str().c_str());
    } else {
        std::cerr << "add-lib: Neither --github or --libpath is set\n";
        std::exit(1);
    }

    // Add library to config
    json config_contents = json::parse(read_file("./cpm/config.json"));
    config_contents.get("libs");
    return EXIT_SUCCESS;
}
