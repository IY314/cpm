// remove-lib.cc
// Created by Isaac Yee

// Standard library headers
#include <string>
#include <vector>
#include <algorithm>

// Internal headers
#include "definitions.hh"
#include "io.hh"

// External library headers
#include "argparse/argparse.hpp"
#include "nlohmann/json.hpp"

using json = nlohmann::json;

void parse_args(argparse::ArgumentParser *args, int argc, char *argv[]) {
    args->add_argument("libname").help("the name of the library");
    args->add_argument("-r", "--rm")
        .default_value(false)
        .implicit_value(true)
        .help("removes the library folder in the filesystem.");

    try {
        args->parse_args(argc, argv);
    } catch (const std::runtime_error &err) {
        std::cerr << err.what() << '\n';
        std::cerr << *args;
        std::exit(1);
    }
}

int main(int argc, char *argv[]) {
    argparse::ArgumentParser args("remove-lib", CPM_VERSION);
    parse_args(&args, argc, argv);

    json config_contents = json::parse(read_file(PATH_TO_CONFIG));
    auto lib_vec = config_contents["libs"].get<std::vector<json>>();
    std::string path;
    lib_vec.erase(std::remove_if(lib_vec.begin(), lib_vec.end(), [args, path] (json j) mutable {
        if (j["name"].get<std::string>() == args.get("libname"))
            path = j["path"].get<std::string>();
            return true;
        return false;
    }), lib_vec.end());
    config_contents["libs"] = lib_vec;
    write_file(PATH_TO_CONFIG, config_contents.dump(4), true);
    if (args["--rm"] == true) {
        std::system(("rm -rf " + path).c_str());
    }
}
