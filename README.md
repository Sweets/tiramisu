<p align="center">
  <b>tiramisu</b><br/>
  desktop notifications, the UNIX way
</p>

---

tiramisu is a notification daemon for \*nix desktops that implement notifications using dbus.

Unlike other daemons, tiramisu does not have any sort of window or pop-up, but rather sends all notifications to STDOUT. Doing so enables endless customization from the end-user.

---

<p align="center">
  <b>Crafted with ♡</b>
</p>

- [anufrievroman/polytiramisu](https://github.com/anufrievroman/polytiramisu)

---

<p align="center">
  <b>Installation</b>
</p>

Tiramisu depends upon Vala, gio, and glib.

|Distribution|Repository|Package name|
|-|-|-|
|Arch Linux|AUR|`tiramisu-git`|
|Alpine Linux|v3.15+|`tiramisu`|
|NixOS|stable|`nixos.tiramisu`|

Don't see your distribution? Check to make sure it wasn't forgotten at [repology](https://repology.org/projects/?search=tiramisu).
Alternatively, build from source.

```sh
$ git clone https://github.com/Sweets/tiramisu
$ cd ./tiramisu
$ make && make install
```

---

<p align="center">
  <b>Usage</b>
</p>

By default, tiramisu outputs all information from a notification to standard output. You can change this with `-o`, or if you wish to use JSON format, `-j`. If you need the output format to be sanitized (quotes to be escaped), you can do so with `-s`.

Using `-o` will interpolate your desired format.

Appropriate keys are `#source`, `#icon`, `#id`, `#summary`, `#body`, `#actions`, `#hints`, and `#timeout`. 

Using `-j` implies `-s`.

Below is an example of the default output of tiramisu with no flags.

```
evolution-mail-notification
evolution
0
New email in Evolution
You have received 4 new messages.
desktop-entry=org.gnome.Evolution|urgency=1
Show INBOX=default
-1
```
