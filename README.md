# Desktop notifications, the UNIX way

tiramisu is a notification daemon based on dunst that outputs notifications
to STDOUT in order to allow the user to process notifications any way they prefer.

<div align="center"><img src="https://github.com/Sweets/tiramisu/blob/master/example.gif"/></div>

# Why?

By allowing users to determine what is done with notifications, there is infinitely more possibilities presented
to the end-user than a simple notification daemon that displays a block of text on the screen and nothing more.

Users could have notifications display in a pre-existing bar, make a control panel of some sort that shows
notifications, push notifications to their phone if their computer has been idle for an amount of time,
make notifications more accessible with text-to-speech, and so much more.

# Projects Using tiramisu

- [polynotifications by alnj](https://github.com/alnj/polynotifications)

# Installation

Clone the repository and build the project. Then move tiramisu to a location that is specified in `$PATH`.

```
$ git clone https://github.com/Sweets/tiramisu
$ cd ./tiramisu
$ make

# cp ./tiramisu /usr/bin/tiramisu
# chmod +x /usr/bin/tiramisu
```

#### Note that the use of a pound symbol (#) denotes escalated privileges.
#### On most Linux systems this can be done with the usage of `sudo`

# Usage

Redirecting output of tiramisu to the input of another program is the ideal methodology to using
tiramisu.

```
tiramisu | your-application
```

By default tiramisu outputs notifications in a psuedo-key-value line format.
You can supply the `-j` flag to output notification data in JSON format.

### Example of default output

```
$ tiramisu
```

```
app_name: evolution-mail-notification
app_icon: evolution
replaces_id: 0
timeout: -1
hints:
    desktop-entry: org.gnome.Evolution
    urgency: 1
actions:
    Show INBOX: default
summary: New email in Evolution
body: You have received 4 new messages.
```

### Example of JSON output

```
$ tiramisu -j
```

```
{"app_name": "evolution-mail-notification", "app_icon": "evolution", "replaces_id": 0, "timeout": -1, "hints": {"desktop-entry": "org.gnome.Evolution", "urgency": 1}, "actions": {"Show INBOX": "default"}, "summary": "New email in Evolution", "body": "You have received 4 new messages."}
```

#### Note that only a single process can claim the org.freedesktop.Notifications name at a given time, so any other running notification daemon must be killed before running tiramisu.

##### Thanks to [BanchouBoo](https://github.com/BanchouBoo) for helping test tiramisu and providing the gif of it.
