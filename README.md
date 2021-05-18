<h1><div align="center"><i>desktop notifications, the unix way</i></div></h1>

tiramisu is a notification daemon based on dunst that outputs notifications
to STDOUT in order to allow the user to process notifications any way they prefer.

# why?

By allowing users to determine what is done with notifications, there is infinitely more possibilities presented
to the end-user than a simple notification daemon that displays a block of text on the screen and nothing more.

Users could have notifications display in a pre-existing bar, make a control panel of some sort that shows
notifications, push notifications to their phone if their computer has been idle for an amount of time,
make notifications more accessible with text-to-speech, and so much more.

# projects made with tiramisu and love

- [polynotifications by alnj](https://github.com/alnj/polynotifications)

# installation

Clone the repository and build the project. Then move tiramisu to a location that is specified in `$PATH`.

```
$ git clone https://github.com/Sweets/tiramisu
$ cd ./tiramisu
$ make
# make install
```

#### Note that the use of a pound symbol (#) denotes escalated privileges.
#### On most Linux systems this can be done with the usage of `sudo`

# usage

tiramisu accepts a single parameter, that being `-o`, which allows users to specify an output format.
The output format specified is interpolated by tiramisu. Listed below are values that are interpolated before output.

The values are listed by their appearance in the default output format.

`#source`, `#icon`, `#id`, `#summary`, `#body`, `#actions`, `#hints`, `#timeout`

#### Note that to use linebreaks you will need to actually have a linebreak in the output string.
#### tiramisu will not collapse \X to the appropriate sequence.

### Example of default output

```
$ tiramisu
```

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

#### Note that only a single process can claim the org.freedesktop.Notifications name at a given time, so any other running notification daemon must be killed before running tiramisu.
