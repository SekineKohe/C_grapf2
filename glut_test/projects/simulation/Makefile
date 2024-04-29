CC      = g++
CFLAGS  = -Wall -g -c
INCLUDE = -I../../include/mingw
LDFLAGS = -L../../lib/mingw/x64
LIBS    = -lfreeglut -lopengl32
SOURCES = waveform.cpp
OBJS    = waveform.o
PROGRAM = b

all: $(PROGRAM)

.cpp.o:
			$(CC) $(CFLAGS) $(INCLUDE) -c $< -o $@

$(PROGRAM):	$(OBJS)
			$(CC) $(OBJS) $(LDFLAGS) $(LIBS) -o $(PROGRAM)

