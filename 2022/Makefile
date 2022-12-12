NAME     = ad12-2
CXX      = clang++ 
CC       = $(CXX)
CXXFLAGS = -W -Wall -O2 -std=c++20

.PHONY: default clean run

default: $(NAME).cpp test

run: $(NAME)
	@if [ -f $(NAME).in ]; then ./$(NAME) < $(NAME).in; else ./$(NAME); fi
	@if [ -f $(NAME).exp ]; then cat $(NAME).exp; fi

test: $(NAME)
	@if [ -f $(NAME)-test.in ]; then ./$(NAME) < $(NAME)-test.in; else ./$(NAME); fi
	@if [ -f $(NAME)-test.exp ]; then cat $(NAME)-test.exp; fi

$(NAME): $(NAME).o

$(NAME).cpp:
	@if [ ! -f $(NAME).cpp ]; then cp ad.cpp $(NAME).cpp; fi

clean:
	$(RM) $(NAME) $(NAME).o
	$(RM) mkerr olderr
