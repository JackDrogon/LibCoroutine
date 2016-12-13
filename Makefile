# TODO: Add depency
# TODO: Add platform detection
# TODO: Add format
# TODO: Delivery params to run
# TODO: Add auto generate Project Makefile with TARGET from file name
# TODO: Use wildchard && vpath to generate OBJS
# TODO: Refer to erlang.mk && kbuild

CC       := g
CFLAGS   := -Weverything
CXX      := g++
CXXFLAGS := -Weverything -std=c++1y
LD       := g++
LDFLAGS  :=

ifeq ($(DEBUG),yes)
	CFLAGS   += -g
	CXXFLAGS += -g
else
	CFLAGS   += -DNDEBUG
	CXXFLAGS += -DNDEBUG
endif


TARGET := fiber_test
OBJS   := fiber_test.o fiber.o scheduler.o


all: $(TARGET)
.PHONY: all clean run

$(TARGET): $(OBJS)
	$(LD) $(LDFLAGS) -o $@ $^

# $(OBJS): %.o : %.cc
# 	$(CXX) $(CXXFLAGS) -c -o $@ $<

run: $(TARGET)
	./$(TARGET)

tags:
	ctags -R --c++-kinds=+p+l+x+c+d+e+f+g+m+n+s+t+u+v --fields=+liaS --extra=+q --extra=+f

gtags:
	gtags -c

clean:
	-$(RM) -rf $(OBJS) $(TARGET)
