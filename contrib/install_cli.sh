 #!/usr/bin/env bash

 # Execute this file to install the yellowduckiecoin cli tools into your path on OS X

 CURRENT_LOC="$( cd "$(dirname "$0")" ; pwd -P )"
 LOCATION=${CURRENT_LOC%YellowDuckieCoin-Qt.app*}

 # Ensure that the directory to symlink to exists
 sudo mkdir -p /usr/local/bin

 # Create symlinks to the cli tools
 sudo ln -s ${LOCATION}/YellowDuckieCoin-Qt.app/Contents/MacOS/yellowduckiecoind /usr/local/bin/yellowduckiecoind
 sudo ln -s ${LOCATION}/YellowDuckieCoin-Qt.app/Contents/MacOS/yellowduckiecoin-cli /usr/local/bin/yellowduckiecoin-cli
