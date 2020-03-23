# rotations

[![sampctl](https://shields.southcla.ws/badge/sampctl-rotations-2f2f2f.svg?style=for-the-badge)](https://github.com/Shiska/rotations)

<!-- This include gives you the possibility to convert rotations -->

# Installation

```bash
sampctl package install Shiska/rotations
```

# Usage

For all basic functions use: `#include <rotation>`  
For additional functions use: `#include <rotation_misc>`  
And if you want the easy way use: `#include <rotation_extra>`  
It applies the basic functions for players, objects and vehicles.  
Note: Vehicle related function only work if occupied due to [GetVehicleRotationQuat](https://wiki.sa-mp.com/wiki/GetVehicleRotationQuat).  
Note: If you want to use the DynamicObject functions you need to include `rotation_extra` after `streamer`

Check the reference for further information.

# Testing

```bash
sampctl package ensure
sampctl package build
sampctl package run
```

# Reference

* [rotation.inc](https://shiska.github.io/rotations/2.0.2/rotation.xml#index)
* [rotation_misc.inc](https://shiska.github.io/rotations/2.0.2/rotation_misc.xml#index)
* [rotation_extra.inc](https://shiska.github.io/rotations/2.0.2/rotation_extra.xml#index)
