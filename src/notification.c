#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <glib.h>

#include "notification.h"

/* Pain. Suffering. */

#define GT_STRING G_VARIANT_TYPE_STRING
#define GT_INT16  G_VARIANT_TYPE_INT16
#define GT_INT32  G_VARIANT_TYPE_INT32
#define GT_INT64  G_VARIANT_TYPE_INT64
#define GT_UINT16 G_VARIANT_TYPE_UINT16
#define GT_UINT32 G_VARIANT_TYPE_UINT32
#define GT_UINT64 G_VARIANT_TYPE_UINT64
#define GT_DOUBLE G_VARIANT_TYPE_DOUBLE
#define GT_BOOL   G_VARIANT_TYPE_BOOLEAN
#define GT_BYTE   G_VARIANT_TYPE_BYTE

static void create_csv_hint_string(GVariant *, char **);
static void strreplsub(char **, const char*, const char*);
static void strsanitize(char **);

void hydrate_notification(notification_t *notification, GVariant *parameters) {
    GVariantIter iterator;

    g_variant_iter_init(&iterator, parameters);

    g_variant_iter_next(&iterator, "s",      &notification->source);
    g_variant_iter_next(&iterator, "u",      &notification->id);
    g_variant_iter_next(&iterator, "s",      &notification->icon);
    g_variant_iter_next(&iterator, "s",      &notification->summary);
    g_variant_iter_next(&iterator, "s",      &notification->body);
    g_variant_iter_next(&iterator, "^a&s",   &notification->actions);
    g_variant_iter_next(&iterator, "@a{sv}", &notification->hints);
    g_variant_iter_next(&iterator, "i",      &notification->timeout);
}

void interpolate_output_string(char **format_string, notification_t notification) {
    /*
     * #actions => harder to interpolate, we'll come back to this
     * #hints   => harder to interpolate, we'll come back to this
     */

    char itoa_buffer[11]; // itoa() isn't standard, so sprintf.
    sprintf(itoa_buffer, "%i", notification.id);

    strreplsub(format_string, "#source",  notification.source);
    strreplsub(format_string, "#id",      itoa_buffer);
    strreplsub(format_string, "#icon",    notification.icon);
    strreplsub(format_string, "#summary", notification.summary);
    strreplsub(format_string, "#body",    notification.body);
    // TODO: actions

    char *hints = NULL;
    create_csv_hint_string(notification.hints, &hints);
    strreplsub(format_string, "#hints", hints);
    free(hints);

    sprintf(itoa_buffer, "%i", notification.timeout);
    strreplsub(format_string, "#timeout", itoa_buffer);
}

/* Supporting methods */

static void create_csv_hint_string(GVariant *hints, char **string_ptr) {
    char *string = calloc(1, sizeof(char));

    GVariantIter iterator;
    gchar *key;
    GVariant *value;

    const void *mapping_table[9][3] = {
        { GT_INT16,  g_variant_get_int16,   "%d" },
        { GT_INT32,  g_variant_get_int32,   "%d" },
        { GT_INT64,  g_variant_get_int64,   "%d" },
        { GT_UINT16, g_variant_get_uint16,  "%u" },
        { GT_UINT32, g_variant_get_uint32,  "%u" },
        { GT_UINT64, g_variant_get_uint64,  "%u" },
        { GT_DOUBLE, g_variant_get_double,  "%f" },
        { GT_BYTE,   g_variant_get_byte,    "%x" },
        { GT_BOOL,   g_variant_get_boolean, "%d" }
    };

    char *temp;
    char *str_val;
    int str_len = 0;

    /* image processing */
    const GVariantType *image_type = G_VARIANT_TYPE("(iiibiiay)");

    char itoa_buffer[11];

    int height;
    int width;
    int bits_per_sample;
    int channels;
    int row_stride;
    int alpha_bool;

    gsize expected_length;
    gsize real_length;
    gsize pixel_stride;
    guint8 *pixels;
    GVariant *pixel_data;

    char *encoded_image;

    g_variant_iter_init(&iterator, hints);
    while (g_variant_iter_loop(&iterator, "{sv}", &key, NULL)) {
        if ((value = g_variant_lookup_value(hints, key, GT_STRING))) {
            str_val = (char *)g_variant_get_string(value, NULL);

            if (strlen(string) == 0) {
                string = (char *)realloc(string,
                    (strlen(str_val) + strlen(key) + 3));

                snprintf(string,
                    (strlen(str_val) + strlen(key) + 3),
                    "%s=%s", key, str_val);
            } else {
                temp = strdup(string);
                string = (char *)realloc(string,
                    (strlen(string) + strlen(str_val) + strlen(key)) + 3);

                snprintf(string,
                    strlen(str_val) + strlen(string) + strlen(key) + 3,
                    "%s,%s=%s", temp, key, str_val);

                free(temp);
            }
        } else if (g_variant_lookup_value(hints, key, image_type)) {
            g_variant_get(value, "(iiibii@ay)", &width, &height, &row_stride,
                &alpha_bool, &bits_per_sample, &channels, &pixel_data);
            pixel_stride = (channels * bits_per_sample + 7) / 8;
            expected_length = (height - 1) * (width * row_stride) *
                pixel_stride;

            pixels = g_variant_get_fixed_array(pixel_data, &real_length,
                sizeof(guint8));

            if (expected_length != real_length)
                continue;
            str_len = 0;

            // Lot of copying and pasting... This could be done slightly more
            // efficiently.
            sprintf(itoa_buffer, "%i", width);
            str_len += strlen(itoa_buffer);

            sprintf(itoa_buffer, "%i", height);
            str_len += strlen(itoa_buffer);

            sprintf(itoa_buffer, "%i", row_stride);
            str_len += strlen(itoa_buffer);

            sprintf(itoa_buffer, "%i", alpha_bool);
            str_len += strlen(itoa_buffer);

            sprintf(itoa_buffer, "%i", bits_per_sample);
            str_len += strlen(itoa_buffer);

            sprintf(itoa_buffer, "%i", channels);
            str_len += strlen(itoa_buffer);

            encoded_image = (char *)g_base64_encode(pixels, real_length);

            str_val = calloc(str_len + strlen(encoded_image) + 7,
                sizeof(char));
            snprintf(str_val, str_len + strlen(encoded_image) + 7,
                "%i:%i:%i:%i:%i:%i:%s", width, height, row_stride, alpha_bool,
                bits_per_sample, channels, encoded_image);

            if (strlen(string) == 0) {
                string = (char *)realloc(string, strlen(key) +
                    strlen(str_val) + 3);
                snprintf(string, strlen(key) + strlen(str_val) + 3,
                    "%s=%s", key, str_val);
            } else {
                temp = strdup(string);
                string = (char *)realloc(string, strlen(string) + strlen(key) +
                    strlen(str_val) + 3);
                snprintf(string, strlen(string) + strlen(key) +
                    strlen(str_val) + 3, "%s,%s=%s", temp, key, str_val);
                free(temp);
            }

            free(str_val);
            free(encoded_image);
        } else {
            for (unsigned int index = 0; index < 9; index++) {
                const void *gt_type = mapping_table[index][0];
                const (*gt_method)() = mapping_table[index][1];
                const char *str_fmt = mapping_table[index][2];

                if ((value = g_variant_lookup_value(hints, key, gt_type))) {
                    str_val = calloc(11, sizeof(char));
                    sprintf(str_val, str_fmt, gt_method(value));

                    if (strlen(string) == 0) {
                        string = (char *)realloc(string,
                            (strlen(str_val) + strlen(key)) + 3);
                        snprintf(string,
                            (strlen(key) + strlen(str_val) + 3),
                            "%s=%s", key, str_val);
                    } else {
                        temp = strdup(string);
                        string = (char *)realloc(string,
                            (strlen(string) + strlen(str_val) +
                             strlen(key)) + 3);

                        snprintf(string,
                            (strlen(string) + strlen(key) +
                             strlen(str_val) + 3),
                            "%s,%s=%s", temp, key, str_val);

                        free(temp);
                    }

                    free(str_val);
                }
            }
        }

        if (value)
            g_variant_unref(value);
    }

    g_variant_unref(hints);
    *string_ptr = string;
}

/* String manipulation methods */

static void strreplsub(char **_haystack, const char *needle, const char *replacement) {
    char *haystack = *_haystack;
    int replacements = 0;
    size_t substr_length = strlen(needle);
    size_t replacement_length = strlen(replacement);

    char *ptr = strdup(haystack);
    while ((ptr = strstr(ptr, needle))) {
        ptr += substr_length;
        ++replacements;
    }

    free(ptr);

    if (!replacements)
        return;

    int output_size = (strlen(haystack) - (replacements * substr_length))
        + (replacements * replacement_length) + 1;
    char *output = calloc(output_size, sizeof(char));

    char *pre_ptr = strdup(haystack);
    char *post_ptr;
    int index = 0;

    free(haystack);
    ptr = pre_ptr;

    for (; index < replacements; index++) {
        post_ptr = strstr(pre_ptr, needle);
        strncat(output, pre_ptr, post_ptr - pre_ptr);
        strncat(output, replacement, replacement_length);
        pre_ptr = post_ptr + substr_length;
    }

    strncat(output, post_ptr + substr_length,
        (strlen(post_ptr) - substr_length) + replacement_length);
    *_haystack = output;

    free(ptr);
}

static void strsanitize(char **_string) {
    char *string = *_string;

    int index, quotes, output_index = 0;
    char cell;

    while ((cell = string[index++]))
        if (cell == '"')
            quotes++;

    if (!quotes)
        return;

    index = 0;

    char *output = calloc(strlen(string) + quotes + 1, sizeof(char));

    while ((cell = string[index])) {
        if (cell == '"')
            output[output_index++] = '\\';

        output[output_index++] = string[index++];
    }

    free(string);
    *_string = output;
}
