SRC_PATH = .
SRCS     = $(wildcard $(SRC_PATH)/*.c)
OBJS     = $(SRCS:.c=.o)
HEADERS  = $(wildcard $(SRC_PATH)/*.h)
TARGET   = server

CC      = gcc
LIBS    =
LDFLAGS = -lpthread
DEFINES = -DDEBUG
INCLUDE = -I.
CFLAGS  = -O2 -Werror-implicit-function-declaration $(DEFINES)

all: $(TARGET)

$(TARGET): $(OBJS) $(LIBS)
	$(CC) $(CFLAGS) -o $(TARGET) $^ $(LDFLAGS)

%.o: %.c $(HEADERS)
	$(CC) -c $(CFLAGS) $(INCLUDE) $<

clean:
	rm -f *.o

run:
	sudo ./$(TARGET)

.PHONY: all clean run
