#include <stdio.h>
#include <glib.h>

#include "events.h"
#include "tiramisu.h"
#include "notification.h"

unsigned int id = 0;

void event_handler(GDBusConnection *connection, const gchar *sender,
    const gchar *object, const gchar *interface, const gchar*label,
    GVariant *parameters, GDBusMethodInvocation *invocation,
    gpointer data) {

    GVariant *return_value = NULL;

    if (!strcmp(label, "GetServerInformation"))
        return_value = g_variant_new("(ssss)", "tiramisu", "Sweets", "1.1", "1.2");
    else if (!strcmp(label, "GetCapabilities")) {

        GVariantBuilder *builder;
        builder = g_variant_builder_new(G_VARIANT_TYPE("as"));

        g_variant_builder_add(builder, "s", "body");
        g_variant_builder_add(builder, "s", "actions");

        return_value = g_variant_new("(as)", builder);

    } else if (!strcmp(label, "Notify")) {

        notification_t notification;
        hydrate_notification(&notification, parameters);

        char *output = strdup(output_format);
        interpolate_output_string(&output, notification);
        printf("%s\n", output);
        fflush(stdout);

        return_value = g_variant_new("(u)", ++id);
    } else
        printf("Unhandled(%s, %s)", sender, label);

    g_dbus_method_invocation_return_value(invocation, return_value);
    g_dbus_connection_flush(connection, NULL, NULL, NULL);
}

void bus_acquired(GDBusConnection *connection, const gchar *name, gpointer data) {
    const gchar *object_path = "/org/freedesktop/Notifications";
    GDBusInterfaceInfo *interface_info = introspection->interfaces[0];
    const GDBusInterfaceVTable *interface_vtable = &(const GDBusInterfaceVTable){
        event_handler
    };

    guint registered_object;
    registered_object = g_dbus_connection_register_object(connection,
        object_path, interface_info, interface_vtable, NULL, NULL, NULL);

    if (registered_object)
        return;

    printf("%s\n", "bus not acquired; unable to register object path");
    stop_main_loop(NULL);
}

void name_acquired(GDBusConnection *connection, const gchar *name, gpointer data) {
    dbus_connection = connection;
}

void name_lost(GDBusConnection *connection, const gchar *name, gpointer data) {
    if (connection)
        return;

    printf("%s\n", "bus not acquired; could not connect to dbus");
    stop_main_loop(NULL);
}
