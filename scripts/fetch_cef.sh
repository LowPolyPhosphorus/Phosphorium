#!/bin/bash
set -e

# Download the correct version directly - get this from the official index
CEF_VERSION="133.4.4+g4feec62+chromium-133.0.6943.142"
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
