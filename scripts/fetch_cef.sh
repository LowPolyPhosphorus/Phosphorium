#!/bin/bash
set -e

CEF_VERSION="144.0.6+g5f7e671+chromium-144.0.7559.59"
PLATFORM="linux64"
FILENAME="cef_binary_${CEF_VERSION}_${PLATFORM}_minimal.tar.bz2"
URL="https://cef-builds.spotifycdn.com/${FILENAME}"

echo "Downloading CEF $CEF_VERSION..."
curl -L --fail --show-error "$URL" -o cef.tar.bz2

echo "Extracting..."
mkdir -p cef
tar -xj --strip-components=1 -C cef -f cef.tar.bz2
rm cef.tar.bz2

echo "Done!"
ls cef/
