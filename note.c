#include <stdio.h>
#include <string.h>

#include <gio/gio.h>
#include <glib.h>

#include "note.h"
#include "callbacks.h"
#include "config.h"

GDBusConnection *dbus_connection = NULL;

/* Build introspection XML based on configuration */

const char *introspection = "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
    "<node name=\"/org/freedesktop/Notifications\">\n"
    "   <interface name=\"org.freedesktop.Notifications\">\n"
    "       <method name=\"Notify\">\n"
#ifdef RECEIVE_ACTIONS
    "           <arg direction=\"in\" name=\"actions\" type=\"as\"/>\n"
#endif
#ifdef RECEIVE_APP_ICON
    "           <arg direction=\"in\" name=\"app_icon\" type=\"s\"/>\n"
#endif
    "           <arg direction=\"in\" name=\"app_name\" type=\"s\"/>\n"
    "           <arg direction=\"in\" name=\"body\" type=\"s\"/>\n"
#ifdef RECEIVE_EXPIRE_TIMEOUT
    "           <arg direction=\"in\" name=\"expire_timeout\" type=\"i\"/>\n"
#endif
    "           <arg direction=\"in\" name=\"hint\" type=\"a{sv}\"/>\n"
    "           <arg direction=\"in\" name=\"id\" type=\"u\"/>\n"
#ifdef RECEIVE_REPLACES_ID
    "           <arg direction=\"in\" name=\"replaces_id\" type=\"u\"/>\n"
#endif
    "           <arg direction=\"in\" name=\"summary\" type=\"s\"/>\n"
    "       </method>\n"
    "   </interface>\n"
    "</node>";

int main(int argc, char **argv) {
    GMainLoop *main_loop;

    guint owned_name;
    GDBusNodeInfo *introspection_data;

    /* Connect to DBUS */

    introspection_data = g_dbus_node_info_new_for_xml(introspection, NULL);
    owned_name = g_bus_own_name(G_BUS_TYPE_SESSION,
        "org.freedesktop.Notifications",
        G_BUS_NAME_OWNER_FLAGS_NONE,
        (GBusAcquiredCallback)bus_acquired, /* bus_acquired_handler */
        (GBusNameAcquiredCallback)name_acquired, /* name_acquired_handler */
        (GBusNameLostCallback)name_lost, /* name_lost_handler */
        NULL, /* user_data */
        NULL); /* user_data_free_func */

    /* Setup and start the loop */

    main_loop = g_main_loop_new(NULL, FALSE);

    g_main_loop_run(main_loop);
    g_clear_pointer(&main_loop, g_main_loop_unref);

    g_clear_pointer(&introspection_data, g_dbus_node_info_unref);
    g_bus_unown_name(owned_name);

}
