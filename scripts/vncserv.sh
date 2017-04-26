#!/bin/bash

########################################
# This script is intended to be remotly#
# Executed on the octane server        #
# as the unix user Octane, it takes in #
# the port number of the session that  #
# the name of the user the rsa key to  #
# be used the destination ip of the    #
# machine to be logged into if not self#
# and the kill status and executes them#
# accordingly; if name, key, port, and #
# kill are present then the server will#
# kill the session at that port, but if#
# the name and key and port are present#
# then a new OTP will be generated for #
# the server, if the name and key are  #
# the only things present, then a new  #
# session is started accordingly       #
########################################

DIR="/home/sspitler/Documents/software/nova-octane"
IP="127.0.0.1"

for i in "$@"
do
case $i in
	-n=*|--name=*)
	NAME="${i#*=}"
	shift # past argument=value
	;;
	
	-i=*|--ssh_key=*)
	KEY="${i#*=}"
	shift # past argument=value
	;;

	-p=*|--port=*)
	PORT="${i#*=}"
	shift # past argument=value
	;;
	
	-k=*|--kill=*)
	KILL="${i#*=}"
	shift # past argument=value
	;;
	
	-d=*|--destination=*)
	IP="${i#*=}"
	shift
	;;

	*)
	printf "UNKNOWN ARGUMENT";
	EXIT='1'
            # unknown option
	;;
esac
done

if ! [ -z "$NAME" ] && ! [ -z "$KEY" ]; then
	echo "valid";
	if ! [ -z "$NAME" ] && ! [ -z "$KEY" ] && ! [ -z "$PORT" ] && ! [ -z "$KILL" ]; then
		ssh -i $KEY $NAME@$IP "bash -s --" < "${DIR}/scripts/kill_session.sh" -p=$PORT
 	elif ! [ -z "$NAME" ] && ! [ -z "$KEY" ] && ! [ -z "$PORT" ]; then
		ssh -i $KEY $NAME@$IP "bash -s --" < "${DIR}/scripts/new_key.sh" -p=$PORT
	elif ! [ -z "$NAME" ] && ! [ -z "$KEY" ]; then
		ssh -i $KEY $NAME@$IP "bash -s --" < "${DIR}/scripts/start_session.sh"
	else
		echo "Invalid Flag Combination"
		exit;
	fi
else
	echo "Name or Key field blank"
	exit;
fi
