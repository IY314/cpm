#include <filesystem>
#include <string>
#include "argparse/argparse.hpp"

#define OPTIONS ""

struct ProgArgs : public argparse::Args {
    std::string &project_path = arg("The path of the project");
    std::string &project_name = arg("The name of the project");
    bool &git = flag("g,git", "Whether or not to initialize a Git repo");
};

int main(int argc, char *argv[]) {
    ProgArgs args = argparse::parse<ProgArgs>(argc, argv);
    args.print();
}
