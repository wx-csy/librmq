SRCS :=	hardcode.cpp main.cpp
CXXFLAGS := -fsanitize=undefined -fsanitize=address

.PHONY : clean

librmq-test : $(SRCS) $(shell find . -name '*.h')
	g++ $(SRCS) -O3 -std=c++14 -ggdb -o $@

hardcode.cpp : prep
	./prep > $@

prep : prep.cpp
	g++ $< $(CXXFLAGS) -O3 -std=c++14 -ggdb -o $@	

clean :
	rm -rf librmq-test hardcode.cpp prep

.DEFAULT_GOAL := librmq-test
.DELETE_ON_ERROR :
