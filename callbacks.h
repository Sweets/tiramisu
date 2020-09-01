#pragma once

#include <gio/gio.h>
#include <glib.h>

#define GT_STRING G_VARIANT_TYPE_STRING
#define GT_INT16 G_VARIANT_TYPE_INT16
#define GT_INT32 G_VARIANT_TYPE_INT32
#define GT_INT64 G_VARIANT_TYPE_INT64
#define GT_UINT16 G_VARIANT_TYPE_UINT16
#define GT_UINT32 G_VARIANT_TYPE_UINT32
#define GT_UINT64 G_VARIANT_TYPE_UINT64
#define GT_DOUBLE G_VARIANT_TYPE_DOUBLE
#define GT_BOOL G_VARIANT_TYPE_BOOLEAN
#define GT_BYTE G_VARIANT_TYPE_BYTE

extern unsigned int notification_id;

void method_handler(GDBusConnection*, const gchar*, const gchar*, const gchar*,
    const gchar*, GVariant*, GDBusMethodInvocation*, gpointer);

void bus_acquired(GDBusConnection*, const gchar*, gpointer);
void name_acquired(GDBusConnection*, const gchar*, gpointer);
void name_lost(GDBusConnection*, const gchar*, gpointer);
