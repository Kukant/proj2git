#!/bin/bash

max=10000000
for (( i=0; i <= $max; i+= 10000 ))
do
	./main --log $i 150
	./main --pow 2 $i 50
	./main --pow -2 $i 50
done
