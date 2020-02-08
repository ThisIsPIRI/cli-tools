#include <cctype>
#include <fstream>
#include <iostream>
#include <cstdint>
#include <string>

using namespace std;

string capitalize(string thisString) {
	thisString[0] = toupper(thisString[0]);
	return thisString;
}

int main(int argc, char** argv) {
#ifdef BUILD_ENCRYPT
	const string verb = "encrypt";
#else
	const string verb = "decrypt";
#endif
	string fileName;
	bool toFile = true;

	if(argc >= 2) {
		fileName = argv[1];
		if(fileName == "-h") {
			cout << "Usage: " << verb << " <filename> <key> [-s]" << endl;
			cout << capitalize(verb) << "s a file using XOR." << endl;
			cout << "The key must be an integer less than 256." << endl;
			cout << "If -s is given, prints the result to stdout instead of a file." << endl;
			return 0;
		}
	}
	if(argc < 3) {
		cout << "Not enough arguments. Try " << verb << " -h for help." << endl;
		return 0;
	}
	else if(argc >= 4 && string(argv[3]) == "-s")
		toFile = false;

	//En/decrypt and overwrite. TODO: Allow for longer keys
	ifstream ifs;
	ostream *out = &cout;
	if(toFile) {
		ofstream *ofs = new ofstream();
#ifdef BUILD_ENCRYPT
		ofs->open(fileName + ".k", ios::binary);
#else
		ofs->open(fileName.substr(0, fileName.size() - 2), ios::binary);
#endif
		out = ofs;
	}
	char key = atoi(argv[2]);
	ifs.open(fileName, ios::binary);
	ifs.unsetf(ios::skipws);
	char c;
	while(ifs.get(c)) {
		(*out) << (uint8_t)(c ^ key);
	}
	ifs.close();
	if(toFile) {
		static_cast<ofstream*>(out)->close();
		delete out;
		remove(fileName.c_str());
	}
	return 0;
}
