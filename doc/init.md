Sample init scripts and service configuration for yellowduckiecoind
==========================================================

Sample scripts and configuration files for systemd, Upstart and OpenRC
can be found in the contrib/init folder.

    contrib/init/yellowduckiecoind.service:    systemd service unit configuration
    contrib/init/yellowduckiecoind.openrc:     OpenRC compatible SysV style init script
    contrib/init/yellowduckiecoind.openrcconf: OpenRC conf.d file
    contrib/init/yellowduckiecoind.conf:       Upstart service configuration file
    contrib/init/yellowduckiecoind.init:       CentOS compatible SysV style init script

Service User
---------------------------------

All three Linux startup configurations assume the existence of a "yellowduckiecoin" user
and group.  They must be created before attempting to use these scripts.
The OS X configuration assumes yellowduckiecoind will be set up for the current user.

Configuration
---------------------------------

At a bare minimum, yellowduckiecoind requires that the rpcpassword setting be set
when running as a daemon.  If the configuration file does not exist or this
setting is not set, yellowduckiecoind will shutdown promptly after startup.

This password does not have to be remembered or typed as it is mostly used
as a fixed token that yellowduckiecoind and client programs read from the configuration
file, however it is recommended that a strong and secure password be used
as this password is security critical to securing the wallet should the
wallet be enabled.

If yellowduckiecoind is run with the "-server" flag (set by default), and no rpcpassword is set,
it will use a special cookie file for authentication. The cookie is generated with random
content when the daemon starts, and deleted when it exits. Read access to this file
controls who can access it through RPC.

By default the cookie is stored in the data directory, but it's location can be overridden
with the option '-rpccookiefile'.

This allows for running yellowduckiecoind without having to do any manual configuration.

`conf`, `pid`, and `wallet` accept relative paths which are interpreted as
relative to the data directory. `wallet` *only* supports relative paths.

For an example configuration file that describes the configuration settings,
see `contrib/debian/examples/yellowduckiecoin.conf`.

Paths
---------------------------------

### Linux

All three configurations assume several paths that might need to be adjusted.

Binary:              `/usr/bin/yellowduckiecoind`  
Configuration file:  `/etc/yellowduckiecoin/yellowduckiecoin.conf`  
Data directory:      `/var/lib/yellowduckiecoind`  
PID file:            `/var/run/yellowduckiecoind/yellowduckiecoind.pid` (OpenRC and Upstart) or `/var/lib/yellowduckiecoind/yellowduckiecoind.pid` (systemd)  
Lock file:           `/var/lock/subsys/yellowduckiecoind` (CentOS)  

The configuration file, PID directory (if applicable) and data directory
should all be owned by the yellowduckiecoin user and group.  It is advised for security
reasons to make the configuration file and data directory only readable by the
yellowduckiecoin user and group.  Access to yellowduckiecoin-cli and other yellowduckiecoind rpc clients
can then be controlled by group membership.

NOTE: When using the systemd .service file, the creation of the aforementioned
directories and the setting of their permissions is automatically handled by
systemd. Directories are given a permission of 710, giving the YellowDuckieCoin group
access to files under it _if_ the files themselves give permission to the
YellowDuckieCoin group to do so (e.g. when `-sysperms` is specified). This does not allow
for the listing of files under the directory.

NOTE: It is not currently possible to override `datadir` in
`/etc/yellowduckiecoin/yellowduckiecoin.conf` with the current systemd, OpenRC, and Upstart init
files out-of-the-box. This is because the command line options specified in the
init files take precedence over the configurations in
`/etc/yellowduckiecoin/yellowduckiecoin.conf`. However, some init systems have their own
configuration mechanisms that would allow for overriding the command line
options specified in the init files (e.g. setting `YellowDuckieCoinD_DATADIR` for
OpenRC).

### macOS

Binary:              `/usr/local/bin/yellowduckiecoind`  
Configuration file:  `~/Library/Application Support/YellowDuckieCoin/yellowduckiecoin.conf`  
Data directory:      `~/Library/Application Support/YellowDuckieCoin`  
Lock file:           `~/Library/Application Support/YellowDuckieCoin/.lock`  

Installing Service Configuration
-----------------------------------

### systemd

Installing this .service file consists of just copying it to
/usr/lib/systemd/system directory, followed by the command
`systemctl daemon-reload` in order to update running systemd configuration.

To test, run `systemctl start yellowduckiecoind` and to enable for system startup run
`systemctl enable yellowduckiecoind`

### OpenRC

Rename yellowduckiecoind.openrc to yellowduckiecoind and drop it in /etc/init.d.  Double
check ownership and permissions and make it executable.  Test it with
`/etc/init.d/yellowduckiecoind start` and configure it to run on startup with
`rc-update add yellowduckiecoind`

### Upstart (for Debian/Ubuntu based distributions)

Drop yellowduckiecoind.conf in /etc/init.  Test by running `service yellowduckiecoind start`
it will automatically start on reboot.

NOTE: This script is incompatible with CentOS 5 and Amazon Linux 2014 as they
use old versions of Upstart and do not supply the start-stop-daemon utility.

### CentOS

Copy yellowduckiecoind.init to /etc/init.d/yellowduckiecoind. Test by running `service yellowduckiecoind start`.

Using this script, you can adjust the path and flags to the yellowduckiecoind program by
setting the YellowDuckieCoinD and FLAGS environment variables in the file
/etc/sysconfig/yellowduckiecoind. You can also use the DAEMONOPTS environment variable here.

### Mac OS X

Copy org.yellowduckiecoin.yellowduckiecoind.plist into ~/Library/LaunchAgents. Load the launch agent by
running `launchctl load ~/Library/LaunchAgents/org.yellowduckiecoin.yellowduckiecoind.plist`.

This Launch Agent will cause yellowduckiecoind to start whenever the user logs in.

NOTE: This approach is intended for those wanting to run yellowduckiecoind as the current user.
You will need to modify org.yellowduckiecoin.yellowduckiecoind.plist if you intend to use it as a
Launch Daemon with a dedicated yellowduckiecoin user.

Auto-respawn
-----------------------------------

Auto respawning is currently only configured for Upstart and systemd.
Reasonable defaults have been chosen but YMMV.
