#!/bin/bash

max=100
for (( i=0; i <= $max; i+= 10 ))
do
	./main --log $i 150
	./main --pow 2 $i 50
	./main --pow -2 $i 50
done
