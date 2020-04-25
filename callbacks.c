#include "tiramisu.h"
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

void method_handler(GDBusConnection *connection, const gchar *sender,
    const gchar *object, const gchar *interface, const gchar *method,
    GVariant *parameters, GDBusMethodInvocation *invocation,
    gpointer user_data) {

    if (!strcmp(method, "Notify")) {
        // ?
    }

    print("%s %s\n", method, sender);

}

void bus_acquired(GDBusConnection *connection, const gchar *name,
    gpointer user_data) {
    print("%s\n", "Bus has been acquired.");

    guint registered_object;
    registered_object = g_dbus_connection_register_object(connection,
        "/org/freedesktop/Notifications", introspection->interfaces[0],
        &(const GDBusInterfaceVTable){ method_handler }, NULL, NULL, NULL);

    if (!registered_object)
        print("%s\n", "Unable to register.");
}

void name_acquired(GDBusConnection *connection, const gchar *name,
    gpointer user_data) {
    dbus_connection = connection;
    print("%s\n", "Name has been acquired.");
}

void name_lost(GDBusConnection *connection, const gchar *name,
    gpointer user_data) {
    // we lost the Notifications daemon name or couldn't acquire it, shutdown

    if (!connection) {
        print("%s; %s\n",
            "Unable to connect to acquire org.freedesktop.Notifications",
            "could not connect to dbus.");
        exit(1);
    } else
        print("%s", "Successfully acquired org.freedesktop.Notifications");

}
