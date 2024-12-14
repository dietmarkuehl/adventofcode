NAME     = ad14-2
CXX      = clang++ 
CC       = $(CXX)
CXXFLAGS = -W -Wall -g -std=c++20 -O3 -fsanitize=undefined
LDFLAGS = -fsanitize=undefined

.PHONY: default clean run add

default: $(NAME).cpp test

run: $(NAME)
	@echo running ./$(NAME) with actual inputs
	@if [ -f $(NAME).in ]; then ./$(NAME) < $(NAME).in; else ./$(NAME); fi
	@if [ -f $(NAME).exp ]; then cat $(NAME).exp; fi

add:
	git add $(NAME).cpp $(NAME).in $(NAME)-test.in $(NAME)-test.exp $(NAME).exp

test: $(NAME)
	@echo running ./$(NAME) with test inputs
	@if [ -f $(NAME)-test.in ]; then ./$(NAME) < $(NAME)-test.in; else ./$(NAME); fi
	@if [ -f $(NAME)-test.exp ]; then cat $(NAME)-test.exp; fi

$(NAME): $(NAME).o

$(NAME).cpp:
	@if [ ! -f $(NAME).cpp ]; then cp ad.cpp $(NAME).cpp; touch $(NAME).in $(NAME)-test.in $(NAME)-test.exp; fi

clean:
	$(RM) $(NAME) $(NAME).o
	$(RM) mkerr olderr
