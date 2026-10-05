# Why these files

The files here are intended for the `dev/` environment, to build/test a small test program for this Neopixel component (aka: library) locally.

They will NOT be used to build your "real application" that makes use of the component.

`settings.json` points ESP-IDF build artifacts to `dev/build` and `dev/sdkconfig`.

@@@TODO: review settings.json, remove what is NOT needed

The repository root `CMakeLists.txt` supports direct ESP-IDF commands from this workspace while still acting as a reusable component when included elsewhere.
