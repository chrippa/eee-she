CFLAGS += -g
PREFIX = /usr

SRC = eee-she.c
OBJ = ${SRC:.c=.o}

all: eee-she

.c.o:
	@echo CC $<
	@${CC} -c ${CFLAGS} $<

eee-she: ${OBJ}
	@echo CC -o $@
	@${CC} -o $@ ${OBJ} ${LDFLAGS}

clean:
	@echo CLEAN ${OBJ} eee-she
	@$(RM) ${OBJ} eee-she

install: all
	@echo INSTALL ${DESTDIR}${PREFIX}/bin/eee-she
	@install -D eee-she ${DESTDIR}${PREFIX}/bin/eee-she
