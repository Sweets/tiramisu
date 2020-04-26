#include <stdio.h>
#include <string.h>

#include <gio/gio.h>
#include <glib.h>

#include "tiramisu.h"
#include "callbacks.h"
#include "config.h"

GDBusConnection *dbus_connection = NULL;
GDBusNodeInfo *introspection = NULL;

/* Build introspection XML based on configuration */

const char *xml = "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
    "<node name=\"/org/freedesktop/Notifications\">\n"
    "   <interface name=\"org.freedesktop.Notifications\">\n"

    "       <method name=\"Notify\">\n"
    "            <arg direction=\"in\"  type=\"s\"     name=\"app_name\"/>\n"
    "            <arg direction=\"in\"  type=\"u\"     name=\"replaces_id\"/>\n"
    "            <arg direction=\"in\"  type=\"s\"     name=\"app_icon\"/>\n"
    "            <arg direction=\"in\"  type=\"s\"     name=\"summary\"/>\n"
    "            <arg direction=\"in\"  type=\"s\"     name=\"body\"/>\n"
    "            <arg direction=\"in\"  type=\"as\"    name=\"actions\"/>\n"
    "            <arg direction=\"in\"  type=\"a{sv}\" name=\"hints\"/>\n"
    "            <arg direction=\"in\"  type=\"i\""
    " name=\"expire_timeout\"/>\n"
    "            <arg direction=\"out\" type=\"u\""
    " name=\"id\"/>\n"
    "       </method>\n"


    "        <method name=\"GetServerInformation\">\n"
    "            <arg direction=\"out\" type=\"s\" name=\"name\"/>\n"
    "            <arg direction=\"out\" type=\"s\" name=\"vendor\"/>\n"
    "            <arg direction=\"out\" type=\"s\" name=\"version\"/>\n"
    "            <arg direction=\"out\" type=\"s\" name=\"spec_version\"/>\n"
    "        </method>\n"

    "   </interface>\n"
    "</node>";

void output(gchar *app_name, guint32 replaces_id, gchar *app_icon,
    gchar *summary, gchar *body, GVariant *actions, GVariant *hints,
    gint32 timeout) {

    /* Build output and fucking send it */

    char *out;

    char *format = "%s"
#ifdef RECEIVE_APP_ICON
        "\n%s"
#endif

    /* TODO: actions */
    /* TODO: hints */

#ifdef RECEIVE_REPLACES_ID
        "\n%lu"
#endif
        "\n%d\n%s\n%s";

    asprintf(&out, format, app_name,
#ifdef RECEIVE_APP_ICON
        app_icon,
#endif
/* TODO: actions */
/* TODO: hints */
#ifdef RECEIVE_REPLACES_ID
        replaces_id,
#endif
        timeout, summary, body);

    printf("%s\n", out);

}

int main(int argc, char **argv) {
    GMainLoop *main_loop;

    guint owned_name;

    /* Connect to DBUS */

    introspection = g_dbus_node_info_new_for_xml(xml, NULL);
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

    g_clear_pointer(&introspection, g_dbus_node_info_unref);
    g_bus_unown_name(owned_name);

}
