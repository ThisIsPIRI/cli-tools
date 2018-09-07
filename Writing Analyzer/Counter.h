#ifndef PIRI_COUNTER
#define PIRI_COUNTER
#pragma once

#include <fstream>
#include <string>
#include <vector>

class Paragraph {
public:
	int chars, words, sentences;
	std::string firstWord, lastWord;
	Paragraph(int, int, int, std::string, std::string);
};

class Counter {
protected:
	bool inWord, inBlank;
	std::string lastWord, firstInThis;
	int paraCharCount, paraWordCount, paraSentenceCount;
	bool isAlphabet(char);
	void flipState();
	void paragraphEnd();
public:
	int charCount, wordCount, sentenceCount, paraCount;
	std::vector<Paragraph> paragraphs; //vector to store paraCharCounts, paraWordCounts and paraSentenceCounts
	void initialize();
	void count(std::ifstream&, bool, bool);
};
#endif