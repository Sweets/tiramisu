TARGET      = tiramisu
SRC        := src/notification.vala src/dbus.vala src/tiramisu.vala

PREFIX     ?= /usr/local
INSTALL     = install -Dm755
RM         ?= rm -f
PKG_CONFIG ?= pkg-config

VALAC      ?= valac
CFLAGS     += -Wall -Wno-unused-value
IFLAGS      = --pkg gio-2.0
LFLAGS      = `$(PKG_CONFIG) --libs glib-2.0 gio-2.0`

all: $(TARGET)

$(TARGET): $(SRC)
	$(VALAC) $(IFLAGS) $(SRC) -o $(TARGET)
#	$(CC) $(CFLAGS) $(IFLAGS) $(SRC) $(LFLAGS) $(LDFLAGS) -o $(TARGET)

install: $(TARGET)
	mkdir -p $(DESTDIR)$(PREFIX)/bin
	$(INSTALL) $(TARGET) $(DESTDIR)$(PREFIX)/bin/$(TARGET)

clean:
	$(RM) ./tiramisu
