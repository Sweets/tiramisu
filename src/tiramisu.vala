public class Tiramisu : Application {
    public static string format = "#source\n#icon\n#id\n#summary\n" +
        "#body\n#actions\n#hints\n#timeout\n";

    public static string json_format = "{'source': '#source', " +
        "'id': #id, 'summary': '#summary', 'body': '#body', " +
        "'actions': '#actions', 'hints': '#hints', 'timeout': #timeout}\n";

    private Tiramisu() {
    }

    public override void activate() {
        this.hold();
        Bus.own_name(BusType.SESSION, "org.freedesktop.Notifications",
            BusNameOwnerFlags.DO_NOT_QUEUE,
            (connection) => {
                try {
                    connection.register_object("/org/freedesktop/Notifications",
                        new NotificationDaemon());
                } catch (IOError _error) {
                    error("Unable to register object path.");
                }
            }, // Bus acquired
            () => {
            }, // Name acquired
            () => {
                error("Unable to acquired DBus name.");
            } // Unable to acquire
        );
    }

    public static int main(string[] arguments) {
        Tiramisu tiramisu = new Tiramisu();
        return tiramisu.run(arguments);
    }
}
