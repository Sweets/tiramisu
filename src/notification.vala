string sanitize(string subj) {
    return subj.replace("'", "\\'");
}

public class Notification : Object {
    public static string create_csv_hint_string(
        GLib.HashTable<string, GLib.Variant> hints) {
        // TODO: This implementation is not to my liking. Rewrite.

        string output = "";
        string buffer = "";
        string separator = "";

        /* Image data bullshit */
        GLib.VariantType image_type = new GLib.VariantType("(iiibiiay)");

        uint width = 0, height = 0, row_stride = 0, bits_per_sample = 0,
            channels = 0;
        bool alpha = false;

        uint8[] pixels = {};
        GLib.Variant pixel_data = null;
        string value_string = "";

        hints.foreach((key, value) => {
            string _key = key;
            string _value = value.print(false);

            if (Tiramisu.sanitize) {
                _key = sanitize(_key);

                if (value.is_of_type(GLib.VariantType.STRING)) {
                    _value = sanitize(_value.substring(1, _value.length - 2));
                    _value = @"'$(_value)'";
                }
            }

            if (Tiramisu.json)
                buffer = buffer.concat(separator, @"'$(_key)': ");
            else
                buffer = buffer.concat(separator, _key, "=");

            if (value.is_of_type(image_type)) {
                value.get("(iiibii@ay)",
                    out width, out height, out row_stride, out alpha,
                    out bits_per_sample, out channels, out pixel_data);

                pixels = pixel_data.get_data_as_bytes().get_data();
                string encoded_image = GLib.Base64.encode((uchar[])pixels);

                value_string = "".concat(@"$(width):$(height):$(row_stride):",
                    @"$(alpha):$(bits_per_sample):$(channels):", encoded_image);

                if (Tiramisu.json)
                    buffer = buffer.concat(@"'$(value_string)'");
                else
                    buffer = buffer.concat(value_string);
            } else
                buffer = buffer.concat(_value);

            output = output.concat(buffer);
            buffer = "";
            separator = ",";
        });

        if (Tiramisu.json)
            output = @"{$(output)}";
        else
            output = @"'$(output)'";

        return output;
    }

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

        string hint_csv = create_csv_hint_string(hints);

        fmt = fmt
            .replace("#source",  app_name)
            .replace("#id",      replaces_id.to_string())
            .replace("#icon",    app_icon)
            .replace("#summary", _summary)
            .replace("#body",    _body)
            .replace("#actions", "TODO(actions)")
            .replace("#hints",   hint_csv)
            .replace("#timeout", timeout.to_string());

        stdout.printf(fmt + "\n");
        stdout.flush();
    }
}
