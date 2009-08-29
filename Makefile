CFLAGS += -g
PREFIX = /usr

all: eee-she
eee-she: eee-she.o
clean:
	$(RM) *.o eee-she

install:
	@echo installing executable file to ${DESTDIR}${PREFIX}/bin
	@mkdir -p ${DESTDIR}${PREFIX}/bin
	@cp -f eee-she ${DESTDIR}${PREFIX}/bin
	@chmod 755 ${DESTDIR}${PREFIX}/bin/eee-she
