DEVICE ?= es-clone-sim

INCLUDES := -I devices/$(DEVICE) -I .

LIBS := -lSDL

OBJS := main/main.o main/event_loop.o \
	fonts/fonts.o fonts/font_8x8.o fonts/font_mini_4x6.o \
	term/term.o

CPPFLAGS := $(INCLUDES)

calMaxima: $(OBJS)
	$(CXX) $(OBJS) $(LIBS) -o $@

%.d: %.cpp
	$(CXX) -MM $(CPPFLAGS) $< > $@.$$$$; \
	sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.$$$$ > $@; \
	rm -f $@.$$$$

%.d: %.c
	$(CC) -MM $(CPPFLAGS) $< > $@.$$$$; \
	sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.$$$$ > $@; \
	rm -f $@.$$$$

include $(OBJS:.o=.d)
