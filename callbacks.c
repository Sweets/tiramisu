#include "tiramisu.h"
#include "callbacks.h"

void method_handler(GDBusConnection *connection, const gchar *sender,
    const gchar *object, const gchar *interface, const gchar *method,
    GVariant *parameters, GDBusMethodInvocation *invocation,
    gpointer user_data) {

    GVariant *return_value = NULL;

    if (!strcmp(method, "Notify")) {
        // ?
    }

    if (!strcmp(method, "GetServerInformation")) {
        // ?
        return_value = g_variant_new("(ssss)",
            "tiramisu", "Sweets", "1.0", "1.2");
            goto flush;
    }

    goto unhandled;

flush:
    g_dbus_method_invocation_return_value(invocation, return_value);
    g_dbus_connection_flush(connection, NULL, NULL, NULL);
    return;

unhandled:
    print("Unhandled: %s %s\n", method, sender);

}

void bus_acquired(GDBusConnection *connection, const gchar *name,
    gpointer user_data) {
    print("%s\n", "Bus has been acquired.");

    guint registered_object;
    registered_object = g_dbus_connection_register_object(connection,
        "/org/freedesktop/Notifications",
        introspection->interfaces[0],
        &(const GDBusInterfaceVTable){ method_handler },
        NULL,
        NULL,
        NULL);

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
