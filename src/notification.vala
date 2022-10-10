string sanitize(string subj) {
    return subj
        .replace("\"", "\\\"")
        .replace("\r", "\\r")
        .replace("\n", "\\n"); // ideally all control sequences \u0000 to \u001f
}

public class Notification : Object {
    public static string image_get_base64_representation(GLib.Variant image) {
        uint width = 0, height = 0, row_stride = 0, bits_per_sample = 0,
            channels = 0;
        bool alpha = false;

        uint8[] pixels = {};
        GLib.Variant pixel_data = null;

        image.get("(iiibii@ay)",
            out width, out height, out row_stride, out alpha,
            out bits_per_sample, out channels, out pixel_data);

        pixels = pixel_data.get_data_as_bytes().get_data();
        string encoded_image = GLib.Base64.encode((uchar[])pixels);

        return "".concat(@"$(width):$(height):$(row_stride):",
            @"$(alpha):$(bits_per_sample):$(channels):", encoded_image);
    }

    public static string create_hint_json_string(
        GLib.HashTable<string, GLib.Variant> hints) {
        // Only if Tiramisu.json is true. Implies Tiramisu.sanitize.

        string output = "";
        string buffer = "";
        string separator = "";

        GLib.VariantType image_type = new GLib.VariantType("(iiibiiay)");

        hints.foreach((key, value) => {
            string _key   = key;
            string _value = "";
            
            _key   = sanitize(_key);
            buffer = buffer.concat(separator, @"\"$(_key)\": ");

            if (value.is_of_type(GLib.VariantType.STRING)) {
                _value = value.print(false);
                _value = _value.substring(1, _value.length - 2);
                _value = sanitize(_value);
                _value = @"\"$(_value)\"";
            } else if (value.is_of_type(image_type)) {
                _value = @"\"$(image_get_base64_representation(value))\"";
            } else {
                _value = @"\"$(value.print(false))\"";
            }

            buffer    = buffer.concat(@"$(_value)");
            output    = output.concat(buffer);

            buffer    = "";
            separator = ", ";
        });

        return @"{$(output)}";
    }

    public static string create_hint_csv_string(
        GLib.HashTable<string, GLib.Variant> hints) {

        string output = "";
        string buffer = "";
        string separator = "";
        
        GLib.VariantType image_type = new GLib.VariantType("(iiibiiay)");

        hints.foreach((key, value) => {
            string _key   = key;
            string _value = "";

            if (value.is_of_type(GLib.VariantType.STRING)) {
                _value = value.print(false);
                _value = _value.substring(1, _value.length - 2);
                _value = sanitize(_value);
                _value = @"'$(_value)'";
            } else if (value.is_of_type(image_type)) {
                _value = image_get_base64_representation(value);
            } else {
                _value = value.print(false);
            }

            if (Tiramisu.sanitize) {
                _key   = sanitize(_key);
                _value = sanitize(_value);
            }

            buffer    = buffer.concat(separator, @"$(_key)=$(_value)");
            output    = output.concat(buffer);

            buffer    = "";
            separator = ",";
        });

        return output;
    }

    public static void output(string source, uint replaces_id, string icon,
        string summary, string body, string[] actions,
        GLib.HashTable<string, GLib.Variant> hints, int timeout) {

        string app_name = source;
        string app_icon = icon;
        string _summary = summary;
        string _body    = body;

        string hint_string = "";
        string fmt = Tiramisu.format;

        if (Tiramisu.json) {
            fmt = Tiramisu.json_format;
            hint_string = create_hint_json_string(hints);
        } else {
            hint_string = create_hint_csv_string(hints);
        }

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
            .replace("#actions", string.joinv(",", actions))
            .replace("#hints",   hint_string)
            .replace("#timeout", timeout.to_string());

        stdout.printf(fmt + "\n");
        stdout.flush();
    }
}
