.POSIX:
.SUFFIXES:
.PHONY: \
	all \
	install \
	uninstall \
	clean

all: libsmolui.a

demo: libsmolui.a
	$(CC) $(CFLAGS) -o $@ demo.c \
		-I. -L. \
		-I${SUNDER_HOME}/lib/raylib \
		$$(${SUNDER_HOME}/lib/raylib/raylib-config desktop --cflags) \
		$$(${SUNDER_HOME}/lib/raylib/raylib-config desktop --libs) \
		-lsmolui

libsmolui.a:
	$(CC) $(CFLAGS) -c -I. -I${SUNDER_HOME}/lib/raylib -o smolui.o microui.c
	$(CC) $(CFLAGS) -c -I. -I${SUNDER_HOME}/lib/raylib -o murl.o murl.c
	ar -rcs $@ smolui.o murl.o

install: libsmolui.a
	mkdir -p "$(SUNDER_HOME)/lib/smolui"
	# cp smolui.sunder "$(SUNDER_HOME)/lib/smolui"
	cp libsmolui.a "$(SUNDER_HOME)/lib/smolui"

uninstall:
	rm -rf "$(SUNDER_HOME)/lib/smolui"

clean:
	rm -f \
		demo \
		*.a \
		*.o
