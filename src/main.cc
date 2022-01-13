#include <filesystem>
#include <string>
#include <sstream>
#include "argparse/argparse.hpp"

#define OPTIONS ""

struct ProgArgs : public argparse::Args {
    std::string &project_path = arg("The path of the project");
    std::string &project_name = arg("The name of the project");
    bool &git = flag("g,git", "Whether or not to initialize a Git repo");
};

int main(int argc, char *argv[]) {
    ProgArgs args = argparse::parse<ProgArgs>(argc, argv);
    std::ostringstream path_stream;
    path_stream << args.project_path << "/" << args.project_name;
    std::string path = path_stream.str();
    std::ostringstream mkdir_root_cmd;
    std::string subdirs[] = {{"src"}, {"bin"}, {"lib"}, {"inc"}};
    mkdir_root_cmd << "mkdir -p " << path;
    for (std::string dir : subdirs) {
        std::ostringstream mkdir_cmd;
        mkdir_cmd << mkdir_root_cmd.str() << "/" << dir;
        std::system(mkdir_cmd.str().c_str());
    }
    std::ostringstream cd_cmd;
    cd_cmd << "cd " << path;
    std::system(cd_cmd.str().c_str());
    if (args.git)
        std::system("git init");
}
