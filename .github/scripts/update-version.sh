
#!/bin/bash
# shellcheck disable=SC2002

# fail the script if any command unexpectedly fails
set -e

if [ ! $# -eq 3 ]; then
    echo "Bad number of arguments: $#" >&2
    echo "usage: $0 <major> <minor> <patch>" >&2
    exit 1
fi

re='^[0-9]+$'
if [[ ! $1 =~ $re ]] || [[ ! $2 =~ $re ]] || [[ ! $3 =~ $re ]] ; then
    echo "error: Not a valid version: $1.$2.$3" >&2
    echo "usage: $0 <major> <minor> <patch>" >&2
    exit 1
fi

NEOPIXEL_VERSION_MAJOR="$1"
NEOPIXEL_VERSION_MINOR="$2"
NEOPIXEL_VERSION_PATCH="$3"
NEOPIXEL_VERSION="$NEOPIXEL_VERSION_MAJOR.$NEOPIXEL_VERSION_MINOR.$NEOPIXEL_VERSION_PATCH"

echo "New Neopixel Driver version='$NEOPIXEL_VERSION'"

echo "Updating 'idf_component.yml' manifest for ESP Registry..."
cat idf_component.yml | sed "s/version:.*/version: $NEOPIXEL_VERSION/g" > __idf_component.yml && mv __idf_component.yml idf_component.yml

echo "Updating 'library.json' manifest for PlatformIO Registry..."
cat library.json | sed "s/^  \"version\":.*/  \"version\": \"$NEOPIXEL_VERSION\",/g" > __library.json && mv __library.json library.json

echo "Updating 'include/neopixel_version.h' for driver itself..."
cat include/neopixel_version.h | \
sed "s/#define NEOPIXEL_VERSION_MAJOR.*/#define NEOPIXEL_VERSION_MAJOR $NEOPIXEL_VERSION_MAJOR/g" | \
sed "s/#define NEOPIXEL_VERSION_MINOR.*/#define NEOPIXEL_VERSION_MINOR $NEOPIXEL_VERSION_MINOR/g" | \
sed "s/#define NEOPIXEL_VERSION_PATCH.*/#define NEOPIXEL_VERSION_PATCH $NEOPIXEL_VERSION_PATCH/g" > include/__neopixel_version.h && mv include/__neopixel_version.h include/neopixel_version.h

exit 0