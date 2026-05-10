#!/bin/bash

# Change this one line when a new version comes out
CEF_VERSION="128.4.9+gc9ae8d2+chromium-128.0.6613.186"

# Change this to match your OS: linux64, macosx64, or windows64
PLATFORM="linux64"

URL="https://cef-builds.spotifycdn.com/cef_binary_${CEF_VERSION}_${PLATFORM}.tar.bz2"

echo "Downloading CEF..."
mkdir -p cef
curl -L "$URL" | tar -xj --strip-components=1 -C cef
echo "Done!"
