#pragma once

#include <glib.h>

typedef struct {
    gchar *source;
    gchar *summary;
    gchar *body;
    gchar *icon;

    guint32 id;
    gint32 timeout;

    gchar **actions;
    GVariant *hints;
} notification_t;

void hydrate_notification(notification_t*, GVariant*);
void interpolate_output_string(char*, notification_t);
