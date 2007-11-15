CC	= gcc
CFOLDER	= u:/c++/
SRCDIR	= ../src
BOOST	= c:/boost/boost_1_34_1
INCLUDE = -I$(SRCDIR)	\
	-I$(BOOST)	\
	-I$(CFOLDER)/logging/
SRC	= $(SRCDIR)/database.cpp	\
	$(SRCDIR)/sqlite3.c	\
	$(SRCDIR)/main.cpp
OBJ	= $(SRC:.cpp,.c=.o)
OUTPUT	= cppa

CFLAGS	= -ggdb3 $(INCLUDE)
LDFLAGS	= 

all: $(OUTPUT)

$(OUTPUT): $(OBJ)
	$(CC) $(LDFLAGS) $(OBJ) -o $(OUTPUT)

.cpp.o:
	$(CC) $(CFLAGS) -c $<

sqlite3.o:
	$(CC) $(CFLAGS) -c $(SRCDIR)/sqlite3.c -o sqlite3.o
