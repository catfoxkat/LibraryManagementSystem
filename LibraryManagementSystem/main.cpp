#include<iostream>
#include<string>
#include<filesystem>
#include<fstream>
#include<conio.h> //for inputListener getch
#include<map>

namespace fs = std::filesystem;

using std::cout;
using std::cin;
using std::string;
using std::ifstream;
using std::ofstream;
using std::vector;
using std::endl;
using std::to_string;
using std::getline;

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
	string username;
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
	string title;
	string author;
	string genre;
	string language;
	string ISBN;

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
void bookSearchSubInterface();
//----------------------------------------------------------------------------------------------------------

class fileHandler {
public:
	void initializeFiles();

	void appendNewAccount(string username, string password);

	void appendNewBook(book newBook);

	account getAccountByName(string username);

	book getBookByISBN(string ISBN);

	void removeBook(string ISBN);

	bool validateAccount(string username, string password);

	vector<string> getBookNames();

private:
	vector<string> splitByDelimiter(string str, string delimiter);
};

//----------------------------------------------------------------------------------------------------------

int listenForInt();

char listenForChar();

string listenForString();

bool promptYesNo(string message);

//----------------------------------------------------------------------------------------------------------

class menu {
public:
	void SetTitle(string title);

	void SetDescription(string title);

	void AppendNav(string navText, int overwriteIndex = -1);

	void DisplayPage();
private:
	string PageTitle;
	string PageDesc;
	string PageNav = "";
	int PageNavIndex = 1;
};

//----------------------------------------------------------------------------------------------------------

void initMenu_Main();
void initMenu_Main_Logged();
void initBookMenu();
void initBookSearchMenu();
menu Menu_Main;
menu Menu_Main_Logged;
menu bookMenu;
menu bookSearchMenu;

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
	initMenu_Main();
	
	cin.ignore();
	
	
	LABEL_START_MENU:
	while (true) {
		Menu_Main.DisplayPage();
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
				cin.ignore();
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
			case 0:
				return 0;
			default: continue;
		}
		
		if (currentUser.exists || guest) {
			break;
		}
	}
	// LOGGED IN
	initMenu_Main_Logged();
	initBookSearchMenu();
	initBookMenu();

	while (true) {
		Menu_Main_Logged.DisplayPage();
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
			case 2:
				cout << "FACILITY BOOKING";
				break;
			case 3:
				cout << "FEEDBACK";
				break;
			case 4:
				if (currentUser.permissionLevel != 1) { break; }
				break;
			case 5:
				if (currentUser.permissionLevel != 1) { break; }
				break;
			case 6:
				if (currentUser.permissionLevel != 1) { break; }
				break;
			
			default: continue;
		}
	}

	return 0;
}

void initMenu_Main() {
	Menu_Main = menu();
	Menu_Main.SetTitle("Welcome to the library of [name here]!");
	Menu_Main.AppendNav("Login");
	Menu_Main.AppendNav("Register");
	Menu_Main.AppendNav("Browse as guest");
	Menu_Main.AppendNav("Quit", 0);
}

void initMenu_Main_Logged() {
	Menu_Main_Logged = menu();
	Menu_Main_Logged.SetTitle("Hello, " + currentUser.username + "!");
	Menu_Main_Logged.SetDescription("Permission level: " + to_string(currentUser.permissionLevel) + "\n");

	Menu_Main_Logged.AppendNav("Browse books");
	Menu_Main_Logged.AppendNav("Facility booking");
	Menu_Main_Logged.AppendNav("Feedback");
	if (currentUser.permissionLevel == 1) {
		Menu_Main_Logged.AppendNav("Modify books");
		Menu_Main_Logged.AppendNav("Manage users");
		Menu_Main_Logged.AppendNav("Analysis");
	}
	Menu_Main_Logged.AppendNav("Log out", 0);
}

void initBookMenu() {
	bookMenu = menu();
	bookMenu.SetTitle("Books");
	bookMenu.AppendNav("Search book");
	if (currentUser.permissionLevel == 1) {
		bookMenu.AppendNav("Add book");
		bookMenu.AppendNav("Remove book");
	}
	bookMenu.AppendNav("Exit", 0);
}

void initBookSearchMenu() {
	bookSearchMenu = menu();
	bookSearchMenu.SetTitle("Search book");
	bookSearchMenu.AppendNav("Display all books");
	bookSearchMenu.AppendNav("By ISBN");
	bookSearchMenu.AppendNav("By Title");
	bookSearchMenu.AppendNav("By Genre");
	bookSearchMenu.AppendNav("By Author");
	bookSearchMenu.AppendNav("By Language");
	bookSearchMenu.AppendNav("Exit", 0);
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
	//LABEL_BOOK_MENU:
	while (true) {
		bookMenu.DisplayPage();
		switch (listenForInt()) {
		case 1:
			bookSearchSubInterface();
			break;
		case 2:
			if (currentUser.permissionLevel != 1) { break; }
			addBook();
			break;
		case 3:
			if (currentUser.permissionLevel != 1) { break; }
			removeBook();
			cout << "Removed";
			cin.ignore();
			break;
		case 0:
			return;
		}
	}
}

void bookSearchSubInterface() {
	while (true) {
		bookSearchMenu.DisplayPage();
		switch (listenForInt()) {
		case 1:
			cout << "Diplay all";
			break;
		case 2:
			cout << "Search by ISBN";
			break;
		case 3:
			cout << "Search by Title";
			break;
		case 4:
			cout << "Search by Genre";
			break;
		case 5:
			cout << "Search by Author";
			break;
		case 6:
			cout << "Search by Language";
			break;
		case 0:
			return;
			break;
		}
		cout << "\nNext Loop";
		cin.ignore();
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
	while (getline(accounts, line)) {
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
	ifstream books(BOOKS_PATH, std::ios::in);
	string line;
	book Book;
	while (getline(books, line)) {
		vector<string> Row = splitByDelimiter(line, BookDelimiter);
		if (Row[0] != ISBN) {
			Book.ISBN = ISBN;
			Book.title = Row[1];
			Book.genre = Row[2];
			Book.author = Row[3];
			Book.language = Row[4];
			Book.pageCount = std::stoi(Row[5]);
		}
	}
	books.close();
	return book();
}

void fileHandler::removeBook(string ISBN) { //remove line with the matched ISBN
	ifstream books(BOOKS_PATH, std::ios::in);
	ofstream newBooks;
	newBooks.open("temp.txt");
	string line;
	while (std::getline(books, line)) {
		vector<string> Row = splitByDelimiter(line, BookDelimiter);
		if (Row[0] != ISBN) {
			newBooks << line << endl;
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
	while (getline(accounts, line)) {
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
			cin.clear();
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
	cin.clear();
	return text;
}

bool promptYesNo(string message) {
	char letter;
	cout << message;
	while (true) {
		letter = listenForChar();
		if (letter == 'y') {
			cin.clear();
			return true;
		}
		else if (letter == 'n') {
			cin.clear();
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
		PageNav.append("[" + to_string(overwriteIndex) + "] " + navText + "\n");
		return;
	}
	PageNav.append("[" + to_string(PageNavIndex) + "] " + navText + "\n");
	PageNavIndex++;
}

void menu::DisplayPage() {
	system("cls");
	cout << PageTitle << endl << PageDesc << endl << PageNav << endl;
}