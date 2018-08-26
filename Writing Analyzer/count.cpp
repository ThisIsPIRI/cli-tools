//TODO: Use the module system
#include <fstream>
#include <string>
#include <vector>

#include "Counter.h"

using namespace std;

Paragraph::Paragraph(int c, int w, int s, string f, string l) : chars(c), words(w), sentences(s), firstWord(f), lastWord(l) {}

//protected
bool Counter::isAlphabet(char c) {
	return '!' <= c && c <= '~';
}
void Counter::flipState() {
	inWord = !inWord;
	inBlank = !inBlank;
}
void Counter::paragraphEnd() {
	paraCount++;
	paragraphs.push_back(Paragraph(paraCharCount, paraWordCount, paraSentenceCount, firstInThis, lastWord));
	charCount += paraCharCount;
	wordCount += paraWordCount;
	sentenceCount += paraSentenceCount;
	paraCharCount = paraWordCount = paraSentenceCount = 0;
}

//public
void Counter::initialize() {
	inWord = false;
	inBlank = true;
	charCount = wordCount = sentenceCount = paraCount = paraCharCount = paraWordCount = paraSentenceCount = 0;
	paragraphs.clear();
}
void Counter::count(ifstream& file, bool countSpaces = false, bool countSentenceEnds = false) {
	initialize();
	bool sentenceEndMet = false;
	bool paragraphJustEnded = true;
	int lineTemp = 0;
	char read; //TODO: handle non-ASCII characters correctly
	while (file.get(read)) {
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
			if (inBlank) { //Another word encountered
				if (lineTemp >= 2) { //New paragraph started
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
			break;
		}
		sentenceEndMet = false;
	}
	//The last paragraph and sentence aren't counted in above loop
	paraSentenceCount++;
	paragraphEnd();
}