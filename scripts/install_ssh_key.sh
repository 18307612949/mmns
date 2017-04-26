#!/bin/bash

########################################
# This script is intended to be remotly#
# Executed on the octane server as the #
# Unix user Octane, it takes in the    #
# Desired username for the key         #
# Destination and the key itself and   #
# Manually installs the key into the   #
# Home folder of the user. That way    #
# The password of the user never needed#
# To be used after they were created   #
########################################

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
esac
done

cat $KEY >> /master-mirror/home/users/$USERNAME/.ssh/authorized_keys

exit;
