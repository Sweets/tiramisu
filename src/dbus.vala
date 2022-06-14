[DBus (name = "org.freedesktop.Notifications")]
public class NotificationDaemon : Object {
    public static uint notification_id = 1;

    [DBus (name = "GetServerInformation")]
    public void get_server_information(out string name,
        out string vendor, out string version, out string spec_version)
        throws DBusError, IOError {
        name = "tiramisu";
        vendor = "Sweets";
        version = "2.0";
        spec_version = "1.2";
    }

    [DBus (name = "GetCapabilities")]
    public string[] get_capabilities() throws DBusError, IOError {
        return {"body", "actions", "icon-static"};
    }

    [DBus (name = "Notify")]
    public uint Notify(string app_name, uint replaces_id, string app_icon,
        string summary, string body, string[] actions,
        GLib.HashTable<string, GLib.Variant> hints,
        int expire_timeout) throws DBusError, IOError {

        Notification.output(app_name, replaces_id, app_icon, summary,
            body, actions, hints, expire_timeout);

        if (replaces_id == 0)
            return notification_id++;
        
        return replaces_id;
    }

    [DBus (name = "CloseNotification")]
    public void close_notification(uint id) throws DBusError, IOError {
        // close notification
    }

    [DBus (name = "NotificationClosed")]
    public void notification_closed(uint id,
        uint reason) throws DBusError, IOError {
        // notification was closed
    }
    

    // action_invoked
}

