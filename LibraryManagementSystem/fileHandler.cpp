#include<iostream>
#include<filesystem>
#include<fstream>
#include<string>
#include"fileHandler.h"

namespace fs = std::filesystem;

using std::cout;
using std::ifstream;
using std::ofstream;
using std::string;
using std::vector;
using std::endl;

const fs::path CURRENT_DIRECTORY = fs::current_path();
const fs::path ACCOUNTS_PATH = CURRENT_DIRECTORY / "accounts.txt";
const fs::path BOOKS_PATH = CURRENT_DIRECTORY / "books.txt";
//std::FILE Accounts;
//std::FILE Books;
//fs::file

const string Delimiter = ", ";


void fileHandler::initializeFiles() {
	cout << "FILEHANDLER" << endl;
	cout << "CURRENT DIRECTORY: \t" << CURRENT_DIRECTORY << endl;
	cout << "ACCOUNTS FILEPATH: \t" << ACCOUNTS_PATH << endl;
	cout << "BOOKS FILEPATH: \t" << BOOKS_PATH << endl;
	cout << "Initialize Accounts file\n";

	if (fs::exists(ACCOUNTS_PATH))
		cout << "\t- accounts.txt already exists.\n";
	else {
		cout << "\t- accounts.txt does not exist, creating new file.\n";
		ofstream accounts("accounts.txt");
		accounts.close();
	}

	cout << "Initialize Books file\n";

	if (fs::exists(BOOKS_PATH))
		cout << "\t- books.txt already exists.\n";
	else {
		cout << "\t- books.txt does not exist, creating new file.\n";
		ofstream books("books.txt");
		books.close();
	}
}

void fileHandler::appendNewAccount(string username, string password) {
	ofstream accounts; accounts.open(ACCOUNTS_PATH, std::ios_base::app);
	accounts << username + Delimiter + password + ", 0" + "\n";  //unsure if ", " should be used as a seperator. structure exp?: "[USR], [PWD], [PERMISSIONLVL]\n" 
	accounts.close();
}



vector<string> splitByDelimiter(string& str, string delimiter) {
	vector<string> splitString;
	size_t pos = 0;
	string token;
	while ((pos = str.find(delimiter)) != string::npos) {
		token = str.substr(0, pos);
		splitString.push_back(token);
		str.erase(0, pos + delimiter.length());
	}
	splitString.push_back(str);
	return splitString;
}

account fileHandler::getAccountByName(string username) { //name pass role
	ifstream accounts(ACCOUNTS_PATH, std::ios::out);
	string line;
	while (std::getline(accounts, line)) {
		vector<string> Row = splitByDelimiter(line, Delimiter);
		if (Row.size() < 3) {
			cout << "Invalid account format, skipping..." << endl << endl;
			continue;
		}
		if (Row[0] == username) {
			account givenAcc;
			givenAcc.exists = true;
			givenAcc.permissionLevel = stoi(Row[2]);
			givenAcc.username = Row[0];
			return givenAcc;
		}
	}

	return account(); //return default exists? false
}

bool fileHandler::validateAccount(string username, string password) {
	ifstream accounts(ACCOUNTS_PATH, std::ios::out);
	string line;
	while (std::getline(accounts, line)) {
		vector<string> Row = splitByDelimiter(line, Delimiter);
		if (Row.size() < 3) {
			cout << "Invalid account format, skipping..." << endl << endl;
			continue;
		}
		if (Row[0] == username && Row[1] == password) {
			return true;
		}
	}

	return false;
}