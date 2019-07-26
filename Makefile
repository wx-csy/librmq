SRCS :=	hardcode.cpp rmq.cpp st.cpp main.cpp

.PHONY : clean

librmq-test : $(SRCS) hardcode.h st.h rmq.h
	g++ $(SRCS) -O3 -ggdb -o $@

hardcode.cpp : prep
	./prep > $@

prep : prep.cpp
	g++ $< -O3 -ggdb -o $@	

clean :
	rm -rf librmq-test hardcode.cpp prep

.DEFAULT_GOAL := librmq-test
