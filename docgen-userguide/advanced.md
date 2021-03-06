Edit the config file directly
-----------------------------
The config file can be found at VirtualDJ\/Plugins\[64\]\/AutoStart\/VDJartnet.conf

All of these lines are optional but will be added by the config tool if it is run.

### Command lines
The main body of the config file is made up of command lines which are of the form \#\#\#\~\<VDJscript command\> where \#\#\# is the three digit channel number from 001 to 512 signifying which channel in the DMX universe should be controlled and \<VDJscript command\> is a VDJscript command that will set \$VDJartnetSend to a value between 0 and 255 which will then be sent over the Art-Net connection.

### Config lines
If a line begins with \+\# where # is a character as given below then it is configuring a certain setting.

#### +H - Set destination host
A line of the form \+H \<host\>\:\<port\> will set the destination host for the Art-Net data to \<host\> and send it to the port \<port\>

#### +T - Set maximum number of skipped packets
If all the commands return the same value several times in a row then VDJartnet will refrain from resending the data up to this many times. The default is to skip 20 packets.

#### +C - Set the check rate
This value is the time delay (in milliseconds) between polling VirtualDJ. The default is to wait 10ms.

### Include Statements
Other files can be included in a config file, for example channel definitions could be in a separate file. The files are included using a line beginning with @ symbol followed by either c for config files or p for preset files followed by a space then the path to the file to be included.

### Example Config
```
+H 127.0.0.1:6454
+T 50
+C 20
@c extraconfig
@p presets
001~play ? set $VDJartnetsend 255 : set $VDJartnetsend 0
```

