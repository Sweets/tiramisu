public class Notification : Object {
    public static void output(string source, uint replaces_id, string icon,
        string summary, string body, string[] actions,
        GLib.HashTable<string, GLib.Variant> hints, int timeout) {
        
        string fmt = Tiramisu.format;

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
}
