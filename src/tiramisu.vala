MainLoop loop;

int main(string[] args) {
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
