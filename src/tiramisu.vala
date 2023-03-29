public class Tiramisu : Application {
    public static string format = "#source\n#icon\n#id\n#summary\n" +
        "#body\n#actions\n#hints\n#timeout";
    public static string json_format = "{\"source\": \"#source\", " +
        "\"id\": #id, \"summary\": \"#summary\", \"body\": \"#body\", " + 
        "\"icon\": \"#icon\", \"actions\": \"#actions\", \"hints\": #hints, " +
        "\"timeout\": #timeout}";

    public static bool sanitize = false;
    public static bool json     = false;

    private const OptionEntry[] options = {
        {"format",   'o', OptionFlags.NONE, OptionArg.STRING, ref format,
            "Output format specifier",                "FORMAT"},

        {"json",     'j', OptionFlags.NONE, OptionArg.NONE,   ref json,
            "Output using JSON (implies --sanitize)", null},

        {"sanitize", 's', OptionFlags.NONE, OptionArg.NONE,   ref sanitize,
            "Sanitize output; escapes quotes", null},
        {null}
    };

    private Tiramisu() {
        this.add_main_option_entries(options);
    }

    public override void activate() {
        this.hold();

        if (json)
            sanitize = true;

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
