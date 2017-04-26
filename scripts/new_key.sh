#!/bin/bash

########################################
# This script is intended to be remotly#
# Executed on the octane client server #
# as the unix user Octane, it takes in #
# the port number of the session that  #
# a new key is desired for and commands#
# the vnc server to generate a new key #
# the key is then echoed and saved in a#
# file in the tmp directory            #
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

${VNC_PATH}/vncpasswd -display :$PORT -o 2>&1 | tee > $FILE

OTP=$(grep one-time $FILE | sed s/^.*:\//g)

echo "{ \"otp\":$OTP }"
