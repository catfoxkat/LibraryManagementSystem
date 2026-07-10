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

const fs::path CURRENT_DIRECTORY = fs::current_path();
const fs::path ACCOUNTS_PATH = CURRENT_DIRECTORY / "accounts.txt";
const fs::path BOOKS_PATH = CURRENT_DIRECTORY / "books.txt";
//std::FILE Accounts;
//std::FILE Books;
//fs::file


void fileHandler::initializeFiles() {
	cout << CURRENT_DIRECTORY << std::endl;
	cout << ACCOUNTS_PATH << std::endl;
	cout << BOOKS_PATH << std::endl;
	cout << "Init Accounts\n";

	if (fs::exists(ACCOUNTS_PATH))
		cout << "accounts.txt already exists.\n";
	else {
		cout << "accounts.txt does not exist, creating new file.\n";
		ofstream accounts("accounts.txt");
		accounts.close();
	}

	cout << "Init Books\n";

	if (fs::exists(BOOKS_PATH))
		cout << "books.txt already exists.\n";
	else {
		cout << "books.txt does not exist, creating new file.\n";
		ofstream books("books.txt");
		books.close();
	}
}

void fileHandler::appendNewAccount(string username, string password) {
	ofstream accounts; accounts.open(ACCOUNTS_PATH, std::ios_base::app);
	accounts << username + ", " + password + "\n";  //unsure if ", " should be used as a seperator. structure exp?: "[USR], [PWD], [PERMISSIONLVL]\n" 
	accounts.close();
}