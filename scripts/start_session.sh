#!/bin/bash

########################################
# This script is intended to be remotly#
# Executed on the octane client server #
# as the unix user Octane, it takes in #
# the port number of the session that  #
# a new key is desired for and commands#
# the vncserver to start a new sessions#
# and make a new key for the user and  #
# the key is then echoed and stored in #
# A file in the tmp directory          #
########################################

mkdir -p /tmp/load-system

USER_STRING=$(date +'%Y-%m-%d_%H-%M')_$USER
FILE="/tmp/load-system/${USER_STRING}"
VNC_PATH="/opt/TurboVNC/bin"

${VNC_PATH}/vncserver -otp 2>&1 | less > $FILE

OTP=$(grep one-time $FILE | sed s/^.*:\//g)
echo $OTP > ~/password.txt
echo "{ \"otp\":$OTP }"
exit;
