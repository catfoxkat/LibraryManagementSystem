#include<iostream>
#include<string>
#include"inputListener.h"
#include"accountHandler.h"
#include"fileHandler.h"

using std::cout;
using std::endl;
using std::string;

bool validateAccountLogin(string username, string password) {
	return fileHandler().validateAccount(username, password);
}

bool valueInRange(int value, int minRange, int maxRange) {
	if (value >= minRange && value <= maxRange) {
		return true;
	}
	return false;
}

bool validString(string str) { //checks if string has invalid chars (restricted to only alphanumeric chars)
	for (int i = 0; i < str.length(); i++) {
		int characterCode = (int)str[i];
		// 0 - 9, A - Z, a - z
		if (valueInRange(characterCode, 48, 57) || valueInRange(characterCode, 65, 90) || valueInRange(characterCode, 97, 122)) {
			continue;
		}
		return false;
	}
	return true;
}

void accountHandler::registerFunction() {
	if (!promptYesNo("Continue to registration? [y/n]\n")) {
		return;
	}
	system("cls");

	inputusr:
	cout << "Input username (Alphanumeric characters only, must be between 3 to 36 characters in length.)\n"; //probably add requirements like "Must be x chars, no symbols"
	string username = listenForString(); //add check for username conflicts

	if (!validString(username)) {
		cout << "Invalid characters.\n\n";
		goto inputusr;
	}
	if (!valueInRange(static_cast<int>(username.length()), 3, 36)) {
		cout << "Username must be between 3 to 36 characters in length.\n\n"; goto inputusr; 
	}

	inputpwd:
	cout << "Input password (Alphanumeric characters only, must be between 6 to 36 characters in length.)\n";
	string password = listenForString();
	cout << "Input confirm password\n";
	string confirmPassword = listenForString();

	if (!(password == confirmPassword)) {
		cout << "Passwords do not match!\n\n"; goto inputpwd;
	}
	if (!validString(password)) {
		cout << "Invalid characters.\n\n";
		goto inputpwd;
	}
	if (!valueInRange(static_cast<int>(password.length()), 6, 36)) {
		cout << "Password must be between 6 to 36 characters in length.\n\n"; goto inputpwd; 
	}

	cout << "REGISTER ACCOUNT WITH USERNAME OF " << username << " AND PASSWORD OF " << password << endl;
	fileHandler().appendNewAccount(username, password);
}

void accountHandler::loginFunction(account* currentUser) {
	LABEL_START_LOGIN_FUNCTION:
	if (!promptYesNo("Continue to login? [y/n]\n")) {
		*currentUser = account();
		return;
	}

	system("cls");

	cout << "Input username\n";
	string username = listenForString();

	cout << "Input password\n";
	string password = listenForString();

	cout << "LOGIN WITH USERNAME OF " << username << " AND PASSWORD OF " << password << endl;
	if (validateAccountLogin(username, password)) {
		cout << "LOGIN SUCCESS." << endl;
		*currentUser = fileHandler().getAccountByName(username);
		return;
	}
	else {
		cout << "INVALID USERNAME OR PASSWORD" << endl;
		goto LABEL_START_LOGIN_FUNCTION;
	}
}