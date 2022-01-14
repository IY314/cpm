// create-c-proj.cc
// Made by Isaac Yee

// Standard library headers
#include <filesystem>
#include <string>
#include <sstream>
#include <fstream>

// External library headers
#include "argparse/argparse.hpp"
#include "nlohmann/json.hpp"

/*
The C code, when written to the file, reads:

#include <stdio.h>

int main() {
    printf("Hello World\n");
    return 0;
}
*/
#define C_DEFAULT_CODE "#include <stdio.h>\n\nint main() {\n    printf(\"Hello World\\n\");\n    return 0;\n}\n"

/*
The C++ code, when written to the file, reads:

#include <iostream>

int main() {
    std::cout << "Hello World\n";
    return 0;
}
*/
#define CXX_DEFAULT_CODE "#include <iostream>\n\nint main() {\n    std::cout << \"Hello World\\n\";\n    return 0;\n}\n"

using json = nlohmann::json;

int main(int argc, char *argv[]) {
    argparse::ArgumentParser args("create-c-proj");
    args.add_argument("project_name");
    args.add_argument("project_path");
    args.add_argument("-g", "--git")
        .default_value(false)
        .implicit_value(true);
    
    try {
        args.parse_args(argc, argv);
    } catch (const std::runtime_error &err) {
        std::cerr << err.what() << '\n';
        std::cerr << args;
        std::exit(1);
    }

    // Resolve path
    std::ostringstream path_stream;
    path_stream << args["project_path"] << "/" << args["project_name"];
    std::string path = path_stream.str();

    // Build and execute mkdir commands
    std::ostringstream mkdir_root_cmd;
    std::string subdirs[] = {{"src"}, {"bin"}, {"lib"}, {"include"}, {".cpm"}};
    mkdir_root_cmd << "mkdir -p " << path;
    for (std::string dir : subdirs) {
        std::ostringstream mkdir_cmd;
        mkdir_cmd << mkdir_root_cmd.str() << "/" << dir;
        std::system(mkdir_cmd.str().c_str());
    }

    // Build and execute cd command
    std::ostringstream cd_cmd;
    cd_cmd << "cd " << path;
    std::system(cd_cmd.str().c_str());

    // If option set, initialize Git repo
    if (args["git"])
        std::system("git init");

    // Write default code to main file (.cc if --cxx option is set else .c)
    std::ostringstream main_file_path;
    main_file_path << path << "/src/main." << (args["cxx"] ? "cc" : "c");
    std::ofstream main_file(main_file_path.str());
    main_file << (args["cxx"] ? CXX_DEFAULT_CODE : C_DEFAULT_CODE);
    main_file.close();

    // Write config to .cpm config file
    std::ostringstream cpm_config_file_path;
    cpm_config_file_path << path << "/.cpm/config.json";
    std::ofstream cpm_config_file(cpm_config_file_path.str());
    json config = {
        {"libs", json::array()},
        {"compile-flags", {"-Wall", "-Werror", std::string("-o bin/") + args["project_name"]}},
        {"include-dirs", json::array()}
    };
    cpm_config_file << config.dump(4);
    cpm_config_file.close();
}
