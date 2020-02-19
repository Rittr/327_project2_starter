/*
 * functionstocomplete.cpp
 *
 *  Created on: Feb. 18 2020
 *      Author: Chris Ritter
 */

//============================================================================
//	necessary includes
//============================================================================
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include "constants.h"
#include "utilities.h"

//============================================================================
//	stuff you will need
//============================================================================
//a structure to track words and number of times they occur

//std::string wordArray[10];
//int countArray[10];
struct entry {
	std::string word;
	std::string upper;
	int number_occurences;
};

//global array of entry structs (global to this file)

entry wordArray[constants::MAX_WORDS];

//variable to keep track of next available slot in array

int nextEmpty = 0;

//functions in header file

void clearArray() {
	//should be zeroing out
	for (int i = 0; i < constants::MAX_WORDS; i++) {
		wordArray[i].word = "NONE";
		wordArray[i].number_occurences = 0;
	}
	nextEmpty = 0;

//	wordArray = {};
//	countArray = {};
}

int getArraySize() {
	//this needs to return the amount of unique words in the array
	//int size = 0;
	return nextEmpty;
}

std::string getArrayWordAt(int i) {
	std::string word = wordArray[i].word;
	return word;
}

int getArrayWord_NumbOccur_At(int i) {
	int numbOccur = wordArray[i].number_occurences;
	return numbOccur;
}

void processToken(std::string &token) {
	if (strip_unwanted_chars(token)) {
		bool isIn = false;
		//add as-is string and convert token to upper for comparison?
		std::string caps = token;
		toUpper(caps);
		for (int i = 0; i < nextEmpty; i++) {

			if (caps == wordArray[i].upper) { //string is in wordArray //caps?
				isIn = true;
				wordArray[i].number_occurences++; //increment int
				return; //not necessary, but may save time
			}
		}

		if (!isIn) { // else add string to array
			wordArray[nextEmpty].word = token;
			wordArray[nextEmpty].number_occurences = 1;
			//toUpper(token);
			wordArray[nextEmpty].upper = caps;

			nextEmpty++;
		}
	}

}

void processLine(std::string &myString) {
	std::string token;
	std::stringstream ss(myString);

	while (getline(ss, token, constants::CHAR_TO_SEARCH_FOR)) { //while there are words in the line
		processToken(token); //process the next word
	}
}

bool processFile(std::fstream &myfstream) {
	if (!myfstream.is_open() /*if myfstream is not open*/) {
		return false;
	}

	std::string line;

	while (!myfstream.eof()) { //myfstream has next line
		getline(myfstream, line);
		processLine(line); //process next line
		//cout<<line;
		//make sure to pass in one line, not the whole file
	}

	return true;
}

bool openFile(std::fstream &myfile, const std::string &myFileName,
		std::ios_base::openmode mode = std::ios_base::in) {
	//how do you file io
	myfile.open(myFileName, mode);
	if (myfile.is_open()) {
		return true;
	}
	//if can't open return false;
	return false;
}

void closeFile(std::fstream &myfile) {
	if (myfile.is_open()) { // if file is open
		myfile.close();
	}
}

int writeArraytoFile(const std::string &outputfilename) {
//	openFile(myfilename);
	std::fstream myOutputfile;

	//myOutputfile.open(outputfilename, std::ios::out));
//	if (!myOutputfile.is_open()) {
	if (!openFile(myOutputfile, outputfilename,
			std::ios::out | std::ios::binary | std::ios::trunc)) {
		return constants::FAIL_FILE_DID_NOT_OPEN;
	}

	if (nextEmpty == 0) {
		return constants::FAIL_NO_ARRAY_DATA;
	}

	for (int i = 0; i < nextEmpty; i++) {
		myOutputfile << wordArray[i].word;
		myOutputfile << " ";
		myOutputfile << wordArray[i].number_occurences;
		myOutputfile << "\n";
	}
	//myOutputfile << "this gets written.\n";
	closeFile(myOutputfile);

	return constants::SUCCESS;
	//return 0;
}

void sortArray(constants::sortOrder so) {
	entry temp;
//	std::string temp1;
//	int tempnum1;
//	std::string temp2;

	switch (so) {
	case constants::NONE:
		break;

	case constants::ASCENDING:
		for (int i = 0; i < nextEmpty - 1; i++) {
			if (wordArray[i].upper > wordArray[i + 1].upper) {
				temp = wordArray[i + 1];
				wordArray[i + 1] = wordArray[i];
				wordArray[i] = temp;

				i = -1;
			}
		}
		break;

	case constants::DESCENDING:
		for (int i = 0; i < nextEmpty - 1; i++) {
			if (wordArray[i].upper < wordArray[i + 1].upper) {
				temp = wordArray[i + 1];
				wordArray[i + 1] = wordArray[i];
				wordArray[i] = temp;

				i = -1;
			}
		}
		break;

	case constants::NUMBER_OCCURRENCES: //the only thing I couldn't get working
		for (int i = 0; i < nextEmpty - 1; i++) {
			if (wordArray[i].number_occurences
					< wordArray[i + 1].number_occurences) {
				temp = wordArray[i + 1];
				wordArray[i + 1] = wordArray[i];
				wordArray[i] = temp;

				i = -1;
			}
		}
		break;

//	default:
//		break;
	}

}

//look in utilities.h for useful functions, particularly strip_unwanted_chars!

