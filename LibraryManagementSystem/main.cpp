#include<iostream>
#include<string>
#include<filesystem>
#include<fstream>
#include<conio.h> //for inputListener getch
#include<map>

namespace fs = std::filesystem;

using std::cout;
using std::cin;
using std::endl;
using std::string;
using std::ifstream;
using std::ofstream;
using std::string;
using std::vector;

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

//----------------------------------------------------------------------------------------------------------

struct account {
	std::string username;
	int permissionLevel = 0; //-1 guest, 0 user, 1 staff
	bool exists = false;
};

class accountHandler {
public:
	void registerFunction();

	void loginFunction();
};

//----------------------------------------------------------------------------------------------------------

class book {
public:
	std::string title;
	std::string author;
	std::string genre;
	std::string language;
	std::string ISBN;

	int pageCount = 0;
	int available = 0; //how many available to borrow
	int borrowed = 0; //how many currently borrowed
};
void addBook();
void removeBook();
void editBook();
void searchBook();
void sortBook();
void bookInterface();

//----------------------------------------------------------------------------------------------------------

class fileHandler {
public:
	void initializeFiles();

	void appendNewAccount(std::string username, std::string password);

	void appendNewBook(book newBook);

	account getAccountByName(std::string username);

	book getBookByISBN(string ISBN);

	void removeBook(string ISBN);

	bool validateAccount(std::string username, std::string password);

	std::vector<std::string> getBookNames();

private:
	vector<string> splitByDelimiter(string str, string delimiter);
};

//----------------------------------------------------------------------------------------------------------

int listenForInt();

char listenForChar();

string listenForString();

bool promptYesNo(std::string message);

//----------------------------------------------------------------------------------------------------------

class menu {
public:
	void SetTitle(std::string title);

	void SetDescription(std::string title);

	void AppendNav(std::string navText, int overwriteIndex = -1);

	void DisplayPage();
private:
	std::string PageTitle;
	std::string PageDesc;
	std::string PageNav = "";
	int PageNavIndex = 1;
};

//----------------------------------------------------------------------------------------------------------

accountHandler AccountHandler;
account currentUser;
bool guest;

const fs::path CURRENT_DIRECTORY = fs::current_path();
const fs::path ACCOUNTS_PATH = CURRENT_DIRECTORY / "accounts.txt";
const fs::path BOOKS_PATH = CURRENT_DIRECTORY / "books.txt";

const string Delimiter = ", ";
const string BookDelimiter = "|";

int main() {
	fileHandler().initializeFiles();

	std::cin.ignore();

	LABEL_START_MENU:
	menu Menu_Main;
	Menu_Main.SetTitle("Welcome to the library of [name here]!");
	Menu_Main.AppendNav("Login");
	Menu_Main.AppendNav("Register");
	Menu_Main.AppendNav("Browse as guest");
	Menu_Main.AppendNav("Quit", 0);
	Menu_Main.DisplayPage();

	while (true) {
		switch (listenForInt()) {
			case 1:
				cout << "entering login function";
				AccountHandler.loginFunction();
				if (currentUser.exists) {
					cout << "yay im in!" << endl;
				}
				else {
					cout << "im not in :(" << endl;
				}
				std::cin.ignore();
				break;
			case 2:
				cout << "entering register function";
				AccountHandler.registerFunction();
				break;
			case 3:
				guest = true;
				currentUser.username = "Guest";
				currentUser.permissionLevel = -1;
				break;
			case 4:
				return 0;
			default: continue;
		}
		
		if (currentUser.exists || guest) {
			break;
		}

		Menu_Main.DisplayPage();
	}
	
	menu Menu_1;
	Menu_1.SetTitle("Hello, " + currentUser.username + "!");
	Menu_1.SetDescription("Permission level: " + std::to_string(currentUser.permissionLevel) + "\n");
	
	Menu_1.AppendNav("Browse books");
	Menu_1.AppendNav("Facility booking");
	Menu_1.AppendNav("Feedback");
	if (currentUser.permissionLevel == 1) {
		Menu_1.AppendNav("Modify books");
		Menu_1.AppendNav("Manage users");
		Menu_1.AppendNav("Analysis");
	}
	Menu_1.AppendNav("Log out", 0);
	Menu_1.DisplayPage();

	while (true) {
		switch (listenForInt()) {
			cout << "waiting";
			case 0:
				cout << "CLEARING";
				guest = false;
				currentUser = account();
				goto LABEL_START_MENU;

			case 1:
				cout << "BROWSE";
				bookInterface();
				break;

			default: continue;
		}
		cout << "left case";
		Menu_1.DisplayPage();
	}

	return 0;
}

//----------------------------------------------------------------------------------------------------------

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

void accountHandler::loginFunction() {
LABEL_START_LOGIN_FUNCTION:
	if (!promptYesNo("Continue to login? [y/n]\n")) {
		currentUser = account();
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
		currentUser = fileHandler().getAccountByName(username);
		return;
	}
	else {
		cout << "INVALID USERNAME OR PASSWORD" << endl;
		goto LABEL_START_LOGIN_FUNCTION;
	}
}

//----------------------------------------------------------------------------------------------------------

void bookInterface() {
	menu bookMenu;
	bookMenu.SetTitle("Books");
	bookMenu.AppendNav("Add book");
	bookMenu.AppendNav("Remove book");
	bookMenu.AppendNav("Exit", 0);

	LABEL_BOOK_MENU:
	while (true) {
		bookMenu.DisplayPage();
		switch (listenForInt()) {
		case 1:
			addBook();
			break;
		case 2:
			removeBook();
			cout << "Removed";
			cin.ignore();
			break;
		case 0:
			return;
		}
	}
}

void addBook() {
	book newBook;

	cout << "Enter book ISBN: ";
	newBook.ISBN = listenForString();
	//cin.ignore();
	cout << "Enter book title: ";
	newBook.title = listenForString();

	cout << "Enter book category: ";
	newBook.genre = listenForString();

	cout << "Enter book author: ";
	newBook.author = listenForString();

	cout << "Enter book language: ";
	newBook.language = listenForString();

	cout << "Enter number of pages: ";
	cin >> newBook.pageCount;

	fileHandler().appendNewBook(newBook);

	cout << "Book added successfully." << endl;
	cin.ignore();
};

void removeBook() {
	cout << "Enter ISBN of book to remove: ";
	fileHandler().removeBook(listenForString());
}

//----------------------------------------------------------------------------------------------------------
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
	accounts << username << Delimiter << password << ", 0" << endl;  //unsure if ", " should be used as a seperator. structure exp?: "[USR], [PWD], [PERMISSIONLVL]\n" 
	accounts.close();
}

void fileHandler::appendNewBook(book newBook)
{
	ofstream booksFile;

	booksFile.open(BOOKS_PATH, std::ios_base::app);

	booksFile
		<< newBook.ISBN << BookDelimiter
		<< newBook.title << BookDelimiter
		<< newBook.genre << BookDelimiter
		<< newBook.author << BookDelimiter
		<< newBook.language << BookDelimiter
		<< newBook.pageCount
		<< endl;

	booksFile.close();
}

vector<string> fileHandler::splitByDelimiter(string str, string delimiter) {
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
	ifstream accounts(ACCOUNTS_PATH, std::ios::in);
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

book fileHandler::getBookByISBN(string ISBN) {
	return book();
}

void fileHandler::removeBook(string ISBN) { //remove line with the matched ISBN
	ifstream books(BOOKS_PATH, std::ios::in);
	ofstream newBooks;
	newBooks.open("temp.txt");
	string line;
	while (std::getline(books, line)) {
		cout << "next line" << endl;
		vector<string> Row = splitByDelimiter(line, BookDelimiter);
		cout << "[ISBN] [ROW[0]]\n" << ISBN << " | " << Row[0] << endl;
		if (Row[0] != ISBN) {
			cout << "copy line" << line << endl;
			
			newBooks << line << endl;
		}
		else {
			cout << "Skip this." << endl;
		}
	}

	newBooks.close();
	books.close();

	remove("books.txt");
	rename("temp.txt", fs::current_path() / "books.txt");
}

bool fileHandler::validateAccount(string username, string password) {
	ifstream accounts(ACCOUNTS_PATH, std::ios::in);
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

//----------------------------------------------------------------------------------------------------------

int listenForInt() {
	while (true) {
		char key = _getch();
		if (keyToInt.contains((int)key)) {
			std::cin.clear();
			return keyToInt[key];
		}
	}
}

char listenForChar() {
	return char(_getch());
}

string listenForString() {
	string text;
	getline(cin, text);
	std::cin.clear();
	return text;
}

bool promptYesNo(string message) {
	char letter;
	std::cout << message;
	while (true) {
		letter = listenForChar();
		if (letter == 'y') {
			std::cin.clear();
			return true;
		}
		else if (letter == 'n') {
			std::cin.clear();
			return false;
		}
	}
}

//----------------------------------------------------------------------------------------------------------

void menu::SetTitle(string title) {
	PageTitle = title;
}

void menu::SetDescription(string desc) {
	PageDesc = desc;
}

void menu::AppendNav(string navText, int overwriteIndex) {
	if (overwriteIndex != -1) {
		PageNav.append("[" + std::to_string(overwriteIndex) + "] " + navText + "\n");
		return;
	}
	PageNav.append("[" + std::to_string(PageNavIndex) + "] " + navText + "\n");
	PageNavIndex++;
}

void menu::DisplayPage() {
	system("cls");
	cout << PageTitle << endl << PageDesc << endl << PageNav << endl;
}