Prerequisites
-------------
Before installation you should have [VirtualDJ](https://virtualdj.com) installed. You will also need to install a lighting program that can receive Art-Net signals. [QLC+](http://qlcplus.org) is recommended.

Installation
------------
You should download a copy of the installer for your operating system from our [downloads page](https://www.kepler22b.uk/vdjartnet/download.html). Once downloaded you just need to run the installer, this will install the plugin into the correct place. Then you just need to restart VirtualDJ if it was already running to load the plugin.

Configuration
-------------
VDJartnet sends a single DMX universe (512 channels) over Art-Net where the value of each channel is determined by sending a VDJscript command to VirtualDJ.

VDJartnet is configured by setting the VDJscript command for each channel.

To open the VDJartnet config tool select the Master tab in the centre then in the Master Effect dropdown select AutoStart" and then click the settings icon next to VDJartnet. Then click the Config button in the VDJartnet panel.

At the top of the main window are fields where you can set the destination host and port, for a simple setup the defaults of 127.0.0.1 and 6454 are correct.

Then for simple setups drag one of the presets in the list of presets (in the second, smaller window) into a row in the list of commands in the main window. Repeat this for every signal you want to send to your lighting software.

If the presets don't contain a signal that you need then you can write a command yourself or start with a preset and edit it. The command should be VDJscript that sets the variable $VDJartnetSend to an integer from 0 to 255 (inclusive). To get help on writing VDJscript commands you can check the reference [on the VirtualDJ Wiki](https://www.virtualdj.com/wiki/VDJ8script.html).
