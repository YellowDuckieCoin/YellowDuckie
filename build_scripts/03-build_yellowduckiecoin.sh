#!/bin/bash

YellowDuckieCoinROOT=$(pwd)
OS=$1

find ./src -name '*.o' -type f -delete
find ./src -name '*.a' -type f -delete
find ./src -name '*.so' -type f -delete
find ./src -name '*.dylib' -type f -delete

make clean


make -j8

case "$OS" in
    "linux")
        mkdir -p $YellowDuckieCoinROOT/bin/linux
        mv ./src/yellowduckiecoin-cli ./bin/linux
        mv ./src/yellowduckiecoind ./bin/linux
        mv ./src/qt/yellowduckiecoin-qt ./bin/linux
        strip ./bin/linux/*
        ;;
    "windows")
        mkdir -p $YellowDuckieCoinROOT/bin/win
        mv $YellowDuckieCoinROOT/src/*.exe $YellowDuckieCoinROOT/bin/win
        mv $YellowDuckieCoinROOT/src/qt/*.exe $YellowDuckieCoinROOT/bin/win
        strip --strip-unneeded $YellowDuckieCoinROOT/bin/win/*
        ;;
    "arm")
        mkdir -p $YellowDuckieCoinROOT/bin/arm
        mv ./src/yellowduckiecoin-cli ./bin/arm
        mv ./src/yellowduckiecoind ./bin/arm
        mv ./src/qt/yellowduckiecoin-qt ./bin/arm
        strip ./bin/arm/*
        ;;
    "osx")
        mkdir -p $YellowDuckieCoinROOT/bin/osx
        mv ./src/yellowduckiecoin-cli ./bin/osx
        mv ./src/yellowduckiecoind ./bin/osx
        mv ./src/qt/yellowduckiecoin-qt ./bin/osx
        strip ./bin/osx/*
        ;;
    *)
        echo "Unsupported OS type: $OS"
        echo "Supported OS types: linux, windows, arm, osx"
        exit 1
        ;;
esac

echo "Build and strip completed for $OS"
