#!/bin/bash

########################################
# This script is intended to generate a#
# New rsa key and use the unix user    #
# Octane to install the key in the new #
# User's home folder. The private key  #
# will then be stored in a db on the   #
# the front end for security.          #
# This script seems weird, but the web #
# Server is suposed to run this script #
# And it will ssh into the machine with#
# Octane running on it and instruct the#
# load balancer to masquerade as said  #
# User in order to install the key for #
# Them manually. That way the user     #
# Password never had to be used to set #
# up their account on the backend just #
# Keys                                 #
########################################

mkdir -p /tmp/load-system
IP="127.0.0.1"


for i in "$@"
do
case $i in
	-u=*|--user=*)
	USERNAME="${i#*=}"
	shift # past argument=value
	;;
	-i=*|--key=*)
	KEY="${i#*=}"
	shift
	;;
	-d=*|--destination=*)
	IP="${i#*=}"
	shift
	;;
esac
done

USER_STRING=$(date +'%Y-%m-%d_%H-%M')_$USERNAME
FILE="/tmp/load-system/${USER_STRING}"

ssh-keygen -t rsa -N "" -f ./$USER_STRING.key -q
ssh -i $KEY octane@$IP "bash -s --" < "${DIR}/scripts/install_ssh_key.sh" -u=$USERNAME -i=./$USER_STRING.key.pub -q

exit;
