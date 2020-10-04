#include <stdio.h>
#include <glib.h>

#include "tiramisu.h"
#include "output.h"

char *sanitize(const char *string) {
    /* allocating double the size of the original string should be enough */
    char *out = calloc(strlen(string) * 2 + 1, 1);

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

void output_notification(GVariant *parameters) {
    GVariantIter iterator;
    gchar *app_name;
    guint32 replaces_id;
    gchar *app_icon;
    gchar *summary;
    gchar *body;
    gchar **actions;
    GVariant *hints;
    gint32 timeout;

    g_variant_iter_init(&iterator, parameters);
    g_variant_iter_next(&iterator, "s", &app_name);
    g_variant_iter_next(&iterator, "u", &replaces_id);
    g_variant_iter_next(&iterator, "s", &app_icon);
    g_variant_iter_next(&iterator, "s", &summary);
    g_variant_iter_next(&iterator, "s", &body);
    g_variant_iter_next(&iterator, "^a&s", &actions);
    g_variant_iter_next(&iterator, "@a{sv}", &hints);
    g_variant_iter_next(&iterator, "i", &timeout);

    char *app_name_sanitized = sanitize(app_name);
    char *app_icon_sanitized = sanitize(app_icon);
    char *summary_sanitized = sanitize(summary);
    char *body_sanitized = sanitize(body);

    if (print_json)
        json_output(app_name_sanitized, app_icon_sanitized, replaces_id,
            timeout, hints, actions, summary_sanitized, body_sanitized);
    else
        default_output(app_name_sanitized, app_icon_sanitized, replaces_id,
            timeout, hints, actions, summary_sanitized, body_sanitized);

    free(app_name_sanitized);
    free(app_icon_sanitized);
    free(app_name);
    free(app_icon);

    free(actions);

    free(summary);
    free(body);
    free(summary_sanitized);
    free(body_sanitized);

    fflush(stdout);
}

void hints_output_iterator(GVariant *hints, const char *str_format,
    const char *int_format, const char *uint_format,
    const char *double_format, const char *boolean_format,
    const char *byte_format, const char *err_format,
    const char *element_delimiter) {

    GVariantIter iterator;
    gchar *key;
    GVariant *value;

    unsigned int index = 0;
    char *value_sanitized;

    g_variant_iter_init(&iterator, hints);
    while (g_variant_iter_loop(&iterator, "{sv}", &key, NULL)) {
        if (index)
            printf(element_delimiter);
        /* Strings */
        if ((value = g_variant_lookup_value(hints, key, GT_STRING))) {
            value_sanitized = sanitize(g_variant_get_string(value, NULL));
            printf(str_format, key, value_sanitized);
            free(value_sanitized);
        }
        /* Integers */
        else if ((value = g_variant_lookup_value(hints, key, GT_INT16)))
            printf(int_format, key, g_variant_get_int16(value));
        else if ((value = g_variant_lookup_value(hints, key, GT_INT32)))
            printf(int_format, key, g_variant_get_int32(value));
        else if ((value = g_variant_lookup_value(hints, key, GT_INT64)))
            printf(int_format, key, g_variant_get_int64(value));
        /* Unsigned integers */
        else if ((value = g_variant_lookup_value(hints, key, GT_UINT16)))
            printf(uint_format, key, g_variant_get_uint16(value));
        else if ((value = g_variant_lookup_value(hints, key, GT_UINT32)))
            printf(uint_format, key, g_variant_get_uint32(value));
        else if ((value = g_variant_lookup_value(hints, key, GT_UINT64)))
            printf(uint_format, key, g_variant_get_uint64(value));
        /* Doubles */
        else if ((value = g_variant_lookup_value(hints, key, GT_DOUBLE)))
            printf(double_format, key, g_variant_get_double(value));
        /* Bytes */
        else if ((value = g_variant_lookup_value(hints, key, GT_BYTE)))
            printf(byte_format, key, g_variant_get_byte(value));
        /* Booleans */
        else if ((value = g_variant_lookup_value(hints, key, GT_BOOL)))
            printf(boolean_format, key, g_variant_get_boolean(value));
        else {
            // value is of unknown type
            printf(err_format, key);
            index++;
            continue;
        }

        g_variant_unref(value);
        index++;
    }

    g_variant_unref(hints);
}

void default_output(gchar *app_name, gchar *app_icon, guint32 replaces_id,
    gint32 timeout, GVariant *hints, gchar **actions, gchar *summary,
    gchar *body) {

    printf("app_name: %s%sapp_icon: %s%sreplaces_id: %u%stimeout: %d%s",
        app_name, delimiter, app_icon, delimiter, replaces_id,
        delimiter, timeout, delimiter);

    printf("hints:%s", delimiter);

    char *str_format    = calloc(64, sizeof(char)), // should be enough
        *int_format     = calloc(64, sizeof(char)),
        *uint_format    = calloc(64, sizeof(char)),
        *double_format  = calloc(64, sizeof(char)),
        *boolean_format = calloc(64, sizeof(char)),
        *byte_format    = calloc(64, sizeof(char)),
        *err_format     = calloc(64, sizeof(char));

    sprintf(str_format,     "\t%%s: %%s%s", delimiter);
    sprintf(int_format,     "\t%%s: %%d%s", delimiter);
    sprintf(uint_format,    "\t%%s: %%u%s", delimiter);
    sprintf(double_format,  "\t%%s: %%f%s", delimiter);
    sprintf(boolean_format, "\t%%s: %%x%s", delimiter);
    sprintf(byte_format,    "\t%%s: %%d%s", delimiter);
    sprintf(err_format,     "\t%%s:%s", delimiter);

    hints_output_iterator(hints,
        str_format, int_format, uint_format, double_format, boolean_format,
        byte_format, err_format, "");

    free(str_format);
    free(int_format);
    free(uint_format);
    free(double_format);
    free(boolean_format);
    free(byte_format);
    free(err_format);

    printf("actions:%s", delimiter);

    unsigned int index = 0;
    while (actions[index] && actions[index + 1]) {
        printf("\t%s: %s%s", actions[index + 1], actions[index], delimiter);
        index += 2;
    }

    printf("summary: %s%sbody: %s%s",
        summary, delimiter, body, delimiter);

}

void json_output(gchar *app_name, gchar *app_icon, guint32 replaces_id,
    gint32 timeout, GVariant *hints, gchar **actions, gchar *summary,
    gchar *body) {

    printf("{"
        "\"app_name\": \"%s\", "
        "\"app_icon\": \"%s\", "
        "\"replaces_id\": %u, "
        "\"timeout\": %d, ",
        app_name, app_icon, replaces_id, timeout);

    printf("\"hints\": {");
    hints_output_iterator(hints, "\"%s\": \"%s\"", "\"%s\": %d", "\"%s\": %u",
        "\"%s\": %f", "\"%s\": %x", "\"%s\": %d", "\"%s\": \"\"", ", ");
    printf("}, \"actions\": {");

    unsigned int index = 0;
    while (actions[index] && actions[index + 1]) {
        if (index)
            printf(", ");
        printf("\"%s\": \"%s\"", actions[index + 1], actions[index]);
        index += 2;
    }

    printf("}, ");
    printf("\"summary\": \"%s\", "
        "\"body\": \"%s\"}\n", summary, body);

}
