#include <stdio.h>

#include "config.h"
#include "format.h"

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
    
#ifdef PRINT_JSON
    
    char *escaped_string = (char *)calloc(512, sizeof(char));
    
    printf("{ \"app_name\": \"%s\",", escape_quotes(app_name, escaped_string));
    printf(" \"app_icon\": \"%s\",", escape_quotes(app_icon, escaped_string));
    printf(" \"replaces_id\": %u, \"timeout\": %d,", replaces_id, timeout);
    printf(" \"summary\": \"%s\",", escape_quotes(summary, escaped_string));
    printf(" \"body\": \"%s\" }\n", escape_quotes(body, escaped_string));
    
    free(escaped_string);
    
#else
    
    printf("%s%s%s%s%u%s%d%s%s%s%s\n",
        app_name, OUTPUT_DELIMITER,
        app_icon, OUTPUT_DELIMITER,
        replaces_id, OUTPUT_DELIMITER,
        timeout, OUTPUT_DELIMITER,
        summary, OUTPUT_DELIMITER, body);
    
#endif

    fflush(stdout);
}
