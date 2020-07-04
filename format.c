#include <stdio.h>

#include "config.h"
#include "format.h"
#include "tiramisu.h"

char* escape_quotes(char *str, char *out) {
    memset(out, 0, strlen(out));
    while (*str) {
        if (*str == '"')
            strcat(out, "\\\"");
        else
            out[strlen(out)] = *str;
        str++;
    }

    return out;
}

void output_notification(gchar *app_name, guint32 replaces_id, gchar *app_icon,
    gchar *summary, gchar *body, GVariant *actions, GVariant *hints,
    gint32 timeout) {

    char *escaped_string = (char *)calloc(512, sizeof(char));

#ifdef PRINT_JSON

    printf("{ \"app_name\": \"%s\",", escape_quotes(app_name, escaped_string));
    printf(" \"app_icon\": \"%s\",", escape_quotes(app_icon, escaped_string));
    printf(" \"replaces_id\": %u, \"timeout\": %d,", replaces_id, timeout);
    printf(" \"hints\": { ");

    const char *int_format = ", \"%s\": %d";
    const char *uint_format = ", \"%s\": %u";

    GVariantIter iterator;
    gchar *key;
    GVariant *v; // Short-hand for `value`

    g_variant_iter_init(&iterator, hints);
    while (g_variant_iter_loop(&iterator, "{s*}", &key, NULL)) {

        /* Horrible code, but for now it's just in testing. */

        if ((v = g_variant_lookup_value(hints, key, G_VARIANT_TYPE_STRING)))
            printf(", \"%s\": \"%s\"", key,
                escape_quotes(g_variant_dup_string(v, NULL), escaped_string));
        else if ((v = g_variant_lookup_value(hints, key, G_VARIANT_TYPE_INT16)))
            printf(int_format, key, g_variant_get_int16(v));
        else if ((v = g_variant_lookup_value(hints, key, G_VARIANT_TYPE_INT32)))
            printf(int_format, key, g_variant_get_int32(v));
        else if ((v = g_variant_lookup_value(hints, key, G_VARIANT_TYPE_INT64)))
            printf(int_format, key, g_variant_get_int64(v));
        else if ((v = g_variant_lookup_value(hints, key,G_VARIANT_TYPE_UINT16)))
            printf(uint_format, key, g_variant_get_uint16(v));
        else if ((v = g_variant_lookup_value(hints, key,G_VARIANT_TYPE_UINT32)))
            printf(uint_format, key, g_variant_get_uint32(v));
        else if ((v = g_variant_lookup_value(hints, key,G_VARIANT_TYPE_UINT64)))
            printf(uint_format, key, g_variant_get_uint64(v));
        else if ((v = g_variant_lookup_value(hints, key,G_VARIANT_TYPE_DOUBLE)))
            printf(", \"%s\": %f", key, g_variant_get_double(v));
        else if ((v=g_variant_lookup_value(hints,key,G_VARIANT_TYPE_BOOLEAN))) {
            if (g_variant_get_boolean(v))
                printf(", \"%s\": 1", key);
            else
                printf(", \"%s\": 0", key);
        } else if ((v = g_variant_lookup_value(hints, key,G_VARIANT_TYPE_BYTE)))
            printf(", \"%s\": %x", key, g_variant_get_byte(v));

    }

    printf(" },");
    printf(" \"summary\": \"%s\",", escape_quotes(summary, escaped_string));
    printf(" \"body\": \"%s\" }\n", escape_quotes(body, escaped_string));

#else

    printf("%s%s%s%s%u%s%d%s%s%s%s\n",
        app_name, OUTPUT_DELIMITER,
        app_icon, OUTPUT_DELIMITER,
        replaces_id, OUTPUT_DELIMITER,
        timeout, OUTPUT_DELIMITER,
        summary, OUTPUT_DELIMITER, body);

#endif

    fflush(stdout);
    free(escaped_string);
}
