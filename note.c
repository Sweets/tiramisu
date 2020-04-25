#include <stdio.h>
#include <string.h>

#include <gio/gio.h>
#include <glib.h>

#include "note.h"
#include "config.h"

GDBusConnection *dbus_connection = NULL;
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
    /* Build introspection */
    guint owned_name;
    GDBusNodeInfo *introspection_data;

    introspection_data = g_dbus_node_info_new_for_xml(introspection, NULL);
    owned_name = g_bus_own_name(G_BUS_TYPE_SESSION,
        "org.freedesktop.Notifications",
        G_BUS_NAME_OWNER_FLAGS_NONE,
        NULL, NULL, NULL, NULL, NULL);

    printf("%s\n", introspection);
    sleep(10);

    g_clear_pointer(&introspection_data, g_dbus_node_info_unref);
    g_bus_unown_name(owned_name);

    /* What now? */
}
