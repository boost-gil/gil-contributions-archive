
all: gil-test

gil-test: gil-test.cpp
	$(CXX) -O0 -ggdb3  $< -o $@ -I ../../../ `pkg-config --cflags --libs libpng` -ljpeg

.PHONY: gil-test
