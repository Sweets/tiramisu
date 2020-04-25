
all:
	$(CC) `pkg-config --libs --cflags glib-2.0 gio-2.0` note.c
