#pragma once

#include <gio/gio.h>
#include <glib.h>

void bus_acquired(GDBusConnection*, const gchar *, gpointer);
void name_acquired(GDBusConnection*, const gchar *, gpointer);
void name_lost(GDBusConnection*, const gchar *, gpointer);
