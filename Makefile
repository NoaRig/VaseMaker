CC		= g++
C		= cpp
H             = h
CFLAGS        = -Wall -g
LDFLAGS       = -lm

CFLAGS		= -g
LFLAGS		= -g

ifeq ("$(shell uname)", "Darwin")
  LDFLAGS     = -framework Foundation -framework GLUT -framework OpenGL -lOpenImageIO -lm
else
  ifeq ("$(shell uname)", "Linux")
    LDFLAGS   = -L /usr/lib64/ -lglut -lGL -lGLU -lOpenImageIO -lm
  endif
endif

all: clean cg04.o vase

vase: cg04.o
		${CC} ${CFLAGS} -o vase cg04.o ${LDFLAGS}

clean:
	rm -rf *.o
	rm -rf vase
