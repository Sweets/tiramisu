#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <glib.h>

#include "notification.h"

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
    // TODO: hints

    sprintf(itoa_buffer, "%i", notification.timeout);
    strreplsub(format_string, "#timeout", itoa_buffer);
}

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
