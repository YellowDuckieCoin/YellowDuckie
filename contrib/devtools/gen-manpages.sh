#!/bin/sh

TOPDIR=${TOPDIR:-$(git rev-parse --show-toplevel)}
SRCDIR=${SRCDIR:-$TOPDIR/src}
MANDIR=${MANDIR:-$TOPDIR/doc/man}

YellowDuckieCoinD=${YellowDuckieCoinD:-$SRCDIR/yellowduckiecoind}
YellowDuckieCoinCLI=${YellowDuckieCoinCLI:-$SRCDIR/yellowduckiecoin-cli}
YellowDuckieCoinTX=${YellowDuckieCoinTX:-$SRCDIR/yellowduckiecoin-tx}
YellowDuckieCoinQT=${YellowDuckieCoinQT:-$SRCDIR/qt/yellowduckiecoin-qt}

[ ! -x $YellowDuckieCoinD ] && echo "$YellowDuckieCoinD not found or not executable." && exit 1

# The autodetected version git tag can screw up manpage output a little bit
YDVER=($($YellowDuckieCoinCLI --version | head -n1 | awk -F'[ -]' '{ print $6, $7 }'))

# Create a footer file with copyright content.
# This gets autodetected fine for yellowduckiecoind if --version-string is not set,
# but has different outcomes for yellowduckiecoin-qt and yellowduckiecoin-cli.
echo "[COPYRIGHT]" > footer.h2m
$YellowDuckieCoinD --version | sed -n '1!p' >> footer.h2m

for cmd in $YellowDuckieCoinD $YellowDuckieCoinCLI $YellowDuckieCoinTX $YellowDuckieCoinQT; do
  cmdname="${cmd##*/}"
  help2man -N --version-string=${YDVER[0]} --include=footer.h2m -o ${MANDIR}/${cmdname}.1 ${cmd}
  sed -i "s/\\\-${YDVER[1]}//g" ${MANDIR}/${cmdname}.1
done

rm -f footer.h2m
