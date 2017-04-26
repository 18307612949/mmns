#!/bin/bash

########################################
# This script is intended to be remotly#
# Executed on the octane client server #
# as the unix user Octane, it takes in #
# the port number of the session that  #
# is desired to be killed and remotly  #
# tells the vnc server to kill the     #
# session, the output is saved to a    #
# file in tmp which could be stored as #
# a log later                          #
########################################

mkdir -p /tmp/load-system

for i in "$@"
do
case $i in
	-p=*|--port=*)
	PORT="${i#*=}"
	shift # past argument=value
	;;
esac
done
USER_STRING=$(date +'%Y-%m-%d_%H-%M')_$USER
FILE="/tmp/load-system/${USER_STRING}"
VNC_PATH="/opt/TurboVNC/bin"

${VNC_PATH}/vncserver -kill :$PORT 2>&1 | tee > $FILE
exit;
