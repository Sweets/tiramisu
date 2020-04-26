#pragma once

#include <stdio.h>
#include <string.h>

#include <gio/gio.h>
#include <glib.h>

#include "config.h"

extern GDBusConnection *dbus_connection;
extern GDBusNodeInfo *introspection;
extern GMainLoop *main_loop;

#define print(...) fprintf(stderr, __VA_ARGS__);

gboolean stop_main_loop(gpointer);
