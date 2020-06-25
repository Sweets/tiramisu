# Desktop notifications, the UNIX way

tiramisu is a notification daemon based on dunst that outputs notifications
to STDOUT in order to allow the user to process notifications any way they prefer.

Perhaps you want notifications to be sent to your bar? tiramisu allows you to do that.

Maybe you want notifications to be redirected to your phone as push notifications? A little bit of elbow grease and some tiramisu would let you do that ;)

Notifications could be made more accessibility friendly even with TTS.

<div align="center"><img src="https://github.com/Sweets/tiramisu/blob/master/example.gif"/></div>

Thanks to [BanchouBoo](https://github.com/BanchouBoo) for helping test tiramisu and providing the gif of it.

# Usage

Redirecting output of tiramisu to the input of another program is the ideal methodology to using
tiramisu.

```
tiramisu | your-application
```

tiramisu outputs a new line for every relevant bit of information regarding a notification.
Anywhere a string may be found in a notification a new-line may be placed,
but the most likely scenario is that a new-line is only found in the notification body,
and because this is the most likely scenario, the body is output last in the notification.

The structure for notifications being output by tiramisu is as follows.

```
$ tiramisu
Application name
Application icon path (if enabled in configuration)
Notification actions (if enabled in configuration; not currently available)
Notification hints (if enabled in configuration; not currently available)
Replacement ID (if enabled in configuration)
Notification timeout
Summary
Body
```

also tiramisu can output information in json so it can be easily parsed.

If any specific setting is disabled in the configuration, the line is omitted
from the output (making all of the output one line shorter).

##### Note that only a single process can claim the org.freedesktop.Notifications name at a given time, so any other running notification daemon must be killed before running tiramisu.

# Configuration

By default tiramisu opts out of outputting notification actions, replacement IDs,
notification timeouts, and hints. These can be enabled by modifying `config.h`
and uncommenting the needed functionality and recompiling tiramisu.

Arrays are same-line, comma-separated. Associative arrays (dictionaries) are the same,
except their keys prefix the values and are separated by a colon.
