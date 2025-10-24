#!/bin/bash

# Check if token is provided
if [ -z "$1" ]; then
    echo "Usage: $0 <github-token>"
    echo "Example: $0 token1234567890abcdef"
    exit 1
fi

TOKEN="$1"

# Install dependencies
sudo apt install -y curl

# Create a folder
rm -rf actions-runner
mkdir actions-runner && cd actions-runner

# Download the latest runner package
curl -o actions-runner-linux-x64-2.329.0.tar.gz -L https://github.com/actions/runner/releases/download/v2.329.0/actions-runner-linux-x64-2.329.0.tar.gz

# Optional: Validate the hash
echo "194f1e1e4bd02f80b7e9633fc546084d8d4e19f3928a324d512ea53430102e1d  actions-runner-linux-x64-2.329.0.tar.gz" | shasum -a 256 -c

# Extract the installer
tar xzf ./actions-runner-linux-x64-2.329.0.tar.gz

# Create the runner and start the configuration experience
./config.sh --url https://github.com/beestarbush/ClockApp --token "$TOKEN"

# Last step, run it!
./run.sh

