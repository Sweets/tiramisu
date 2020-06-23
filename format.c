#include <stdio.h>

#include "config.h"
#include "format.h"

void escape_quotes(char *str, char *out) {
    memset(out, 0, strlen(out));
    while (*str) {
        if (*str == '"')
            strcat(out, "\\\"");
        else
            out[strlen(out)] = *str;
        str++;
    }
}

void output_notification(gchar *app_name, guint32 replaces_id, gchar *app_icon,
    gchar *summary, gchar *body, GVariant *actions, GVariant *hints,
    gint32 timeout) {
    /* This function will raise warnings. I don't fucking care. */

    /* 2048 characters should be significantly long enough*/
    char *string = (char *)calloc(2048, sizeof(char));

#ifndef PRINT_JSON
    strcat(string, app_name);

#ifdef RECEIVE_APP_ICON
    sprintf(string, "%s%s%s", string, OUTPUT_DELIMITER, app_icon);
#endif

    /* TODO: actions */
    /* TODO: hints */

#ifdef RECEIVE_REPLACES_ID
    sprintf(string, "%s%s%lu", string, OUTPUT_DELIMITER, replaces_id);
#endif

    sprintf(string, "%s%s%d%s%s%s%s", string, OUTPUT_DELIMITER,
        timeout, OUTPUT_DELIMITER, summary, OUTPUT_DELIMITER, body);
#else
    char *escaped_str = (char *)calloc(512, sizeof(char));
    escape_quotes(app_name, escaped_str);
    sprintf(string, "{ \"app_name\": \"%s\"", escaped_str);

#ifdef RECEIVE_APP_ICON
    escape_quotes(app_icon, escaped_str);
    sprintf(string, "%s, \"app_icon\": \"%s\"", string, escaped_str);
#endif

#ifdef RECEIVE_REPLACES_ID
    sprintf(string, "%s, \"replaces_id\": %lu", string, replaces_id);
#endif

    escape_quotes(summary, escaped_str);
    sprintf(string, "%s, \"timeout\": %d, \"summary\": \"%s\"", string, timeout, escaped_str);

    escape_quotes(body, escaped_str);
    sprintf(string, "%s, \"body\": \"%s\" }", string, escaped_str);

#endif

    printf("%s\n", string);
    fflush(stdout);

    free(string);
    free(escaped_str);
}
