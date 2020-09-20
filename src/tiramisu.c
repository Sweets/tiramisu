#include <stdio.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>

#include <gio/gio.h>
#include <glib.h>
#include <glib-unix.h>

#include "tiramisu.h"
#include "output.h"

GDBusConnection *dbus_connection = NULL;
GDBusNodeInfo *introspection = NULL;
GMainLoop *main_loop = NULL;

unsigned int notification_id = 0;
char print_json = 0;
char *delimiter = "\n";

gboolean stop_main_loop(gpointer user_data) {
    g_main_loop_quit(main_loop);

    return G_SOURCE_CONTINUE;
}

int main(int argc, char **argv) {
    /* Parse arguments */

    char argument;
    while ((argument = getopt(argc, argv, "hjd:"))) {
        switch (argument) {
            case 'd':
                delimiter = optarg;
                break;
            case 'h':
                printf("%s\n",
                    "tiramisu -[h|d|j]\n"
                    "-h\tHelp dialog\n"
                    "-d\tDelimeter for default output style.\n"
                    "-j\tUse JSON output style\n");
                return EXIT_SUCCESS;
                break;
            case 'j':
                print_json = 1;
                break;
            default:
                break;
        }
    }

    guint owned_name;

    /* Connect to DBUS */

    introspection = g_dbus_node_info_new_for_xml(INTROSPECTION_XML, NULL);
    owned_name = g_bus_own_name(G_BUS_TYPE_SESSION,
        "org.freedesktop.Notifications",
        G_BUS_NAME_OWNER_FLAGS_NONE,
        (GBusAcquiredCallback)bus_acquired,
        (GBusNameAcquiredCallback)name_acquired,
        (GBusNameLostCallback)name_lost,
        NULL, /* user_data */
        NULL); /* user_data_free_func */

    /* Setup and start the loop */

    main_loop = g_main_loop_new(NULL, FALSE);

    guint signal_term = g_unix_signal_add(SIGTERM, stop_main_loop, NULL);
    guint signal_int = g_unix_signal_add(SIGINT, stop_main_loop, NULL);

    g_main_loop_run(main_loop);
    g_clear_pointer(&main_loop, g_main_loop_unref);

    g_source_remove(signal_term);
    g_source_remove(signal_int);

    g_clear_pointer(&introspection, g_dbus_node_info_unref);
    g_bus_unown_name(owned_name);

    return EXIT_SUCCESS;
}

void bus_acquired(GDBusConnection *connection, const gchar *name,
    gpointer user_data) {
    print("%s\n", "Bus has been acquired.");

    guint registered_object = g_dbus_connection_register_object(connection,
        "/org/freedesktop/Notifications", introspection->interfaces[0],
        &(const GDBusInterfaceVTable){ method_handler }, NULL, NULL, NULL);

    if (!registered_object) {
        print("%s\n", "Unable to register.");
        stop_main_loop(NULL);
    }
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
        printf("%s; %s\n",
            "Unable to connect to acquire org.freedesktop.Notifications",
            "could not connect to dbus.");
        stop_main_loop(NULL);
    }
    else
        print("%s\n", "Successfully acquired org.freedesktop.Notifications");
}

void method_handler(GDBusConnection *connection, const gchar *sender,
    const gchar *object, const gchar *interface, const gchar *label,
    GVariant *parameters, GDBusMethodInvocation *invocation,
    gpointer user_data) {

    GVariant *return_value = NULL;

    if (!strcmp(label, "GetServerInformation"))
        return_value = g_variant_new("(ssss)",
            "tiramisu", "Sweets", "1.0", "1.2");
    else if (!strcmp(label, "Notify")) {
        output_notification(parameters);
        return_value = g_variant_new("(u)", ++notification_id);
    } else
        print("Unhandled: %s %s\n", label, sender);

    g_dbus_method_invocation_return_value(invocation, return_value);
    g_dbus_connection_flush(connection, NULL, NULL, NULL);

}
