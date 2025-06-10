
# VitaSDK paf-component

A paf component installer for building an environment to develop original applications with the look and functionality of native applications using paf for PlayStation®Vita with VitaSDK.

So what is different from the paf that is already in the VitaSDK?

The paf stuff currently in the VitaSDK is pretty bare bones.

And while most of the original paf is C++, some stuff has been forced into being C code in VitaSDK.

## Installation

Run `./install.sh`

Currently, we only guarantee that the installation will work on Linux/WSL2.

Therefore, the installation process may not work correctly on platforms other than these.

## Updating the paf-component

Currently same as Installation section.

## Missing Features

`paf::math`
- have replaced it with our own implementation because it requires a header that is included in the official SDK.

`paf::map`
- have replaced it with our own implementation because it requires a header that is included in the official SDK.

  although publicly available alternatives such as xtree may be possible.

## Samples

`/paf_samples/vitasdk`
- There files necessary to build with VitaSDK.

`/paf_samples/scesdk`
- There files necessary to build with Official SDK.

`/paf_samples/samples`
- Some main sample code is included.

## QFA

- I have the .xml file for myself, but how do I create the .rco?

  use [psp2cxml-tool](https://github.com/Princess-of-Sleeping/psp2cxml-tool/releases).

## Credits

[GrapheneCt](https://github.com/GrapheneCt/) for Reverse engineering of most of the PAF ([ScePaf-RE](https://github.com/GrapheneCt/ScePaf-RE/)).

[Ibrahim](https://github.com/Ibrahim778) for contribution to paf samples.
