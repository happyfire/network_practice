#!/bin/sh

# this  file looks through all the files in the current directory for the string POSIX,
#and then prints the names of those files to the standard output.

for file in *
do
	if grep -q POSIX $file
	then
		echo $file
	fi
done

for foo in bar fud 43
do
	echo $foo
done

for file in $(ls f*.sh); do
	echo $file
done


exit 0
