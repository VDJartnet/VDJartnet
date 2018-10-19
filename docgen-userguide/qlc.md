For those that wish to customise beyond simply triggering QLC+ Virtual Console Widgets on the same machine please see the the [QLC+ Documentation](http://www.qlcplus.org/docs/index.html) in particular the section on [The Art-Net plugin](http://www.qlcplus.org/docs/artnetplugin.html)

---

Otherwise you simply want to enable, under the `Inputs/Outputs` tab in QLC+, the plugin labelled `ArtNet 127.0.0.1` in the input channel with `Universe 1` selected (if you have a hardware controller or other input you may need to use another universe, if so, remember this later)

---

To connect a Virtual Console Widget to an ArtNet channel simply right click and select `Widget Properties`. Then, in the `External Input` section, press the `Choose...` button. Now click the triangle next to the universe from earlier (labeled `ArtNet/127.0.0.1`) and double click the text `<Double click here to enter channel number manually>` and enter the channel number from 1 to 512 inclusive.
