#include <stdio.h>

#include "config.h"
#include "format.h"

void output_notification(gchar *app_name, guint32 replaces_id, gchar *app_icon,
    gchar *summary, gchar *body, GVariant *actions, GVariant *hints,
    gint32 timeout) {
    /* This function will raise warnings. I don't fucking care. */

    /* 2048 characters should be significantly long enough*/
    char *string = (char *)calloc(2048, sizeof(char));

    strcat(string, app_name);

#ifdef RECEIVE_APP_ICON
    sprintf(string, "%s\n%s", string, app_icon);
#endif

    /* TODO: actions */
    /* TODO: hints */

#ifdef RECEIVE_REPLACES_ID
    sprintf(string, "%s\n%lu", string, replaces_id);
#endif

    sprintf(string, "%s\n%d\n%s\n%s", string, timeout, summary, body);

    printf("%s\n", string);

    free(string);
}
