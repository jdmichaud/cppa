CC	= g++
LD	= g++
CFOLDER	= u:/c++/
SRCDIR	= ../src
BOOST	= c:/boost/boost_1_34_1
INCLUDE = -I$(SRCDIR)	\
	-I$(BOOST)	\
	-I$(CFOLDER)/logging/
SRC	= $(SRCDIR)/database.cpp	\
	$(SRCDIR)/main.cpp
OBJ	= database.o main.o
OUTPUT	= cppa
LIBDIR	= $(BOOST)/stage/lib
LIB	= -lboost_filesystem-gcc42-mt-d		\
	-lboost_wave-gcc42-mt-d			\
	-lboost_thread-gcc42-mt-d		\
	-lboost_program_options-gcc42-mt-d	\
	-lboost_regex-gcc42-mt-d		\
	-lboost_date_time-gcc42-mt-d


CFLAGS	= -ggdb3 $(INCLUDE)
LDFLAGS	= -L$(LIBDIR) -static

all: $(OUTPUT)

$(OUTPUT): $(OBJ) sqlite3.o
	$(LD) $(LDFLAGS) $(OBJ) sqlite3.o $(LIB) -o $(OUTPUT)

$(OBJ): $(SRC)
	$(CC) $(CFLAGS) -c $?

sqlite3.o: ../src/sqlite3.c
	gcc  $(CFLAGS) -c $?

clean:
	rm -fr *.o
	rm -fr *~
	rm -fr *#

re: clean all
