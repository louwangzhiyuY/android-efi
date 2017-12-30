# android-efi

android-efi is a simple EFI bootloader for Android™ boot images.
It accepts the partition GUID of an Android boot partition on the command line,
loads the kernel, ramdisk and command line and finally hands over control to the
kernel.

## License
```
android-efi
Copyright (C) 2017 lambdadroid

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <https://www.gnu.org/licenses/>.
```

**Note:** The Android robot contained in `splash.png` is reproduced or modified from work created and 
shared by Google and used according to terms described in the 
[Creative Commons 3.0 Attribution License](https://creativecommons.org/licenses/by/3.0/).
