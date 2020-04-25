#pragma once

#include <gio/gio.h>
#include <glib.h>

void method_handler(GDBusConnection*, const gchar*, const gchar*, const gchar*,
    const gchar*, GVariant*, GDBusMethodInvocation*, gpointer);

void bus_acquired(GDBusConnection*, const gchar*, gpointer);
void name_acquired(GDBusConnection*, const gchar*, gpointer);
void name_lost(GDBusConnection*, const gchar*, gpointer);
