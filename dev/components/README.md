# Make a link here

To be able to use the `dev` folder for building and running the Test Program (in folder `dev\main`), it must have a reference to the actual component (aka: library).

First time after download or clone, you must make a link (Windows: Junction) from here (the `dev\components` folder) to the root folder, which is 2 levels above.

## Windows

Using PowerShell (PS prompt), create a `Junction` here:

```code
cd dev\components

New-Item -ItemType Junction -Name neopixel -Target ..\..
```

After this, in VSC the `dev\components` folder seems to contain the whole "neopixel" folder once again.

Example:

```code
PS <YOURPATH>  cd dev\components\
PS <YOURPATH>\dev\components>  New-Item -ItemType Junction -Name neopixel -Target ..\..

    Directory: <YOURPATH>\dev\components

Mode        LastWriteTime   Length   Name                                                                                                               
----        -------------   ------   ----                                                                                                               
d----l   18-10-2026 11:02            neopixel                                                                                                           
```

## Linux

Using shell, create a symbolic link here:

```code
cd dev/components

ln -s ../.. neopixel
```
