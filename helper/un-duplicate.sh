#!/bin/bash

# This tool scans a given text-file and ensures,
# that words are unique per line. In case there are
# duplicates, numbers are appended.
# WARNING: Make sure, input file has newline char at the end,
# otherwise the last line will be ignored.

if [ "$#" -ne 2 ]; then
    echo "Usage: ./un-duplicate.sh <input-file> <output-file>"
    exit 1;
fi

input_file="$1"
output_file="$2"
echo "Un-duplicate file "$input_file" into "$output_file

echo -n "" > $output_file;
while read line; do
	i=0;
	words=( $line )
	for word in "${words[@]}"
	do
		j=0;
		matches=0;
		words_check=( $line )
		#run from line begin until < i, if word present
		for word_check in "${words_check[@]}"
		do
			if [[ $word_check == $word ]] 
			then
				((matches++));
			fi
			if [ "$j" -lt "$i" ]
			then
				((j++));
			else
				break;
			fi
		done
		#if more than 1 match, append i to current word
		if [[ $matches > 1 ]]
		then
			echo -n $word$i >> $output_file;
		else
			echo -n $word >> $output_file;
		fi
		((i++));
		if [[ $i -lt ${#words[@]} ]]
		then
			echo -n " " >> $output_file;
		fi		
	done
	echo "" >> $output_file;
done < $input_file
echo "Finished."
