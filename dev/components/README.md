# Make a link here

To be able to use the `dev` folder for running a test program (in main), it must have a reference to the actual component (aka: library).

First time after download or clone, you must make a link (Windows: Junction) from here (the `dev\components` folder) to the root folder, which is 2 levels above.

## Windows

Using PowerShell, create a Junction here:

    (pwd -> dev\components)

    New-Item -ItemType Junction -Name neopixel -Target ..\..

After this, in VSC the dev\components folder seems to contain the full "neopixel" folder.

## Linux

Using shell, create a symbolic link here:

    (pwd -> dev\components)

    ln -s ../.. neopixel
