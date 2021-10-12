MainLoop loop;

[DBus (name = "org.freedesktop.Notifications")]
public class Tiramisu : Object {
    public string[] get_server_information() throws GLib.Error {
        return {"tiramisu", "Sweets", "1.2", "1.2"};
    }

    public string[] get_capabilities() throws GLib.Error {
        return {"body", "actions", "icon-static"};
    }
}

/*
 * TODO: move the signal methods
 */


/*
 * TODO: move above signal methods to a new file
 */


int main() {
    Bus.own_name(BusType.SESSION, "org.freedesktop.Notifications",
        BusNameOwnerFlags.NONE,
        (dbus_connection) => {
            // Bus acquired, register object
            try {
                dbus_connection.register_object(
                    "/org/freedesktop/Notifications",
                    new Tiramisu()
                );
            } catch (IOError error) {
                stderr.printf("Bus not acquired; unable to register object");
            }
        },
        () => {}, // Name acquired
        () => stderr.printf("name not acquired\n"));
    loop = new MainLoop();
    loop.run();

    return 0;
}
