#!/bin/bash
set -e

echo "Fetching CEF via official cef-project tooling..."
python3 -m pip install requests
python3 cmake/cef_version.py
