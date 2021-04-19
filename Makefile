CC = g++
CF = -std=c++11 -g -ftrapv -fsanitize=undefined -Wall -Werror -Wformat-security -Wignored-qualifiers -Winit-self -Wswitch-default -Wfloat-equal -Wshadow -Wpointer-arith -Wtype-limits -Wempty-body -Wlogical-op -Wmissing-field-initializers -Wcast-qual -Wwrite-strings -lm -L.

CPPFILES = $(shell find src/ModelSQL -name '*.cpp')
HPPFILES = $(shell find headers/ModelSQL -name '*.hpp')
OFILES = $(addprefix obj/ModelSQL/, $(CPPFILES:src/ModelSQL/%.cpp=%.o))

BINFILES = client server

all: $(BINFILES)

obj:
	@mkdir -p obj
	@mkdir -p obj/ModelSQL

%: src/%.cpp $(OFILES) $(HPPFILES)
	@$(CC) $(CF) $< $(OFILES) -I headers/ModelSQL -o $@
	@echo '$@ compiled'

obj/ModelSQL/%.o: src/ModelSQL/%.cpp $(HPPFILES) obj
	@$(CC) $(CF) -c $< -o $@
	@echo '$@ compiled'

clean:
	@rm -rf obj

fclean: clean
	@rm -f $(BINFILES)

re: fclean all

.PHONY: all clean fclean re
.SECONDARY: $(OFILES)