#include "note.h"
#include "callbacks.h"

void notification_received(GDBusConnection *connection, const gchar *sender,
    GVariant *parameters, GDBusMethodInvocation *invocation) {
    //
/*    GVariantIter iterator;
    GVariant *value;
    gchar *key;
    GVariant *dictionary;

    g_variant_iter_init(&iterator, dictionary);
    while (g_variant_iter_loop(&iterator, "s", &key, &value)) {
        g_print("Item '%s' has type '%s'\n", key,
            g_variant_get_type_string(value));
    }*/
}

void bus_acquired(GDBusConnection *connection, const gchar *name,
    gpointer user_data) {
    printf("%s\n", "Bus has been acquired.");
}

void name_acquired(GDBusConnection *connection, const gchar *name,
    gpointer user_data) {
    dbus_connection = connection;
    printf("%s\n", "Name has been acquired.");
}

void name_lost(GDBusConnection *connection, const gchar *name,
    gpointer user_data) {
    // we lost the Notifications daemon name or couldn't acquire it, shutdown

    if (!connection) {
        printf("%s; %s\n",
            "Unable to connect to acquire org.freedesktop.Notifications",
            "could not connect to dbus.");
    } else
        printf("%s", "Successfully acquired org.freedesktop.Notifications");

    exit(1);
}
