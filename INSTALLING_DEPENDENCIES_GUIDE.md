# Installing Dependencies Guide

Here you can find how to install Clevit dependencies to your Operation System.

# Dependencies

You need install this dependencies before install Clevit

* cmake 2.8.11 or a newer version - https://cmake.org/

* make

* Qt5 - https://www.qt.io/

* OpenSSL - https://www.openssl.org/ but the easiest but unofficial way for Windows is this:  https://slproweb.com/download/Win32OpenSSL-1_1_0f.exe

---

## Installing dependencies on Arch Linux, Antergos, Manjaro ...
`pacaur -Syy cmake qt5-base qt5-translations qt5-graphicaleffects xdg-utils qt5-quickcontrols2 qt5-quickcontrols qt5-declarative libevent openssl`

## Installing dependencies on Ubuntu , Linux Mint, Debian ...
`sudo apt-get install build-essential libssl-dev make cmake qtdeclarative5-dev qml-module-qtquick-controls qt5-default openssl`

## Installing dependencies on RedHat, Fedora ...
`sudo dnf install automake gcc gcc-c++ kernel-devel make cmake qt5 qt5-devel qt5-qtbase qt5-qtbase-devel qt5-qtdeclarative qt5-qtdeclarative-devel openssl`

## MacOS & Windows
Check the official web sites
