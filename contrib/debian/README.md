
Debian
====================
This directory contains files used to package yellowduckiecoind/yellowduckiecoin-qt
for Debian-based Linux systems. If you compile yellowduckiecoind/yellowduckiecoin-qt yourself, there are some useful files here.

## yellowduckiecoin: URI support ##


yellowduckiecoin-qt.desktop  (Gnome / Open Desktop)
To install:

	sudo desktop-file-install yellowduckiecoin-qt.desktop
	sudo update-desktop-database

If you build yourself, you will either need to modify the paths in
the .desktop file or copy or symlink your yellowduckiecoin-qt binary to `/usr/bin`
and the `../../share/pixmaps/yellowduckiecoin128.png` to `/usr/share/pixmaps`

yellowduckiecoin-qt.protocol (KDE)

