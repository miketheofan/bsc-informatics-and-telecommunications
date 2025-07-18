#!/bin/bash

inputFile=$1
input_dir=$2
numFilesPerDirectory=$3

# First we initialize arrays that we will use later on.
ENTRIES=()
COUNTRIES=()
ROUND_ROBIN=()

# Then we check if arguments are given correctly.
if(("$#" != 3)); then
	echo $'\n'"Error.There must be exactly 3 arguments."
	exit 3
fi

if [ ! -e "$inputFile" ]; then
	echo $'\n'"Input File ""$inputFile"" does not exist."
	exit 2
fi

if (("$numFilesPerDirectory" <= 0)); then
	echo $'\n'"Number of files per directory must be a positive number."
	exit 1
fi

# The following function is used in order to put every record in the correct file of the correct directory
# according to his country.
createDirectories(){

	word=4
	false="FALSE"

	while read -r line; do

		ENTRIES+=("$line")
		
		# Here we get the country of the record.
		tempCountry=$(echo "$line" | awk -v N=$word '{print $4}')

		checkIfCountryExists "$tempCountry"
		if [ "$exists" = "$false" ]; then

			COUNTRIES+=("$tempCountry")
			ROUND_ROBIN[$index]=1
		fi

		# Here we check if directory already exists for this certain country.
		cd "$input_dir" || exit
		if [ ! -d "$tempCountry" ]; then
			mkdir "$tempCountry"
		fi

		cd "$tempCountry" || exit

		getIndex "$tempCountry"
		fileName="$tempCountry-""$tempIndex"".txt"
		
		# And here we insert record in the correct .txt file.
		echo "$line" >> "$fileName"

		cd ../..

	done < "$inputFile"	
}

# This function is used to get in which .txt file a record must be inserted, according to round robin.
getIndex(){

	tempIndex="${ROUND_ROBIN[$index]}"

	if [ "${ROUND_ROBIN[$index]}" -lt "$numFilesPerDirectory" ]; then
		ROUND_ROBIN[$index]=$((${ROUND_ROBIN[$index]}+1))
	else
		ROUND_ROBIN[$index]=$((1))
	fi
}

# This function is used in order to check whether a country has already been inserted in COUNTRIES array.
checkIfCountryExists(){

	exists=FALSE
	index=0

	for elem in "${COUNTRIES[@]}"; do
    	if [ "$elem" = "$1" ]; then
    		exists=TRUE
    		break
    	fi
    	index=$(($index+1))
    done
}

# MAIN FUNCTIONALITY
main(){

	# If directory exists, then delete it.
	if [ -d "$input_dir" ]; then
	 	rm -rf "$input_dir"
	fi

	mkdir "$input_dir"

	createDirectories
}

main