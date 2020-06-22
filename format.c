#include <stdio.h>

#include "config.h"
#include "format.h"

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
    sprintf(string, "{ \"app_name\": \"%s\"", app_name);

#ifdef RECEIVE_APP_ICON
    sprintf(string, "%s, \"app_icon\": \"%s\"", string, app_icon);
#endif

#ifdef RECEIVE_REPLACES_ID
    sprintf(string, "%s, \"replaces_id\": %lu", string, replaces_id);
#endif

    sprintf(string, "%s, \"timeout\": %d, \"summary\": \"%s\", \"body\": \"%s\" }", string, timeout, summary, body);

#endif

    printf("%s\n", string);
    fflush(stdout);

    free(string);
}
