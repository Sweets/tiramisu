MainLoop loop;

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

    // notification_closed
    // close_notification
    // action_invoked
}

int main() {
    loop = new MainLoop();
    Bus.own_name(BusType.SESSION, "org.freedesktop.Notifications",
        BusNameOwnerFlags.DO_NOT_QUEUE,
        (dbus_connection) => {
            // Bus acquired, register object
            try {
                dbus_connection.register_object(
                    "/org/freedesktop/Notifications",
                    new Tiramisu()
                );
            } catch (IOError _error) {
                error("Bus not acquired; unable to register object");
            }
        },
        () => {}, // Name acquired
        () => {
            error("Unable to acquired name org.freedesktop.Notifications");
        });
    loop.run();

    return 0;
}
