string sanitize(string subj) {
    return subj.replace("'", "\\'");
}

public class Notification : Object {
    public static void output(string source, uint replaces_id, string icon,
        string summary, string body, string[] actions,
        GLib.HashTable<string, GLib.Variant> hints, int timeout) {

        string app_name = source;
        string app_icon = icon;
        string _summary = summary;
        string _body    = body;

        string fmt = Tiramisu.format;

        if (Tiramisu.json)
            fmt = Tiramisu.json_format;

        if (Tiramisu.sanitize) {
            app_name = sanitize(app_name);
            app_icon = sanitize(app_icon);
            _summary = sanitize(_summary);
            _body    = sanitize(_body);
        }

        fmt = fmt
            .replace("#source",  app_name)
            .replace("#id",      replaces_id.to_string())
            .replace("#icon",    app_icon)
            .replace("#summary", _summary)
            .replace("#body",    _body)
            .replace("#actions", "TODO(actions)")
            .replace("#hints",   "TODO(hints)")
            .replace("#timeout", timeout.to_string());

        stdout.printf(fmt + "\n");
        stdout.flush();
    }
}
