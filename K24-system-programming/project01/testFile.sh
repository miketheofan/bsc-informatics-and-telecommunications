#!/bin/bash

# Firstly, we get the data we want from the command line arguments.
virusesFile=$1
countriesFile=$2
numLines=$3
duplicatesAllowed=$4

# Following variables are used to store all the generated informations about any citizen 
# in order to check for duplicates and also produce duplicates(when needed).
PRODUCED_IDS=() 
PRODUCED_FIRST_NAMES=()
PRODUCED_LAST_NAMES=()
PRODUCED_COUNTRIES=()
PRODUCED_AGES=()

ALPHABET_FIRST=(A B C D E F G H I J K L M N O P Q R S T U V W X Y Z)
ALPHABET_REST=(a b c d e f g h i j k l m n o p q r s t u v w x y z)

if(("$#" != 4)); then
	echo $'\n'"Error.There must be exactly 4 arguments."
	exit 4
fi

if [ ! -e "$virusesFile" ]; then
	echo $'\n'"Viruses File ""$virusesFile"" does not exist."
	exit 1
fi

if [ ! -e "$countriesFile" ]; then
	echo $'\n'"Countries File ""$countriesFile"" does not exist."
	exit 2
fi

if (("$numLines" <= 0)); then
	echo $'\n'"Number of lines must be a positive number."
	exit 3
fi

# The following function reads every line from virusesFile given as command line argument
# and stores all data in VIRUSES array.
readViruses(){

	VIRUSES=()

	while read -r line; do
		VIRUSES+=("$line")
	done < "$virusesFile"
	
	virusesNumber=${#VIRUSES[@]}	
}

# The following function reads every line from countriesFile given as command line argument
# and stores all data in COUNTRIES array.
readCountries(){

	COUNTRIES=()

	while read -r line; do
		COUNTRIES+=("$line")
	done < "$countriesFile"
	
	countriesNumber=${#COUNTRIES[@]}
}

# The following function creates a random string used both to generate first and last name of 
# a citizen record. First letter is generated as an upper one, while all the rest are lower case.
createFirstName(){

	loop=$1

	# For every third entry, if flag for duplicates allowed is given, we produce a first name
	# that has already been produced. 
	if [ "$loop" -ne 0 ] && [ $((loop%3)) -eq 0 ] && [ "$duplicatesAllowed" -ne 0 ]; then

		FIRST_NAME=${PRODUCED_FIRST_NAMES[RANDOM_ENTRY]}
		return
	fi

	FIRST_NAME=""
	FIRST_LETTER=$((RANDOM%26))
	FIRST_NAME+=${ALPHABET_FIRST[$FIRST_LETTER]}
	NUMBER_OF_CHARACTERS=$((RANDOM % 11 + 2))

	for (( i = 0; i < NUMBER_OF_CHARACTERS; i++)); do
		
		LETTER=$((RANDOM%26))
		FIRST_NAME+=${ALPHABET_REST[$LETTER]}
	done

	PRODUCED_FIRST_NAMES+=("$FIRST_NAME")
}

createLastName(){

	loop=$1

	# For every third entry, if flag for duplicates allowed is given, we produce a last name
	# that has already been produced. 
	if [ "$loop" -ne 0 ] && [ $((loop%3)) -eq 0 ] && [ "$duplicatesAllowed" -ne 0 ]; then

		LAST_NAME=${PRODUCED_LAST_NAMES[RANDOM_ENTRY]}
		return
	fi

	LAST_NAME=""
	FIRST_LETTER=$((RANDOM%26))
	LAST_NAME+=${ALPHABET_FIRST[$FIRST_LETTER]}
	NUMBER_OF_CHARACTERS=$((RANDOM % 11 + 2))

	for (( i = 0; i < NUMBER_OF_CHARACTERS; i++)); do
		
		LETTER=$((RANDOM%26))
		LAST_NAME+=${ALPHABET_REST[$LETTER]}
	done

	PRODUCED_LAST_NAMES+=("$LAST_NAME")
}

# The following function generates a random number used for age information. The number will be between:
# [1,120].
createAge(){

	loop=$1

	# For every third entry, if flag for duplicates allowed is given, we produce an age
	# that has already been produced. 
	if [ "$loop" -ne 0 ] && [ $((loop%3)) -eq 0 ] && [ "$duplicatesAllowed" -ne 0 ]; then

		AGE=${PRODUCED_AGES[RANDOM_ENTRY]}
		return
	fi

	AGE=$((RANDOM % 120 + 1))
	PRODUCED_AGES+=("$AGE")
}

# The following function creates a date string in the format of: DD-MM-YYYY. 
createDate(){

	DATE=""

	DAY=$((RANDOM%29+1))
	if [[ ${#DAY} -lt 2 ]]; then
		DAY="0${DAY}"
	fi

	MONTH=$((RANDOM%11+1))
	if [[ ${#MONTH} -lt 2 ]]; then
		MONTH="0${MONTH}"
	fi

	YEAR=$((RANDOM%121+1900))

	DATE="$DAY-$MONTH-$YEAR"
}

# The following function creates a random number used as ID between: [1,9999].
# If duplicatesAllowed flag is given as 0, for every number generated, it checks
# if number has been generated before. 
createID(){

	loop=$1

	# For every third entry, if flag for duplicates allowed is given, we produce an id
	# that has already been produced. 
	if [ "$loop" -ne 0 ] && [ $((loop%3)) -eq 0 ] && [ "$duplicatesAllowed" -ne 0 ]; then

		RANDOM_ENTRY=$((RANDOM % ${#PRODUCED_IDS[@]}))	
		ID=${PRODUCED_IDS[$RANDOM_ENTRY]}
		return
	fi

	ID=$((RANDOM%9999))

	if [ "$duplicatesAllowed" -eq 0 ]; then
		checkIfExists $ID

		while [ "$EXISTS" = TRUE ]; do
			
			ID=$((RANDOM%9999))
			checkIfExists $ID
		done
	fi

	PRODUCED_IDS+=("$ID")
}

# The following function creates a string that represents both the "YES" or "NO"
# data and the date. By default there is a 66% chance that the script will produce
# "YES" and 33% to produce "NO". If script produced "NO", there is a 25% chance
# a date will also be produced. That is in order to force main program handle this certain error.
createInfo(){

	INFO=""
	RANDOM_NUMBER=$((RANDOM%3))

	createDate
	
	if [ $RANDOM_NUMBER -eq 0 ]; then
		
		RANDOM_NUMBER=$((RANDOM%4))

		if [ $RANDOM_NUMBER -eq 0 ]; then
			INFO="NO $DATE"
		else
			INFO="NO"
		fi
		
	else
		INFO="YES $DATE"
	fi
}

# The following function creates a fully formated record, using all the above functions.
createRecord(){
	
	createID "$j"
	id=$ID

	createFirstName "$j"
	FIRST_NAME=$FIRST_NAME
	
	createLastName "$j"
	LAST_NAME=$LAST_NAME
	
	# For every third entry, if flag for duplicates allowed is given, we produce a country
	# that has already been produced. 
	if [ "$j" -ne 0 ] && [ $((j%3)) -eq 0 ] && [ "$duplicatesAllowed" -ne 0 ]; then

		COUNTRY=${PRODUCED_COUNTRIES[$RANDOM_ENTRY]}

	else

		COUNTRY=${COUNTRIES[$((RANDOM%countriesNumber))]}
		PRODUCED_COUNTRIES+=("$COUNTRY")
	fi
	
	createAge "$j"
	age=$AGE
	
	VIRUS=${VIRUSES[$((RANDOM%virusesNumber))]}
	PRODUCED_VIRUSES+=("$VIRUS")
	
	createInfo
	info=$INFO

	RECORD="$id $FIRST_NAME $LAST_NAME $COUNTRY $age $VIRUS $info"
}

# The following function creates a records file of numLines total records.
createFile(){

	rm -f "citizenRecordsFile"

	file="citizenRecordsFile.txt"

	for (( j = 0; j < "$numLines"; j++ )); do
		
		createRecord $j
		
		if [ $j -eq 0 ]; then
			echo "$RECORD" > $file
		else
			echo "$RECORD" >> $file
		fi
	done
}

# The following function checks if id exists in PRODUCED_IDS.
checkIfExists(){

	EXISTS=FALSE

	for elem in "${PRODUCED_IDS[@]}"; do
    	if [ "$elem" -eq "$1" ]; then
    		EXISTS=TRUE
    		break
    	fi
    done
}

################## THIS IS THE MAIN PART OF THE SCRIPT FILE #########################
readViruses
readCountries
createFile