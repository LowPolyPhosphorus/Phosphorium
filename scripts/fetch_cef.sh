#!/bin/bash
set -e

CEF_VERSION="137.0.10+g7e14fe1+chromium-137.0.7151.69"
PLATFORM="linux64"
FILENAME="cef_binary_${CEF_VERSION}_${PLATFORM}_minimal.tar.bz2"
URL="https://cef-builds.spotifycdn.com/${FILENAME}"

echo "Downloading CEF from: $URL"
curl -L --fail --show-error "$URL" -o cef.tar.bz2

echo "Extracting..."
mkdir -p cef
tar -xj --strip-components=1 -C cef -f cef.tar.bz2
rm cef.tar.bz2

echo "Done! CEF contents:"
ls cef/
