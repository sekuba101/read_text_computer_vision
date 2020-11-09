CC = g++
CFLAGS = -g -Wall
src = $(wildcard *.cpp)
obj = $(src:.cpp=.o)

OPENCV = `pkg-config opencv --cflags --libs`
LIBS = $(OPENCV)

myprog: $(obj)
	$(CC) $(CFLAGS) $(LIBS) -I. -o $@ $^ $(LIBS) 

.PHONY: clean
clean:
	rm -f $(obj) myprog
