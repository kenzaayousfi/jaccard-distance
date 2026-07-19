jaccard_dir = .

CC = gcc
RM = rm -f

CFLAGS = -std=c2x \
  -Wall -Wconversion -Werror -Wextra -Wpedantic -Wwrite-strings \
  -O2 -g \
  -I$(jaccard_dir)

vpath %.c $(jaccard_dir)
vpath %.h $(jaccard_dir)

objects = main.o lecture.o distance.o graph.o hashtable.o holdall.o

executable = jdis

.PHONY: all clean

all: $(executable)

clean:
	$(RM) $(objects) $(executable)

$(executable): $(objects)
	$(CC) $(objects) -o $(executable)

main.o: main.c lecture.h distance.h graph.h hashtable.h

lecture.o: lecture.c lecture.h hashtable.h holdall.h

distance.o: distance.c distance.h lecture.h hashtable.h holdall.h

graph.o: graph.c graph.h lecture.h hashtable.h holdall.h

hashtable.o: hashtable.c hashtable.h hashtable_ip.h

holdall.o: holdall.c holdall.h holdall_ip.h

$(objects): makefile
