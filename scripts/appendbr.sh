#!/bin/bash

FILE=$1
if ! [ -f $FILE ]; then
	echo "File not exist: $FILE"
	exit 1
fi

#sed -r 's/(.*)\r/\1\<br\>/g' $FILE
sed -r 's/(.*)/\1\<br\>/g' $FILE

