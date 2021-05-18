#pragma once

#include <stdio.h>
#include <string.h>

#include <gio/gio.h>
#include <glib.h>

extern GDBusConnection *dbus_connection;
extern GDBusNodeInfo *introspection;
extern GMainLoop *main_loop;

extern char *output_format;

#ifdef DEBUG
#define print(...) fprintf(stderr, __VA_ARGS__);
#else
#define print(...) (void)(__VA_ARGS__);
#endif

#define INTROSPECTION_XML "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"\
    "<node name=\"/org/freedesktop/Notifications\">\n"\
    "   <interface name=\"org.freedesktop.Notifications\">\n"\
    "       <method name=\"Notify\">\n"\
    "            <arg direction=\"in\"  type=\"s\"     name=\"app_name\"/>\n"\
    "            <arg direction=\"in\"  type=\"u\""\
    " name=\"replaces_id\"/>\n"\
    "            <arg direction=\"in\"  type=\"s\"     name=\"app_icon\"/>\n"\
    "            <arg direction=\"in\"  type=\"s\"     name=\"summary\"/>\n"\
    "            <arg direction=\"in\"  type=\"s\"     name=\"body\"/>\n"\
    "            <arg direction=\"in\"  type=\"as\"    name=\"actions\"/>\n"\
    "            <arg direction=\"in\"  type=\"a{sv}\" name=\"hints\"/>\n"\
    "            <arg direction=\"in\"  type=\"i\""\
    " name=\"expire_timeout\"/>\n"\
    "            <arg direction=\"out\" type=\"u\""\
    " name=\"id\"/>\n"\
    "       </method>\n"\
    "        <method name=\"GetServerInformation\">\n"\
    "            <arg direction=\"out\" type=\"s\" name=\"name\"/>\n"\
    "            <arg direction=\"out\" type=\"s\" name=\"vendor\"/>\n"\
    "            <arg direction=\"out\" type=\"s\" name=\"version\"/>\n"\
    "            <arg direction=\"out\" type=\"s\" name=\"spec_version\"/>\n"\
    "        </method>\n"\
    "   </interface>\n"\
    "</node>"

#define DEFAULT_OUTPUT_FORMAT "#source\n#body"

gboolean stop_main_loop(gpointer);
