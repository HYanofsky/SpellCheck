/*
Hadassah Yanofsky
DSA 2 - Fall 2020
Cooper Union 
*/

#include <iostream>
#include <fstream>
#include <sstream>
#include "hash.h"
using namespace std;

//load the dictionary into the hashTable
void loadDictonary(const string& filename, hashTable *dictionary) {
	bool valid; //makes sure that no char were invaild 
	string line;

	ifstream input(filename);

	//error if file can't be opened
	if (!input) {
		cerr << "Error: could not open " << filename << "\n";
		exit(1);
	}

	//read whole file
	while (!input.eof()) {
		getline(input, line);
		valid = true;

		//needs to be less than 20 chars for the word to be valid
		if (line.length() > 20) {
			valid = false;
		}

		for (size_t i = 0; i <= line.length(); i++) {
			//makes sure all chars are valid
			if (isdigit(line[i]) || isalpha(line[i]) || line[i] == '-' || line[i] == '\'' || int(line[i])==0){
				//change all upper to lower case
				if (isupper(line[i])) {
					line[i] = tolower(line[i]);
				}
			}
			else {
				valid = false;
			}
		}
		//load into the table
		if (valid == true) {
			dictionary->insert(line);
		}
	}
	input.close();
	
	
}

//Check the words in the document to see if they are valid
void CheckDoc(const string& in_filename, const string& out_filename, hashTable* dictionary) {
	string line, word;
	int line_num = 0;
	bool valid=true;

	ifstream input(in_filename);
	if (!input) {
		cerr << "Error: could not open " << in_filename << "\n";
		exit(1);
	}
	ofstream output(out_filename);
	if (!output) {
		cerr << "Error: could not open " << out_filename << "\n";
		exit(1);
	}

	//read the whole input file
	while (!input.eof()) {
		getline(input, line);
		line_num++;
		word = "";

		//parse each line into words
		for (size_t i = 0; i <= line.length(); i++) {
			//check if the char is valid
			if (isdigit(line[i]) || isalpha(line[i]) || line[i] == '-' || line[i] == '\'') {
				//change all upper to lower case
				if (isupper(line[i])) {
					line[i] = tolower(line[i]);
				}
				if (isdigit(line[i])) {
					valid = false;
				}
				word = word + line[i];
			}
			//when an invalid char is found then parse into new word and check if the word is valid
			else {
				//needs to be less than 20 chars and no numbers for the word to be valid
				if(word=="" || valid == false ){
					//do nothing
				}			
				else if (word.length() > 20) {
					output << "Long word at line " << line_num << ", starts: " << word.substr(0, 20) << "\n";
				}
				else if (!dictionary->contains(word)) {
					output << "Unknown word at line " << line_num << ": " << word << "\n";
				}
				//reset
				word = "";
				valid = true;
			}
		}
	}
	input.close();
	output.close();
}

int main() {
	//hashTable that will contain the dictionary
	hashTable dictionary(5000);

	
	string filename, in_filename, out_filename;
	cout << "Enter name of dictionary: ";
	cin >> filename;

	clock_t t1 = clock();

	//load dictionary into a hash table
	loadDictonary(filename, &dictionary);
	clock_t t2 = clock();
	double timeDiff = ((double)(t2 - t1)) / CLOCKS_PER_SEC;
	
	cout << "Total time (in seconds) to load dictionary: " << timeDiff << "\n";

	
	cout << "Enter name of input file: ";
	cin >> in_filename;
	cout << "Enter name of output file: ";
	cin >> out_filename;

	clock_t t3 = clock();
	//check the doc if words are valid
	CheckDoc(in_filename, out_filename, &dictionary);
	
	clock_t t4 = clock();
	double timeDiff2 = ((double)(t4 - t3))/ CLOCKS_PER_SEC;

	cout << "Total time(in seconds) to check document: " << timeDiff2 << "\n";
	
	
	return 0;
}
