#!/bin/bash
set -e

# Get the official CEF project tooling
git clone --depth 1 https://github.com/chromiumembedded/cef-project.git cef-project

# Copy its CMake helpers into our project
cp -r cef-project/cmake ./cmake

# Use its download script to fetch CEF binaries
cd cef-project
python3 tools/automate/automate-git.py \
  --download-dir=../cef \
  --no-build \
  --force-clean

echo "Done! CEF contents:"
ls ../cef/
