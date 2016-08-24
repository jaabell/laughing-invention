
CC= g++ -O3 -std=c++11
CFLAGS = -c

all: create makedocumentation generate_command_patterns gen_snips

gen_snips:generate_command_patterns gen_snips.py
		@python gen_snips.py >/dev/null 2>&1

generate_command_patterns:makedocumentation generate_command_patterns.cpp
		@$(CC) $(CFLAGS) generate_command_patterns.cpp
		@$(CC) generate_command_patterns.o -o generate_command_patterns;
		@./generate_command_patterns >/dev/null 2>&1
		@rm ./generate_command_patterns ./generate_command_patterns.o

makedocumentation:create feiparser.yy makedocumentation.cpp
		@$(CC) $(CFLAGS) makedocumentation.cpp
		@$(CC) makedocumentation.o -o makedocumentation;
		@./makedocumentation >/dev/null 2>&1
		@rm ./makedocumentation.o ./makedocumentation

create:
		@./create.sh >/dev/null 2>&1
