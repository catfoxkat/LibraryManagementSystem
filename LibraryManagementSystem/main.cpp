#include<iostream>
#include<iomanip>
#include<string>
#include<filesystem>
#include<fstream>
#include<conio.h> //for inputListener getch
#include<map>
#include<vector>

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
using std::left;
using std::right;
using std::setw;
using std::setfill;


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

enum bookProperty { // do not rearrange!! it will break select menus and file handling, GREATLY!
	ISBN,
	Title,
	Author,
	Genre,
	Language,
	PageCount
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

struct book {
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
void displayAllBooks(vector<book>& Books, bool sort = false, bookProperty BookProperty = ISBN);
void displayBookMenuInterface(vector<book>& Books);
void displayHeader();
void displayBookDetails(int index, book Book);
void searchBookProperty();
void sortBooksBy(vector<book>& books, bookProperty BookProperty);
bookProperty selectBookProperty();

//----------------------------------------------------------------------------------------------------------

class fileHandler {
public:
	void initializeFiles();

	void appendNewAccount(string username, string password);

	void appendNewBook(book newBook);

	account getAccountByName(string username);

	vector<book> getBooksBy(bookProperty BookProperty, string Property);

	vector<book> getAllBooks();

	void removeBook(string ISBN);

	bool validateAccount(string username, string password);
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

	void AppendNav(string navText, int overwriteIndex = -1, char overwriteChar = '?');

	void DisplayPage(bool clearScreen = true);
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
void initDisplayBookMenu();
menu Menu_Main;
menu Menu_Main_Logged;
menu bookMenu;
menu bookSearchMenu;
menu displayBookMenu;

//----------------------------------------------------------------------------------------------------------

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
			accountHandler().loginFunction();
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
			accountHandler().registerFunction();
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
	initDisplayBookMenu();

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

	string accountType;
	switch (currentUser.permissionLevel) {
	case -1:
		accountType = "Guest";
		break;
	case 0:
		accountType = "User";
		break;
	case 1:
		accountType = "Administrator";
		break;
	default:
		accountType = "Unknown";
		break;
	}

	Menu_Main_Logged.SetDescription("Permission level: " + to_string(currentUser.permissionLevel) + " (" + accountType + ")\n");

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
	bookSearchMenu.AppendNav("By Property");
	bookSearchMenu.AppendNav("Exit", 0);
}

void initDisplayBookMenu() {
	displayBookMenu = menu();
	displayBookMenu.AppendNav("Sort", -1, 's');
	displayBookMenu.AppendNav("Select", -1, 'x');
	displayBookMenu.AppendNav("View selected", -1, 'v');
	displayBookMenu.AppendNav("Quit", 0);
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
		case 1: {
			vector<book> Books = fileHandler().getAllBooks();
			displayAllBooks(Books, false, ISBN);
			displayBookMenuInterface(Books);
			break;
		}
		case 2:
			searchBookProperty();
			break;
		case 0:
			return;
		}
	}
}

void displayHeader() {
	cout << left
		<< "      "
		<< setw(13) << "ISBN"
		<< " | "
		<< setw(64) << "TITLE"
		<< " | "
		<< setw(24) << "AUTHOR"
		<< " | "
		<< setw(16) << "GENRE"
		<< " | "
		<< setw(10) << "LANGUAGE"
		<< " | "
		<< setw(4) << "PAGE"
		<< setw(0) << endl;
}

void displayBookDetails(int index, book Book) {
	cout << right
		<< "[" << setw(3) << index << "] "
		<< left
		<< setw(13) << Book.ISBN.substr(0, 13)
		<< " | "
		<< setw(64) << Book.title.substr(0, 64)
		<< " | "
		<< setw(24) << Book.author.substr(0, 24)
		<< " | "
		<< setw(16) << Book.genre.substr(0, 16)
		<< " | "
		<< setw(10) << Book.language.substr(0, 10)
		<< " | "
		<< setw(4) << to_string(Book.pageCount).substr(0, 4)
		<< setw(0) << endl;
}

void displayAllBooks(vector<book>& Books, bool sort, bookProperty BookProperty) {
	system("CLS");

	if (sort) {
		sortBooksBy(Books, BookProperty);
	}

	displayHeader();
	cout << endl;
	for (int i = 0; i < Books.size(); i++) {
		displayBookDetails(i + 1, Books[i]);
	}
}

void displayBookMenuInterface(vector<book>& Books) { //ADD PER VECTOR SUPPORT (FOR SEARCHED BOOKS)
	displayBookMenu.DisplayPage(false);
	while (true) {
		switch (listenForChar()) {
		case 's':
			cout << "Sort";
			displayAllBooks(Books, true, selectBookProperty());
			displayBookMenu.DisplayPage(false);
			cout << "Displaying sorted";
			break;
		case 'x':
			cout << "Select";
			break;
		case 'v':
			cout << "View selected";
			break;
		case '0':
			return;
		default:
			continue;
		}
	}
}

bookProperty selectBookProperty() {
	cout << endl
		<< "Select a property." << endl
		<< "[1] ISBN" << endl
		<< "[2] Title" << endl
		<< "[3] Author" << endl
		<< "[4] Genre" << endl
		<< "[5] Language" << endl
		<< "[6] Page Count" << endl;
SELECTBOOKPROPERTY_LABEL:
	int num = listenForInt();
	if (num > 6) {
		goto SELECTBOOKPROPERTY_LABEL;
	}
	return bookProperty(num - 1);
}

void searchBookProperty() {
	system("CLS");
	bookProperty BookProperty = selectBookProperty();
	cout << "Search by : ";
	vector<book> Books = fileHandler().getBooksBy(BookProperty, listenForString());
	system("CLS");
	if (Books.size() == 0) {
		cout << "No entries found." << endl << "Press enter to exit.";
		cin.ignore();
		return;
	}
	else {
		cout << Books.size() << " entries found." << endl;
		displayHeader();
		cout << endl;
		for (int i = 0; i < Books.size(); i++) {
			displayBookDetails(i + 1, Books[i]);
		}
	}
	displayBookMenuInterface(Books);
	return;
}

//----------------------------------------------------------------------------------------------------------

bool hasOnlyInt(string str) {
	for (int i = 0; i < str.length(); i++) {
		if (!valueInRange((int)str[i], 48, 57)) {
			return false;
		}
	}
	return true;
}

bool containsDelimiter(string str) {
	for (int i = 0; i < str.length(); i++) {
		if (str[i] == '|') {
			return true;
		}
	}
	return false;
}

void addBook() {
	book newBook;

ADDBOOK_LABEL_ISBN:
	cout << "Enter book ISBN: ";
	string ISBN = listenForString();
	if (!(ISBN.length() == 10 || ISBN.length() == 13)) {
		cout << "INVALID ISBN, ISBN must be 10 (ISBN10) or 13 (ISBN13) numbers long." << endl;
		goto ADDBOOK_LABEL_ISBN;
	}
	if (!hasOnlyInt(ISBN)) {
		cout << "INVALID ISBN, ISBN cannot contain non numeric values." << endl;
		goto ADDBOOK_LABEL_ISBN;
	}
	newBook.ISBN = ISBN;

ADDBOOK_LABEL_TITLE:
	cout << "Enter book title: ";
	string TITLE = listenForString();
	if (containsDelimiter(TITLE)) {
		cout << "ILLEGAL CHARACTER, system does not allow usage of \'|\' (delimiter)" << endl;
		goto ADDBOOK_LABEL_TITLE;
	}
	newBook.title = TITLE;


ADDBOOK_LABEL_GENRE:
	cout << "Enter book genre: ";
	string GENRE = listenForString();
	if (containsDelimiter(GENRE)) {
		cout << "ILLEGAL CHARACTER, system does not allow usage of \'|\' (delimiter)" << endl;
		goto ADDBOOK_LABEL_GENRE;
	}
	newBook.genre = GENRE;

ADDBOOK_LABEL_AUTHOR:
	cout << "Enter book author: ";
	string AUTHOR = listenForString();
	if (containsDelimiter(AUTHOR)) {
		cout << "ILLEGAL CHARACTER, system does not allow usage of \'|\' (delimiter)" << endl;
		goto ADDBOOK_LABEL_AUTHOR;
	}
	newBook.author = AUTHOR;

ADDBOOK_LABEL_LANGUAGE:
	cout << "Enter book language: ";
	string LANGUAGE = listenForString();
	if (containsDelimiter(LANGUAGE)) {
		cout << "ILLEGAL CHARACTER, system does not allow usage of \'|\' (delimiter)" << endl;
		goto ADDBOOK_LABEL_LANGUAGE;
	}
	newBook.language = LANGUAGE;

ADDBOOK_LABEL_PAGECOUNT:
	cout << "Enter number of pages: ";
	string PAGECOUNT;
	cin >> PAGECOUNT;
	if (!hasOnlyInt(PAGECOUNT)) {
		cout << "INVALID PAGECOUNT, PAGECOUNT cannot contain non numeric values." << endl;
		goto ADDBOOK_LABEL_PAGECOUNT;
	}

	newBook.pageCount = std::stoi(PAGECOUNT);
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
		<< newBook.author << BookDelimiter
		<< newBook.genre << BookDelimiter
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
			givenAcc.permissionLevel = std::stoi(Row[2]);
			givenAcc.username = Row[0];
			return givenAcc;
		}
	}

	return account(); //return default exists? false
}

vector<book> fileHandler::getBooksBy(bookProperty BookProperty, string Property) {
	ifstream books(BOOKS_PATH, std::ios::in);
	string line;
	vector<book> bookArray;
	while (getline(books, line)) {
		vector<string> Row = splitByDelimiter(line, BookDelimiter);
		if (Row[(int)BookProperty] == Property) {
			book Book;
			Book.ISBN = Row[0];
			Book.title = Row[1];
			Book.genre = Row[2];
			Book.author = Row[3];
			Book.language = Row[4];
			Book.pageCount = std::stoi(Row[5]);
			bookArray.push_back(Book);
		}
	}
	books.close();
	return bookArray;
}

vector<book> fileHandler::getAllBooks() {
	ifstream books(BOOKS_PATH, std::ios::in);
	string line;
	vector<book> array;
	while (getline(books, line)) {
		vector<string> Row = splitByDelimiter(line, BookDelimiter);
		book Book;
		Book.ISBN = Row[0];
		Book.title = Row[1];
		Book.author = Row[2];
		Book.genre = Row[3];
		Book.language = Row[4];
		Book.pageCount = std::stoi(Row[5]);
		array.push_back(Book);
	}
	books.close();
	return array;
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

void menu::AppendNav(string navText, int overwriteIndex, char overwriteChar) {
	if (overwriteChar != '?') {
		string str = "[] " + navText + "\n"; str.insert(str.begin() + 1, overwriteChar);
		PageNav.append(str);
		return;
	}
	if (overwriteIndex != -1) {
		PageNav.append("[" + to_string(overwriteIndex) + "] " + navText + "\n");
		return;
	}
	PageNav.append("[" + to_string(PageNavIndex) + "] " + navText + "\n");
	PageNavIndex++;
}

void menu::DisplayPage(bool clearScreen) {
	if (clearScreen) {
		system("cls");
	}
	cout << PageTitle << endl << PageDesc << endl << PageNav << endl;
}

//----------------------------------------------------------------------------------------------------------

long long int convertToComparisonINT(bookProperty BookProperty, string Str) { //only uses first letter of string ASCII code, or entire int
	if (Str.length() == 0 || Str == "") {
		return 0;
	}
	switch (BookProperty) {
	case ISBN:
	case PageCount:
		return std::stoll(Str);

	case Title:
	case Genre:
	case Author:
	case Language:
		return (int)(Str[0]);

	default:
		cout << "Error with bookProperty";
		return 0;
	}
}

string getPropertyFromBook(bookProperty BookProperty, book Book) {
	switch (BookProperty) {
	case ISBN:
		return Book.ISBN;
	case Title:
		return Book.title;
	case Genre:
		return Book.genre;
	case Author:
		return Book.author;
	case Language:
		return Book.language;
	case PageCount:
		return to_string(Book.pageCount);
	default:
		cout << "Error with book property";
		return "";
	}
	return "";
}

void sortBooksBy(vector<book>& books, bookProperty BookProperty) {
	int position = 0;
	vector<long long int> comparisonArray;
	comparisonArray.reserve(books.size());
	for (int i = 0; i < books.size(); i++) {
		comparisonArray.push_back(convertToComparisonINT(BookProperty, getPropertyFromBook(BookProperty, books[i])));
	}
LABELSTARTSORT:
	long long int smallest = comparisonArray[0];
	int positionSmall = 0;

	for (int i = 1; i < comparisonArray.size(); i++) {
		if (smallest > comparisonArray[i]) {
			smallest = comparisonArray[i];
			positionSmall = i;
		}
	}

	books.insert(books.begin() + position, books[positionSmall + position]);
	books.erase(books.begin() + positionSmall + position + 1);

	comparisonArray.erase(comparisonArray.begin() + positionSmall);
	position++;

	if (!(comparisonArray.size() == 1))
		goto LABELSTARTSORT;
}