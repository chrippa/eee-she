CFLAGS += -g

all: eee-she
eee-she: eee-she.o
clean:
	$(RM) *.o eee-she
