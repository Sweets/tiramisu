#pragma once

#include <stdio.h>
#include <string.h>

#include <gio/gio.h>
#include <glib.h>

#include "config.h"

extern GDBusConnection *dbus_connection;
extern GDBusNodeInfo *introspection;

#define print(...) fprintf(stderr, __VA_ARGS__);

void output(gchar*, guint32, gchar*, gchar*, gchar*, GVariant*, GVariant*,
    gint32);
