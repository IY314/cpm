.DEFAULT_GOAL := all

.PHONY : all

all: create-c-proj add-lib

create-c-proj:
	@echo "create-c-proj: Building target..."
	@mkdir -p bin
	@${CXX} src/create-c-proj.cc \
		-Ilib/argparse/include \
		-Ilib/json/include \
		-Wall -Werror -Wextra -O2 \
		-o bin/create-c-proj \
		-std=gnu++17
	@echo "create-c-proj: Target built successfully"

add-lib:
	@echo "add-lib: Building target..."
	@mkdir -p bin
	@${CXX} src/add-lib.cc \
		-Ilib/argparse/include \
		-Wall -Werror -Wextra -O2 \
		-o bin/add-lib \
		-std=gnu++17
	@echo "add-lib: Target built successfully"
