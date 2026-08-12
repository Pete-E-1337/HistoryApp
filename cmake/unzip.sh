#!/bin/bash
if [[ $# > 2 ]]; then
	echo "Usage: [file to unzip (without .zip)] ([dont print stamp])"
	exit
fi

dir=$1
file=${dir}.zip

if [[ !(-f $file) ]]; then
	echo "Zip file $file does not exist"
	exit
fi
unzip -qq $file -d `dirname $file`
