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

	if (argc >= 2) {
		fileName = argv[1];
		if (fileName == "/?") { //Help. TODO: Make it more efficient
			cout << "Usage: " << verb << " <filename> <key>" << endl;
			cout << capitalize(verb) << "s a file using XOR." << endl;
			cout << "The key must be an integer less than 256." << endl;
			cout << "If the filename contains a whitespace, surround it with \"\"." << endl;
			return 0;
		}
	}
	if (argc < 3) {
		cout << "Not enough arguments. Try " << verb << " /? for help." << endl;
		return 0;
	}
	//En/decrypt and overwrite. TODO: Allow for longer keys
	ifstream ifs;
	ofstream ofs;
	char key = atoi(argv[2]);
#ifdef BUILD_ENCRYPT
	ofs.open(fileName + ".k", ios::binary);
#else
	ofs.open(fileName.substr(0, fileName.size() - 2), ios::binary);
#endif
	ifs.open(fileName, ios::binary);
	ifs.unsetf(ios::skipws);
	char c;
	while(ifs.get(c)) {
		ofs << (uint8_t)(c ^ key);
	}
	ifs.close();
	ofs.close();

	remove(fileName.c_str());
	cout << "Successfully " << verb << "ed the file." << endl;
	return 0;
}