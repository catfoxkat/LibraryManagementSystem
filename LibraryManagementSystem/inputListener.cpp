#include<conio.h>
#include<map>
#include<iostream>
#include<string>

using std::cin;
using std::getline;
using std::string;

std::map<int, int> keyToInt = {
			{48, 0},
			{49, 1},
			{50, 2},
			{51, 3},
			{52, 4},
			{53, 5},
			{54, 6},
			{55, 7},
			{56, 8},
			{57, 9}
};

int listenForInt() {
	while (true) {
		char key = _getch();
		if (keyToInt[key]) { return keyToInt[key]; }
	}
}

char listenForChar() {
	return char(_getch());
}

string listenForString() {
	string text;
	getline(cin, text);
	return text;
}

bool promptYesNo(string message) {
	char letter;
	std::cout << message;
	while (true) {
		letter = listenForChar();
		if (letter == 'y') {
			return true;
		}
		else if (letter == 'n') {
			return false;
		}
	}
}