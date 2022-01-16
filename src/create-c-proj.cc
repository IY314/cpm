// create-c-proj.cc
// Made by Isaac Yee

// Standard library headers
#include <string>
#include <sstream>
#include <iostream>

// Internal headers
#include "definitions.hh"
#include "io.hh"

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

void parse_args(argparse::ArgumentParser *args, int argc, char *argv[]) {
    args->add_argument("project_name")
        .help("the name of the project");
    args->add_argument("project_path")
        .help("the path to the project (not including the directory of the project)");
    args->add_argument("-g", "--git")
        .default_value(false)
        .implicit_value(true)
        .help("initializes a Git repo for the project");
    args->add_argument("-x", "--cxx")
        .default_value(false)
        .implicit_value(true)
        .help("initializes a CXX project");
    
    try {
        args->parse_args(argc, argv);
    } catch (const std::runtime_error &err) {
        std::cerr << err.what() << '\n';
        std::cerr << *args;
        std::exit(1);
    }
}

int main(int argc, char *argv[]) {
    argparse::ArgumentParser args("create-c-proj", CPM_VERSION);
    parse_args(&args, argc, argv);

    // Resolve path
    std::ostringstream path_stream;
    path_stream << args.get("project_path") << "/" << args.get("project_name");
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
    if (args["git"] == true)
        std::system("git init");

    // Write default code to main file (.cc if --cxx option is set else .c)
    std::ostringstream main_file_path;
    main_file_path << path << "/src/main." << (args["cxx"] == true ? "cc" : "c");
    write_file(main_file_path.str(), args["cxx"] == true ? CXX_DEFAULT_CODE : C_DEFAULT_CODE);

    // Write config to .cpm config file
    std::ostringstream cpm_config_file_path;
    cpm_config_file_path << path << "/.cpm/config.json";
    json config = {
        {"libs", json::array()},
        {"compile-flags", {"-Wall", "-Werror", std::string("-o bin/") + args.get("project_name")}}
    };
    write_file(cpm_config_file_path.str(), config.dump(4), true);
    return EXIT_SUCCESS;
}
