#!/bin/bash

echo "Checking the required tools..."

# === Check for vcpkg ===
if command -v vcpkg > /dev/null 2>&1; then 
    echo "vcpkg is installed."
else
    echo "Installing vcpkg..."

    # Clone the vcpkg repository if it doesn't exist
    if [ ! -d "$HOME/vcpkg" ]; then 
        git clone https://github.com/microsoft/vcpkg.git $HOME/vcpkg
    else
        echo "The vcpkg repository already exists at $HOME/vcpkg"
    fi

    # Navigate to the vcpkg directory
    cd $HOME/vcpkg

    # Run the bootstrap script
    ./bootstrap-vcpkg.sh

    # Add vcpkg to PATH in .bashrc if not already present
    if ! grep -q 'export PATH="$HOME/vcpkg' ~/.bashrc; then
        echo 'export PATH="$HOME/vcpkg:$PATH"' >> ~/.bashrc
        echo "vcpkg PATH added to ~/.bashrc"
    else
        echo "vcpkg PATH already present in ~/.bashrc"
    fi

    # Apply the PATH change for the current session
    export PATH="$HOME/vcpkg:$PATH"
fi

# === Check for cmake ===
if command -v cmake > /dev/null 2>&1; then 
    echo "CMake is installed."
else
    echo "Installing CMake..."

    sudo apt update
    sudo apt install cmake
fi

echo "All requirements are installed."
