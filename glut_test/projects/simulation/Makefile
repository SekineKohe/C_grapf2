CC      = g++
CFLAGS  = -Wall -g -c
INCLUDE = -I../../include/mingw
LDFLAGS = -L../../lib/mingw/x64
LIBS    = -lfreeglut -lopengl32
SOURCES = WU_ell2.cpp
OBJS    = WU_ell2.o
PROGRAM = a

all: $(PROGRAM)

.cpp.o:
			$(CC) $(CFLAGS) $(INCLUDE) -c $< -o $@

$(PROGRAM):	$(OBJS)
			$(CC) $(OBJS) $(LDFLAGS) $(LIBS) -o $(PROGRAM)

