#pragma once

#include <stdio.h>
#include <string.h>

#include <gio/gio.h>
#include <glib.h>

#include "config.h"

extern GDBusConnection *dbus_connection;
extern GDBusNodeInfo *introspection;
extern GMainLoop *main_loop;

#ifdef DEBUG
#define print(...) fprintf(stderr, __VA_ARGS__);
#else
#define print(...) (void)(__VA_ARGS__);
#endif

gboolean stop_main_loop(gpointer);
