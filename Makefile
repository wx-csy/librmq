SRCS :=	hardcode.cpp naive.cpp st.cpp ind.cpp main.cpp
CXXFLAGS := -fsanitize=undefined -fsanitize=address

.PHONY : clean

librmq-test : $(SRCS) librmq.h
	g++ $(SRCS) $(CXXFLAGS) -O3 -std=c++14 -ggdb -o $@

hardcode.cpp : prep
	./prep > $@

prep : prep.cpp
	g++ $< -O3 -std=c++14 -ggdb -o $@	

clean :
	rm -rf librmq-test hardcode.cpp prep

.DEFAULT_GOAL := librmq-test