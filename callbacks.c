#include <stdio.h>

#include "tiramisu.h"
#include "callbacks.h"

unsigned int notification_id = 0;

char *sanitize(char *string, char *out) {
    memset(out, 0, strlen(out));

    while (*string) {
        if (*string == '"')
            strcat(out, "\\\"");
        else if (*string == '\n')
            strcat(out, "\\n");
        else
            out[strlen(out)] = *string;
        string++;
    }

    return out;
}

void method_handler(GDBusConnection *connection, const gchar *sender,
    const gchar *object, const gchar *interface, const gchar *method,
    GVariant *parameters, GDBusMethodInvocation *invocation,
    gpointer user_data) {

    GVariantIter iterator;
    gchar *app_name;
    guint32 replaces_id;
    gchar *app_icon;
    gchar *summary;
    gchar *body;
    gchar **actions;
    GVariant *hints;
    gint32 timeout;
    GVariant *return_value = NULL;

    if (!strcmp(method, "Notify"))
        goto output;

    if (!strcmp(method, "GetServerInformation")) {
        return_value = g_variant_new("(ssss)",
            "tiramisu", "Sweets", "1.0", "1.2");
            goto flush;
    }

    print("Unhandled: %s %s\n", method, sender);

output:
    notification_id++;

    g_variant_iter_init(&iterator, parameters);
    g_variant_iter_next(&iterator, "s", &app_name);
    g_variant_iter_next(&iterator, "u", &replaces_id);
    g_variant_iter_next(&iterator, "s", &app_icon);
    g_variant_iter_next(&iterator, "s", &summary);
    g_variant_iter_next(&iterator, "s", &body);
    g_variant_iter_next(&iterator, "^a&s", &actions);
    g_variant_iter_next(&iterator, "@a{sv}", &hints);
    g_variant_iter_next(&iterator, "i", &timeout);

    char *sanitized = (char *)calloc(512, sizeof(char));

    printf("%s: %s\n%s: %s",
        "app_name", sanitize(app_name, sanitized),
        "app_icon", sanitize(app_icon, sanitized));
    printf("%s: %u\n%s: %d\n",
        "replaces_id", replaces_id,
        "timeout", timeout);
    printf("%s\n", "hints");

    gchar *key;
    GVariant *value;

    const char *int_format = "\t%s: %d\n";
    const char *uint_format = "\t%s: %u\n";

    unsigned int index = 0;

    g_variant_iter_init(&iterator, hints);
    while (g_variant_iter_loop(&iterator, "{sv}", &key, NULL)) {

        /* There has to be a better way. glib, why? */

        if ((value = g_variant_lookup_value(hints, key, GT_STRING)))
            printf("\t%s: %s\n", key,
                sanitize(g_variant_dup_string(value, NULL), sanitized));
        else if ((value = g_variant_lookup_value(hints, key, GT_INT16)))
            printf(int_format, key, g_variant_get_int16(value));
        else if ((value = g_variant_lookup_value(hints, key, GT_INT32)))
            printf(int_format, key, g_variant_get_int32(value));
        else if ((value = g_variant_lookup_value(hints, key, GT_INT64)))
            printf(int_format, key, g_variant_get_int64(value));
        else if ((value = g_variant_lookup_value(hints, key, GT_UINT16)))
            printf(uint_format, key, g_variant_get_uint16(value));
        else if ((value = g_variant_lookup_value(hints, key, GT_UINT32)))
            printf(uint_format, key, g_variant_get_uint32(value));
        else if ((value = g_variant_lookup_value(hints, key, GT_UINT64)))
            printf(uint_format, key, g_variant_get_uint64(value));
        else if ((value = g_variant_lookup_value(hints, key, GT_DOUBLE)))
            printf("\t%s: %f\n", key, g_variant_get_double(value));
        else if ((value = g_variant_lookup_value(hints, key, GT_BYTE)))
            printf("\t%s: %x\n", key, g_variant_get_byte(value));
        else if ((value = g_variant_lookup_value(hints, key, GT_BOOL))) {
            if (g_variant_get_boolean(value))
                printf("\t%s: 1\n", key);
            else
                printf("\t%s: 0\n", key);
        }

    }

    printf("%s\n", "actions");

    while (actions[index + 1]) {
        printf("\t%s = '%s'\n", actions[index], actions[++index]);
        index++;
    }

    printf("%s: %s\n%s: %s",
            "summary", sanitize(summary, sanitized),
            "body", sanitize(body, sanitized));

    return_value = g_variant_new("(u)", notification_id);

    fflush(stdout);
    free(sanitized);

    goto flush;

flush:
    g_dbus_method_invocation_return_value(invocation, return_value);
    g_dbus_connection_flush(connection, NULL, NULL, NULL);
    return;

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
