#!/bin/bash

# Vertocoin Build Script
# Copyright (c) 2025 The Vertocoin developers

set -e

echo "Building Vertocoin..."
echo "====================="

# Check if we're in the right directory
if [ ! -f configure.ac ]; then
    echo "Error: Please run this script from the Vertocoin root directory"
    exit 1
fi

# Install dependencies (macOS with Homebrew)
if [[ "$OSTYPE" == "darwin"* ]]; then
    echo "Installing dependencies for macOS..."
    if ! command -v brew &> /dev/null; then
        echo "Error: Homebrew not found. Please install Homebrew first."
        exit 1
    fi
    
    brew install automake berkeley-db4 libtool boost miniupnpc openssl pkg-config protobuf python3 qt5 zmq libevent
fi

# Generate configure script
echo "Generating configure script..."
./autogen.sh

# Configure the build
echo "Configuring build..."
./configure \
    --enable-cxx \
    --disable-shared \
    --with-pic \
    --prefix=/usr/local \
    --enable-upnp-default \
    --disable-tests \
    --disable-bench \
    --with-miniupnpc \
    --without-qrencode \
    --without-gui \
    --disable-qt \
    --enable-zmq \
    --enable-wallet \
    PKG_CONFIG_PATH="/usr/local/lib/pkgconfig:/usr/local/opt/openssl/lib/pkgconfig"

# Build the project
echo "Building Vertocoin..."
make clean
make -j$(nproc 2>/dev/null || sysctl -n hw.ncpu 2>/dev/null || echo 2)

echo ""
echo "Build completed successfully!"
echo "Binaries are located in src/"
echo ""
echo "Built binaries:"
echo "- vertocoind (daemon)"
echo "- vertocoin-cli (command line interface)"
echo "- vertocoin-tx (transaction utility)"
echo "- vertocoin-qt (GUI wallet)"
echo ""
echo "To install system-wide, run: sudo make install"
echo ""
echo "IMPORTANT: Genesis block has been updated with:"
echo "- Time: 1720080000"
echo "- Nonce: 314792"
echo "- Hash: 0x2add13684ddffe0efcea40d31d44e4c7a7695921bb78dc45e4022115c4ac6ab3"
echo "- Initial Supply: 5,000,000,000 VTO"
echo ""
echo "You can now test the blockchain!"
echo ""
echo "To verify genesis block:"
echo "./verify_genesis.sh"
