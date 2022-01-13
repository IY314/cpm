all: create-c-proj

create-c-proj:
	@mkdir -p bin
	@g++ src/create-c-proj.cc \
		-Ilib/argparse/include \
		-Ilib/json/include \
		-Wall -Werror \
		-o bin/create-c-proj \
		-std=gnu++17
	@echo "Target built successfully"
