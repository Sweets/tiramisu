public class Tiramisu : Application {
    private static string format = "#source\n#icon\n#id\n#summary\n" +
        "#body\n#actions\n#hints\n#timeout\n";

    private static string json_format = "{'source': '#source', " +
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

    public static void output(string source, uint replaces_id, string icon,
        string summary, string body, string[] actions,
        GLib.HashTable<string, GLib.Variant> hints, int timeout) {
        
        string fmt = Tiramisu.json_format;

        fmt = fmt
            .replace("#source", source)
            .replace("#id", replaces_id.to_string())
            .replace("#icon", icon)
            .replace("#summary", summary)
            .replace("#body", body)
            .replace("#actions", "TODO(actions)")
            .replace("#hints", "TODO(hints)")
            .replace("#timeout", timeout.to_string());

        stdout.printf(fmt);
        stdout.flush();
    }

    public static int main(string[] arguments) {
        Tiramisu tiramisu = new Tiramisu();
        return tiramisu.run(arguments);
    }
}
