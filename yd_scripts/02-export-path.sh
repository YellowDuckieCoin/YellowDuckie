OS=${1}
YellowDuckieROOT=$(pwd)



if [[ ! ${OS} || ! ${YellowDuckieROOT} ]]; then
    echo "Error: Invalid options"
    echo "Usage: ${0} <operating system> <github workspace path>"
    exit 1
fi

if [[ ${OS} == "windows" ]]; then
    export PATH=${YellowDuckieROOT}/depends/x86_64-w64-mingw32/native/bin:${PATH}
elif [[ ${OS} == "osx" ]]; then
    export PATH=${YellowDuckieROOT}/depends/x86_64-apple-darwin14/native/bin:${PATH}
elif [[ ${OS} == "linux" || ${OS} == "linux-disable-wallet" ]]; then
    export PATH=${YellowDuckieROOT}/depends/x86_64-linux-gnu/native/bin:${PATH}
elif [[ ${OS} == "arm32v7" || ${OS} == "arm32v7-disable-wallet" ]]; then
    export PATH=${YellowDuckieROOT}/depends/arm-linux-gnueabihf/native/bin:${PATH}
elif [[ ${OS} == "aarch64" || ${OS} == "aarch64-disable-wallet" ]]; then
    export PATH=${YellowDuckieROOT}/depends/aarch64-linux-gnu/native/bin:${PATH}
else
    echo "You must pass an OS."
    echo "Usage: ${0} <operating system> <github workspace path>"
    exit 1
fi


if [[ ${OS} == "windows" ]]; then
    CONFIG_SITE=${YellowDuckieROOT}/depends/x86_64-w64-mingw32/share/config.site ./configure --prefix=/ --disable-ccache --disable-maintainer-mode --disable-dependency-tracking --enable-reduce-exports --disable-bench --disable-tests --disable-gui-tests --with-qtdbus=no --enable-shared=no --with-incompatible-bdb CFLAGS="-O2 " CXXFLAGS="-O2 "
elif [[ ${OS} == "osx" ]]; then
    CONFIG_SITE=${YellowDuckieROOT}/depends/x86_64-apple-darwin14/share/config.site ./configure --prefix=/ --disable-ccache --disable-maintainer-mode --disable-dependency-tracking --enable-reduce-exports --disable-bench --disable-tests --with-qtdbus=no --disable-gui-tests GENISOIMAGE=${YellowDuckieROOT}/depends/x86_64-apple-darwin14/native/bin/genisoimage
elif [[ ${OS} == "linux" || ${OS} == "linux-disable-wallet" ]]; then
    if [[ ${OS} == "linux-disable-wallet" ]]; then
        EXTRA_OPTS="--disable-wallet"
    fi
    CONFIG_SITE=${YellowDuckieROOT}/depends/x86_64-linux-gnu/share/config.site ./configure --prefix=/ --disable-ccache --disable-maintainer-mode --disable-dependency-tracking --enable-glibc-back-compat --enable-reduce-exports  --disable-tests --with-qtdbus=no  --disable-bench --with-qtdbus=no --disable-gui-tests --with-incompatible-bdb CFLAGS="-O2 " CXXFLAGS="-O2 " LDFLAGS="-static-libstdc++" ${EXTRA_OPTS}
elif [[ ${OS} == "arm32v7" || ${OS} == "arm32v7-disable-wallet" ]]; then
    if [[ ${OS} == "arm32v7-disable-wallet" ]]; then
        EXTRA_OPTS="--disable-wallet"
        CONFIG_SITE=${YellowDuckieROOT}/depends/arm-linux-gnueabihf/share/config.site ./configure --prefix=/ --enable-glibc-back-compat --enable-reduce-exports LDFLAGS=-static-libstdc++ --disable-tests --with-libs=no --with-gui=no ${EXTRA_OPTS}
    else
        CONFIG_SITE=${YellowDuckieROOT}/depends/arm-linux-gnueabihf/share/config.site ./configure --prefix=/ --disable-ccache --disable-maintainer-mode --disable-dependency-tracking --enable-glibc-back-compat --enable-reduce-exports --disable-bench --with-qtdbus=no --disable-gui-tests CFLAGS="-O2 " CXXFLAGS="-O2 " LDFLAGS="-static-libstdc++"
    fi
elif [[ ${OS} == "aarch64" || ${OS} == "aarch64-disable-wallet" ]]; then
    if [[ ${OS} == "aarch64-disable-wallet" ]]; then
        EXTRA_OPTS="--disable-wallet"
        CONFIG_SITE=${YellowDuckieROOT}/depends/aarch64-linux-gnu/share/config.site ./configure --prefix=/ --enable-glibc-back-compat --enable-reduce-exports LDFLAGS=-static-libstdc++ --disable-tests --with-libs=no --with-gui=no ${EXTRA_OPTS}
    else
        CONFIG_SITE=${YellowDuckieROOT}/depends/aarch64-linux-gnu/share/config.site ./configure --prefix=/ --disable-ccache --disable-maintainer-mode --disable-dependency-tracking --enable-glibc-back-compat --enable-reduce-exports --disable-bench --with-qtdbus=no --disable-gui-tests CFLAGS="-O2 " CXXFLAGS="-O2 " LDFLAGS="-static-libstdc++"
    fi
else
    echo "You must pass an OS."
    echo "Usage: ${0} <operating system> <github workspace path> <disable wallet (true | false)>"
    exit 1
fi

#  CONFIG_SITE=${YellowDuckieROOT}/depends/x86_64-linux-gnu/share/config.site ./configure --prefix=/ --disable-ccache --disable-maintainer-mode --disable-dependency-tracking --enable-glibc-back-compat --enable-reduce-exports --disable-bench --disable-gui-tests --disable-tests  CFLAGS="-O2 " CXXFLAGS="-O2 " LDFLAGS="-static-libstdc++"