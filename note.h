#pragma once

#include <gio/gio.h>
#include <glib.h>

extern GDBusConnection *dbus_connection;

char *build_introspection_xml(void);
