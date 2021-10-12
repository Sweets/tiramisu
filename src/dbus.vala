[DBus (name = "org.freedesktop.Notifications")]
public class Tiramisu : Object {
    [DBus (name = "GetServerInformation")]
    public void get_server_information(out string name,
        out string vendor, out string version, out string spec_version)
        throws DBusError, IOError {
        name = "tiramisu";
        vendor = "Sweets";
        version = "1.2";
        spec_version = "1.2";
    }

    [DBus (name = "GetCapabilities")]
    public string[] get_capabilities() throws DBusError, IOError {
        return {"body"};
    }

    [DBus (name = "Notify")]
    public uint Notify(string app_name, uint replaces_id, string app_icon,
        string summary, string body, string[] actions,
        GLib.HashTable<string, GLib.Variant> hints,
        int expire_timeout) throws DBusError, IOError {

        message("Notification data received");

        return 0;
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

