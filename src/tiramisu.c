#include <stdio.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>

#include <gio/gio.h>
#include <glib.h>
#include <glib-unix.h>

#include "tiramisu.h"
#include "notification.h"
#include "events.h"

GDBusConnection *dbus_connection = NULL;
GDBusNodeInfo *introspection = NULL;
GMainLoop *main_loop = NULL;

char *output_format;
int sanitize_output = 0;

gboolean stop_main_loop(gpointer user_data) {
    g_main_loop_quit(main_loop);

    return G_SOURCE_CONTINUE;
}

int main(int argc, char **argv) {
    /* Parse arguments */

    char argument;
    while ((argument = getopt(argc, argv, "hsjo:")) >= 0) {
        switch (argument) {
            case 'h':
                printf("%s\n",
                    "tiramisu -[h|o]\n"
                    "-h\tHelp dialog\n"
                    "-s\tSanitize output (for outputs that use quotes, e.g. JSON)\n"
                    "-j\tOutput to JSON (implies `-s`)."
                    "-o\tOutput formatting\n");
                return EXIT_SUCCESS;
                break;
            case 's':
                sanitize_output = 1;
                break;
            case 'j':
                sanitize_output = 1;
                output_format = JSON_OUTPUT_FORMAT;
                break;
            case 'o':
                output_format = strdup(optarg);
                break;
            default:
                break;
        }
    }

    if (!output_format)
        output_format = DEFAULT_OUTPUT_FORMAT;

    guint owner_id;
    gchar *bus_name = "org.freedesktop.Notifications";
    introspection = g_dbus_node_info_new_for_xml(INTROSPECTION_XML, NULL);

    owner_id = g_bus_own_name(G_BUS_TYPE_SESSION, bus_name, G_BUS_NAME_OWNER_FLAGS_NONE,
        bus_acquired, name_acquired, name_lost, NULL, NULL);
    main_loop = g_main_loop_new(NULL, FALSE);

    guint signal_term = g_unix_signal_add(SIGTERM, stop_main_loop, NULL);
    guint signal_int = g_unix_signal_add(SIGINT, stop_main_loop, NULL);

    g_main_loop_run(main_loop);

    g_source_remove(signal_term);
    g_source_remove(signal_int);

    g_clear_pointer(&main_loop, g_main_loop_unref);
    g_clear_pointer(&introspection, g_dbus_node_info_unref);

    g_bus_unown_name(owner_id);
    return EXIT_SUCCESS;
}

