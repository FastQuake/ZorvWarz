SRCS = $(wildcard *.cpp)
OBJS = $(patsubst %.cpp,%.o,$(SRCS))
LD_FLAGS = -lsfml-system -lsfml-window -lsfml-graphics 
CC_FLAGS = -g 
TITLE = zorv

$(TITLE): $(OBJS)
	g++ -o $@ $^ $(LD_FLAGS)
%.o: %.cpp
	g++ $(CC_FLAGS) -c $<

clean:
	rm *.o
	rm $(TITLE)

run:
	./$(TITLE)
