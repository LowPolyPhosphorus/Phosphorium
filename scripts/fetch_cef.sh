#!/bin/bash
set -e  # stop immediately if anything fails

CEF_VERSION="128.4.9+gc9ae8d2+chromium-128.0.6613.186"
PLATFORM="linux64"

URL="https://cef-builds.spotifycdn.com/cef_binary_${CEF_VERSION}_${PLATFORM}.tar.bz2"

echo "Downloading from: $URL"
mkdir -p cef

# Download and show progress
curl -L --fail --show-error "$URL" -o cef.tar.bz2

echo "Download complete, extracting..."
tar -xj --strip-components=1 -C cef -f cef.tar.bz2

echo "Cleaning up..."
rm cef.tar.bz2

echo "CEF folder contents:"
ls cef/
