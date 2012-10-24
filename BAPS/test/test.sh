#!/usr/bin/bash

# Script to automatically check the behavior of the trivial partioner
# Started on 15/09/2008 by Melvin Zhang for CS5206

for i in 001 002 003 004 005 101 102 103 104 105; do
	bin/BAPS proj-d$i-01.prj > /dev/null
	# Do a diff from generated solution to expected solution
  # The expected solution has some non unique lines removed, hence
  # the generated solution in solution/ should be a superset of the expected solution.
  DIFF=`diff solution/part-d$i-tv.sol test/part-d$i-tv.sol | grep \>`
	if [ -z "$DIFF" ]; then 
    echo proj-d$i-01.prj OK; 
  else
    echo FAIL! Diff between solution/part-d$i-tv.sol and test/part-d$i-tv.sol
	  diff solution/part-d$i-tv.sol test/part-d$i-tv.sol;
  fi
done
