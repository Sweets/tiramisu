# Desktop notifications, the UNIX way

tiramisu is a notification daemon based on dunst that outputs notifications
to STDOUT in order to allow the user to process notifications any way they prefer.

Perhaps you want notifications to be sent to your bar? tiramisu allows you to do that.

Maybe you want notifications to be redirected to your phone as push notifications? A little bit of elbow grease and some tiramisu would let you do that ;)

Notifications could be made more accessibility friendly even with TTS.

<div align="center"><img src="https://github.com/Sweets/tiramisu/blob/master/example.gif"/></div>

Thanks to [BanchouBoo](https://github.com/BanchouBoo) for helping test tiramisu and providing the gif of it.

# Projects Using tiramisu

- [polynotifications by alnj](https://github.com/alnj/polynotifications)

# Usage

Redirecting output of tiramisu to the input of another program is the ideal methodology to using
tiramisu.

```
tiramisu | your-application
```

By default tiramisu outputs notifications in JSON format. By removing the `PRINT_JSON` macro from config.h you can have tiramisu print out notifications straight to STDOUT, with each bit of information being split by a new line (the delimiter can also be changed in config.h)

```
$ tiramisu
{ "app_name": "notify-send", "app_icon": "", "replaces_id": 0, "timeout": -1, "summary": "Notification summary", "body": "Notification body" }
```

also tiramisu can output information in json so it can be easily parsed.

If any specific setting is disabled in the configuration, the line is omitted
from the output (making all of the output one line shorter).

#### Note that only a single process can claim the org.freedesktop.Notifications name at a given time, so any other running notification daemon must be killed before running tiramisu.

