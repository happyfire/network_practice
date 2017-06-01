#!/bin/sh

if [ -f /bin/bash ]
then
	echo "file /bin/bash exists"
fi

if [ -d /bin/bash ]
then
	echo "/bin/bash is a directory"
else
	echo "/bin/bash is NOT a directory"
fi

printf "Is it morning? Please answer yes or no: "
read timeofday

if [ "$timeofday" = "yes" ]; then
	echo "Good morning"
elif [ "$timeofday" = "no" ]; then
	echo "Good afternoon"
else
	echo "Sorry, $timeofday not recognized. Enter yes or no"
	exit 1
fi

exit 0
