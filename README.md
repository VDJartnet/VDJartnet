# VDJartnet

VDJartnet is a plugin for VirtualDJ to allow it to output Art-Net data to control a DMX program such as QLC+.
While it has only been tested on QLC+ it should also work on other DMX software that can take an artnet input.

VDJartnet is configured using a config file which can be found at \<VirtualDJ\>/Plugins64/AutoStart/VDJartnet/config.txt on Mac and \<VirtualDJ\>\\Plugins\\AutoStart\\VDJartnet\\config.txt on Windows

## The config file layout is as described below

The first line is the IP address to which the Art-Net data should be sent.

The subsequent lines are of the format
\#\#\#~\<VDJscript command\>
where \#\#\# is the three digit channel number from 001 to 512 signifying which channel in the Art-Net universe should be controlled and \<VDJscript command\> is a command in VDJscript that will return a value between 0 and 255 which will then be sent over the Art-Net connection.

[The VDJscript reference can be found here](https://www.virtualdj.com/wiki/VDJscript.html "VDJscript Reference")

Additional configuration files may be included by adding a line \@\<file path\> where \<file path\> is the path to the additional configuration file, this additional file must not include a connection ip address, only channel definitions and further include statements.

### Example commands are as follows
Additional
```
001~get_constant 255
//sends a constant value of 255 to artnet channel 1
002~var $artnet 1 ? get_constant 255 : get_constant 0
//sends a value of 255 to artnet channel 2 if the variable $artnet equals 1, otherwise sends a value of 0
```
