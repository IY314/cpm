.DEFAULT_GOAL := all

.PHONY : all

all: create-c-proj

create-c-proj:
	@mkdir -p bin
	@${CXX} src/create-c-proj.cc \
		-Ilib/argparse/include \
		-Ilib/json/include \
		-Wall -Werror -Wextra -O2 \
		-o bin/create-c-proj \
		-std=gnu++17
	@echo "create-c-proj: Target built successfully"
