#!/bin/sh

printf "Enter password:"
read trythis

while [ "$trythis" != "secret" ]; do
	printf "Sorry, try again:"
	read trythis
done

echo "done"

exit 0
