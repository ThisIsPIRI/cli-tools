#include <cstring>
#include <fstream>
#include <iostream>
#include <string>

#include "Counter.h"

using namespace std;

void printHelp();
int main(int argc, char** argv) {
	if(argc <= 1) { //The file name wasn't supplied
		printHelp();
		return 0;
	}
	//Parse and apply arguments
	Counter c;
	bool showParaWords = false, countSpaces = false, countSentenceEnds = false;
	for(int i = 2; i < argc; i++) {
		if(strcmp(argv[i], "-s") == 0) countSpaces = true;
		else if(strcmp(argv[i], "-p") == 0) countSentenceEnds = true;
		else if(strcmp(argv[i], "-w") == 0) showParaWords = true;
	}
	ifstream in(argv[1]);
	//Analyze the file
	c.count(in, countSpaces, countSentenceEnds);
	//Print results 
	cout << c.charCount << " characters, " << c.wordCount << " words, " << c.sentenceCount << " sentences, " << c.paraCount << " paragraphs\n";
	for(int i = 0; i < c.paraCount; i++) {
		cout << "Paragraph " << (i + 1) << " has " << c.paragraphs[i].sentences << " sentences, " << c.paragraphs[i].words << " words, " << c.paragraphs[i].chars << " characters\n";
		if(showParaWords) cout << '(' << c.paragraphs[i].firstWord << " ... " << c.paragraphs[i].lastWord << ")\n";
	}
	in.close();
	return 0;
}
void printHelp() {
	cout << "Usage : count filename [-s] [-p] [-w]\n";
	cout << "Counts(from an ASCII file):\n";
	cout << "number of pharagraphs, \n";
	cout << "number of characters, words and sentences(in entire file and in each paragraphs).\n";
	cout << "-s : count spaces as characters.\n";
	cout << "-p : count punctuation marks that end a sentence(., ?, !) as characters.\n";
	cout << "-w : show the first and last ewords of each paragraph.\n";
	cout << "A paragraph is a group of words preceded by two newlines or the start of the file.\n";
}
