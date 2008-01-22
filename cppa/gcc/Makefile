CC	= g++
LD	= g++
CFOLDER	= /home/michaudjd/c++/
SRCDIR	= ../src
BOOST	= /home/michaudjd/lib/boost-trunk/
INCLUDE = -I$(SRCDIR)	\
	-I$(BOOST)	\
	-I$(CFOLDER)/loglite/
SRC	= $(SRCDIR)/database.cpp	\
	$(SRCDIR)/main.cpp
OBJ	= database.o main.o
OUTPUT	= cppa
LIBDIR	= $(BOOST)/stage/lib
LIB	= -lboost_filesystem-gcc40-mt-d		\
	-lboost_wave-gcc40-mt-d			\
	-lboost_thread-gcc40-mt-d		\
	-lboost_program_options-gcc40-mt-d	\
	-lboost_regex-gcc40-mt-d		\
	-lboost_date_time-gcc40-mt-d \
	-lboost_system-gcc40-mt-d \
	-pthread \
	-ldl


CFLAGS	= -ggdb3 $(INCLUDE) -D_DEBUG
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
