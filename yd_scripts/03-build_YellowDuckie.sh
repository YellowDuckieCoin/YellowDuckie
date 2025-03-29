#!/bin/bash

YellowDuckieROOT=$(pwd)
OS=$1

# find ./src -name '*.o' -type f -delete
# find ./src -name '*.a' -type f -delete
# find ./src -name '*.so' -type f -delete
# find ./src -name '*.dylib' -type f -delete

# make clean


make -sj16

case "$OS" in
    "linux")
        mkdir -p $YellowDuckieROOT/release/linux
        mv $YellowDuckieROOT/src/yellowduckiecoin-cli $YellowDuckieROOT/release/linux
        mv $YellowDuckieROOT/src/yellowduckiecoind $YellowDuckieROOT/release/linux
        mv $YellowDuckieROOT/src/qt/yellowduckiecoin-qt $YellowDuckieROOT/release/linux
        # strip ./release/linux/*
        ;;
    "windows")
        mkdir -p $YellowDuckieROOT/release/win
        mv $YellowDuckieROOT/src/*.exe $YellowDuckieROOT/release/win
        mv $YellowDuckieROOT/src/qt/*.exe $YellowDuckieROOT/release/win
        # strip --strip-unneeded $YellowDuckieROOT/release/win/*
        ;;
    "arm")
        mkdir -p $YellowDuckieROOT/release/arm
        mv ./src/YellowDuckie-cli ./release/arm
        mv ./src/YellowDuckied ./release/arm
        mv ./src/qt/YellowDuckie-qt ./release/arm
        # strip ./release/arm/*
        ;;
    "osx")
        mkdir -p $YellowDuckieROOT/release/osx
        mv ./src/YellowDuckie-cli ./release/osx
        mv ./src/YellowDuckied ./release/osx
        mv ./src/qt/YellowDuckie-qt ./release/osx
        # strip ./release/osx/*
        ;;
    *)
        echo "Unsupported OS type: $OS"
        echo "Supported OS types: linux, windows, arm, osx"
        exit 1
        ;;
esac

echo "Build and strip completed for $OS"
