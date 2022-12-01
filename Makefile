NAME     = ad1
CXX      = /opt/llvm-15.0.4/bin/clang++ 
CXX      = clang++ 
CC       = $(CXX)
CXXFLAGS = -W -Wall -O2 -std=c++20

.PHONY: default clean run

default: $(NAME).cpp run

run: $(NAME)
	@if [ -f $(NAME).in ]; then ./$(NAME) < $(NAME).in; else ./$(NAME); fi

$(NAME): $(NAME).o

$(NAME).cpp:
	@if [ ! -f $(NAME).cpp ]; then cp ad.cpp $(NAME).cpp; fi

clean:
	$(RM) $(NAME) $(NAME).o
	$(RM) mkerr olderr
