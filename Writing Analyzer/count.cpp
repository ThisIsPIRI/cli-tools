/*TODO: import standard libraries via modules
TODO: the C++ module system is not complete yet. Refactor to conform to the standars
when it comes out*/
#include <fstream>
#include <string>
#include <vector>

export module count; //module count; doesn't work but this does.

using namespace std;

export class Paragraph {
public:
	int chars, words, sentences;
	string firstWord, lastWord;
	Paragraph(int c, int w, int s, string f, string l) : chars(c), words(w), sentences(s), firstWord(f), lastWord(l) {}
};

export class Counter {
protected:
	char read;
	bool inWord, inBlank, sentenceEndMet, paragraphJustEnded;
	int lineTemp, spaceTemp;
	string lastWord, firstInThis;
	bool isAlphabet(char c) {
		return '!' <= c && c <= '~';
	}
	void flipState() {
		inWord = !inWord;
		inBlank = !inBlank;
	}
	void paragraphEnd() {
		paraCount++;
		paragraphs.push_back(Paragraph(paraCharCount, paraWordCount, paraSentenceCount, firstInThis, lastWord));
		charCount += paraCharCount;
		wordCount += paraWordCount;
		sentenceCount += paraSentenceCount;
		paraCharCount = paraWordCount = paraSentenceCount = 0;
	}
public:
	int charCount, wordCount, sentenceCount, paraCount, paraCharCount, paraWordCount, paraSentenceCount;
	bool countSpaces = false, countSentenceEnds = false;
	vector<Paragraph> paragraphs; //vector to store paraCharCounts, paraWordCounts and paraSentenceCounts
	void initialize() {
		inWord = sentenceEndMet = false;
		paragraphJustEnded = inBlank = true;
		lineTemp = spaceTemp = charCount = wordCount = sentenceCount = paraCount = paraCharCount = paraWordCount = paraSentenceCount = 0;
		paragraphs.clear();
	}
	void count(ifstream& file) {
		initialize();
		while (file.good()) {
			read = file.get(); //TODO: handle non-ASCII characters correctly
			switch (read) {
			case '\n':
				lineTemp++;
				goto BLANK; //Placed to avoid incrementing paraCharCount when '\n' is detected. TODO: Use a more elegant method if there is one
			case ' ':
				if (countSpaces) paraCharCount++;
			case '\t':
			BLANK:
				if (inWord) {
					flipState();
					if (sentenceEndMet) paraSentenceCount++;
				}
				break;
			case '.':case '?':case '!':
				sentenceEndMet = true;
				if (countSentenceEnds) paraCharCount++;
				continue; //instead of break so as to avoid sentenceEndMet being set to false
			default:
				//if (isAlphabet(read)) {
				if (inBlank) { //another word encountered
					if (lineTemp >= 2) { //new paragraph started
						paragraphEnd();
						paragraphJustEnded = true;
					}
					else if (paragraphJustEnded && paraWordCount > 0) { //The first word of a new paragraph. Check paraWordCount so it doesn't register an empty string as the first word of the first paragraph
						firstInThis = lastWord;
						paragraphJustEnded = false;
					}
					lastWord.clear(); //Before paragraphEnd() since the method uses the field
					lineTemp = 0;
					paraWordCount++;
					flipState();
				}
				paraCharCount++;
				lastWord += read;
				//}
				break;
			}
			sentenceEndMet = false;
		}
		//The last paragraph and sentence aren't counted in above loop
		paraSentenceCount++;
		paragraphEnd();
	}
};