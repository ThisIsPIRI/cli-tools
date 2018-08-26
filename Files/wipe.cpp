#include <fstream>
#include <iostream>
#include <string>

using namespace std;

#define WIPE_NUM 5

int main(int argc, char** argv)
{
	ifstream ifs;
	ofstream ofs;
	streamoff length = 0;
	char wipeBits[WIPE_NUM][1] = { {0}, {10}, {100}, {-1}, {0} };
	if (argc < 2) {
		cout << "Not enough arguments. Try wipe /? for help." << endl;
		return 0;
	}
	if (strcmp(argv[1], "/?") == 0) { //help
		cout << "Usage : wipe <filename 1> <filename 2> ..." << endl;
		cout << "Wipes files, overwriting them with 0, 10, 100 and -1." << endl;
		cout << "If a filename contains a whitespace, surround it with \"\"" << endl;
		return 0;
	}
	for (int i = 1; i < argc; i++) {
		string fileName = argv[i];
		ifs.open(fileName, ios::binary);
		if (ifs.rdstate() == ios::failbit) {
			cout << "Couldn't find " << fileName << endl;
			ifs.close();
			continue;
		}
		ifs.seekg(0, ifs.end);
		length = ifs.tellg();
		ifs.close();
		ofs.open(fileName, ios::binary);
		for (int j = 0; j < WIPE_NUM; j++) { //Use WIPE_NUM different patterns to wipe
			ofs.seekp(0);
			for (int k = 0; k < length; k++) { //Overwrite the file
				ofs.write(wipeBits[j], 1);
			}
			ofs.flush(); //Physically write the patterns to the disk
			//TODO: Confirm the above does
		}
		ofs.close();
		remove(fileName.c_str()); //Remove the file
		cout << "Successfully wiped " << length << " bytes in " << fileName << endl;
	}
	return 0;
}