#pragma once

#include <gio/gio.h>
#include <glib.h>

void output_notification(gchar*, guint32, gchar*, gchar*, gchar*, GVariant**,
    GVariant*, gint32);
