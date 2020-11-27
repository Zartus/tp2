#!/bin/bash

ko='\e[00;31m';
wipe='\e[00m';
ok='\e[01;32m';

COMMAND="$1"
BASE=.
TESTFILES=../Test/test_files
RET=

function test_format {
    if [ -x $BASE/$COMMAND ]
    then
    rm -f $TESTFILES/result_construct_$1.txt
#    echo "Running " $BASE/$COMMAND -c $1
	$BASE/$COMMAND -c $1 > $TESTFILES/result_construct_$1.txt
	DIFF=`diff -b -E $TESTFILES/result_construct_$1.txt $TESTFILES/references/result_construct_$1.txt`
	if [ $? -eq 0 ]
	then
		RET=0
	else
#		echo "Erreur  : " $DIFF
		RET=1
	fi
	rm -f $TESTFILES/result_construct_$1.txt
    else
	echo "Command $BASE/$COMMAND not found"
	RET=2
    fi
}