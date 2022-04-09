# Soaria plugins for BlueCat's Plug'n Script

Open source script repo for Soaria's BCPS scripts.

Everything in here is licenced under the Apache 2.0 licence. This means (non-binding simple English version) that you can more or less do what you want with it as long as you keep credits and don't try to sue over patents.

## ChannelSwitcher

* articulator.*

This little piece lets you set a keyswitch per MIDI channel, and notes that come in on the specified MIDI channel will get articulated via inserted keyswitch notes. This is useful for DAWs that don't support articulations natively, like Bitwig. (Sadly Live doesn't work for this since it doesn't support MIDI channels at all.)

You will need to execute the Nunjucks templater under "templater" if you want to mess with the kuiml in a meaningful way.

You'll also need to copy over the "library" tree from the Plug'N Script VST in order to get this to compile. I'm uncertain what the licence situation is there, so I'm not distributing them.
