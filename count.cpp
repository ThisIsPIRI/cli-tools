/*I'm giving unicode/international strings support up for an indefinite amount of time. Contributions welcome.

TODO: import standard libraries via modules
TODO: the C++ module system is not complete yet. Refactor to conform to the standars
when it comes out*/
#define _SILENCE_CXX17_CODECVT_HEADER_DEPRECATION_WARNING
#include <codecvt>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>

export module count; //module count; doesn't work but this does.

using namespace std;

#ifdef _WIN32
typedef wchar_t onechar;
typedef wstring onestring;
typedef wifstream oneifstream;
typedef wstringstream onestringstream;
#else
typedef char onechar;
typedef string onestring;
typedef ifstream oneifstream;
typedef stringstream onestringstream;
#endif

export class Paragraph {
public:
	int chars, words, sentences;
	onestring firstWord, lastWord;
	Paragraph(int c, int w, int s, onestring f, onestring l) : chars(c), words(w), sentences(s), firstWord(f), lastWord(l) {}
};

export class Counter {
protected:
	onechar read;
	bool inWord, inBlank, sentenceEndMet, paragraphJustEnded;
	int lineTemp, spaceTemp;
	onestring lastWord, firstInThis;
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
	void count(oneifstream& file) {
		initialize();
		//https://stackoverflow.com/a/10607146
		file.imbue(locale(locale::empty(), new codecvt_utf8<wchar_t>));
		onestringstream stream;
		stream << file.rdbuf();
		onestring str = stream.str();
		for(onechar& read : str) {
			//read = file.get(); //TODO: handle non-ASCII characters correctly
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