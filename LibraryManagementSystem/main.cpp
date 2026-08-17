#include<iostream>
#include<ios>
#include<iomanip>
#include<string>
#include<filesystem>
#include<fstream>
#include<conio.h> //for inputListener getch
#include<cstdlib>
#include<cstdio>
#include<utility>

using namespace std;

namespace fs = filesystem;

enum bookProperty { // do not rearrange!! it will break select menus and file handling, GREATLY!
	ISBN,
	Title,
	Author,
	Genre,
	Language,
	PageCount
};

//----------------------------------------------------------------------------------------------------------

void LoggedIn();

//----------------------------------------------------------------------------------------------------------

struct account {
	string username;
	int permissionLevel = 0; //-1 guest, 0 user, 1 staff
	bool exists = false;
};

void registerFunction();

void loginFunction();

//----------------------------------------------------------------------------------------------------------

struct book {
	string ISBN;
	string title;
	string author;
	string genre;
	string language;

	long long int pageCount = 0;
	int available = 0; //how many available to borrow
	int borrowed = 0; //how many currently borrowed
};

void addBook();
void removeBook();
void editBook();
void bookInterface();
void bookSearchSubInterface();
void displayAllBooks(bool sort = false, bookProperty BookProperty = ISBN, int selectionIndex = -1);
void displayBookMenuInterface();
void displayHeader();
void displayBookDetails(int index, book Book, int selectionIndex = -1);
void searchBookProperty();
void sortBooksBy(bookProperty BookProperty);
bookProperty selectBookProperty();

//----------------------------------------------------------------------------------------------------------

void initializeFiles();

void appendNewAccount(string username, string password);

void appendNewBook(book newBook);

account getAccountByName(string username);

void getBooksBy(bookProperty BookProperty, string Property);

void getAllBooks();

void removeBook(string ISBN);

bool validateAccount(string username, string password);

int splitByDelimiter(string str, string delimiter);

//----------------------------------------------------------------------------------------------------------

int listenForInt();

char listenForChar();

string listenForString();

long long int getNumber();

bool promptYesNo(string message);

//----------------------------------------------------------------------------------------------------------

struct menu {
	string PageTitle;
	string PageDesc;
	string PageNav = "";
	int PageNavIndex = 1;
};

void SetTitle(menu* Menu, string title);

void SetDescription(menu* Menu, string title);

void AppendNav(menu* Menu, string navText, int overwriteIndex = -1, char overwriteChar = '?');

void DisplayPage(menu* Menu, bool clearScreen = true);

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

const int MAX_BOOKS = 512;
const int SPLIT_BY_MAX = 16;

const fs::path CURRENT_DIRECTORY = fs::current_path();
const fs::path ACCOUNTS_PATH = CURRENT_DIRECTORY / "accounts.txt";
const fs::path BOOKS_PATH = CURRENT_DIRECTORY / "books.txt";

const string Delimiter = ", ";
const string BookDelimiter = "|";

account currentUser;
bool guest;

book BookArray[MAX_BOOKS];
int BooksIndexed = 0;

string SplitString[SPLIT_BY_MAX];

int main() {
	initializeFiles();
	initMenu_Main();

	cin.ignore();
	
	bool exitProgram = false;

	while (!exitProgram) {
		DisplayPage(&Menu_Main);
		switch (listenForInt()) {
		case 1:
			cout << "entering login function";
			loginFunction();
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
			registerFunction();
			break;
		case 3:
			guest = true;
			currentUser.username = "Guest";
			currentUser.permissionLevel = -1;
			break;
		case 0:
			exitProgram = true;
			break;
		}

		if (currentUser.exists || guest) {
			LoggedIn();
		}
	}
	return 0;
}

void LoggedIn() {
	initMenu_Main_Logged();
	initBookSearchMenu();
	initBookMenu();
	initDisplayBookMenu();

	bool exitMenu_Main = false;

	while (!exitMenu_Main) {
		DisplayPage(&Menu_Main_Logged);
		switch (listenForInt()) {
			cout << "waiting";
		case 0:
			cout << "CLEARING";
			guest = false;
			currentUser = account();
			exitMenu_Main = true;
			break;
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
		}
	}
	return;
}

void initMenu_Main() {
	Menu_Main = menu();
	SetTitle(&Menu_Main, "Welcome to the library of [name here]!");
	AppendNav(&Menu_Main, "Login");
	AppendNav(&Menu_Main, "Register");
	AppendNav(&Menu_Main, "Browse as guest");
	AppendNav(&Menu_Main, "Quit", 0);
}

void initMenu_Main_Logged() {
	Menu_Main_Logged = menu();
	SetTitle(&Menu_Main_Logged, "Hello, " + currentUser.username + "!");

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

	SetDescription(&Menu_Main_Logged, "Permission level: " + to_string(currentUser.permissionLevel) + " (" + accountType + ")\n");

	AppendNav(&Menu_Main_Logged, "Browse books");
	AppendNav(&Menu_Main_Logged, "Facility booking");
	AppendNav(&Menu_Main_Logged, "Feedback");
	if (currentUser.permissionLevel == 1) {
		AppendNav(&Menu_Main_Logged, "Modify books");
		AppendNav(&Menu_Main_Logged, "Manage users");
		AppendNav(&Menu_Main_Logged, "Analysis");
	}
	AppendNav(&Menu_Main_Logged, "Log out", 0);
}

void initBookMenu() {
	bookMenu = menu();
	SetTitle(&bookMenu, "Books");
	AppendNav(&bookMenu, "Search book");
	if (currentUser.permissionLevel == 1) {
		AppendNav(&bookMenu, "Add book");
		AppendNav(&bookMenu, "Remove book");
	}
	AppendNav(&bookMenu, "Exit", 0);
}

void initBookSearchMenu() {
	bookSearchMenu = menu();
	SetTitle(&bookSearchMenu, "Search book");
	AppendNav(&bookSearchMenu, "Display all books");
	AppendNav(&bookSearchMenu, "By Property");
	AppendNav(&bookSearchMenu, "Exit", 0);
}

void initDisplayBookMenu() {
	displayBookMenu = menu();
	AppendNav(&displayBookMenu, "Sort", -1, 's');
	AppendNav(&displayBookMenu, "Select", -1, 'x');
	AppendNav(&displayBookMenu, "View selected", -1, 'v');
	AppendNav(&displayBookMenu, "Quit", 0);
}

//----------------------------------------------------------------------------------------------------------

static bool valueInRange(long long int value, long long int minRange, long long int maxRange) {
	if (value >= minRange && value <= maxRange) {
		return true;
	}
	return false;
}

static bool validString(string str) { //checks if string has invalid chars (restricted to only alphanumeric chars)
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

void registerFunction() {
	if (!promptYesNo("Continue to registration? [y/n]\n")) {
		return;
	}
	system("cls");

	string username, password;
	bool pass = false;
	while (!pass) {
		cout << "Input username (Alphanumeric characters only, must be between 3 to 36 characters in length.)\n"; //probably add requirements like "Must be x chars, no symbols"
		username = listenForString(); //add check for username conflicts

		if (!validString(username)) {
			cout << "Invalid characters.\n\n";
		}
		else if (!valueInRange(static_cast<int>(username.length()), 3, 36)) {
			cout << "Username must be between 3 to 36 characters in length.\n\n";
		}
		else {
			pass = true;
		}

	}
	pass = false;
	while (!pass) {
		cout << "Input password (Alphanumeric characters only, must be between 6 to 36 characters in length.)\n";
		password = listenForString();
		cout << "Input confirm password\n";
		string confirmPassword = listenForString();

		if (!(password == confirmPassword)) {
			cout << "Passwords do not match!\n\n"; continue;
		}
		if (!validString(password)) {
			cout << "Invalid characters.\n\n";
		}
		else if (!valueInRange(static_cast<int>(password.length()), 6, 36)) {
			cout << "Password must be between 6 to 36 characters in length.\n\n";
		}
		else {
			pass = true;
		}

	}

	cout << "REGISTER ACCOUNT WITH USERNAME OF " << username << " AND PASSWORD OF " << password << endl;
	appendNewAccount(username, password);
}

void loginFunction() {
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
	if (validateAccount(username, password)) {
		cout << "LOGIN SUCCESS." << endl;
		currentUser = getAccountByName(username);
		return;
	}
	else {
		cout << "INVALID USERNAME OR PASSWORD" << endl;
		loginFunction();
	}
}

//----------------------------------------------------------------------------------------------------------

void bookInterface() {
	bool exit = false;
	while (!exit) {
		DisplayPage(&bookMenu);
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
			exit = true;
		}
	}
}

void bookSearchSubInterface() {
	bool exit = false;
	while (!exit) {
		DisplayPage(&bookSearchMenu);
		switch (listenForInt()) {
		case 1: {
			getAllBooks();
			displayAllBooks(false, ISBN);
			displayBookMenuInterface();
			break;
		}
		case 2:
			searchBookProperty();
			break;
		case 0:
			exit = true;
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

void displayBookDetails(int index, book Book, int selectionIndex) {
	if (selectionIndex == index) {
		cout << setw(6 + 13 + 64 + 24 + 16 + 10 + 4 + (5 * 3)) << setfill('=') << "" << endl;
		cout << setfill(' ');
	}
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
		<< endl;
	if (selectionIndex == index) {
		cout << setw(6 + 13 + 64 + 24 + 16 + 10 + 4 + (5 * 3)) << setfill('=') << "" << endl;
		cout << setfill(' ');
	}
	cout << setw(0);
}

void displayAllBooks(bool sort, bookProperty BookProperty, int selectionIndex) {
	system("CLS");

	if (sort) {
		sortBooksBy(BookProperty);
	}

	displayHeader();
	cout << endl;
	for (int i = 0; i < BooksIndexed; i++) {
		displayBookDetails(i + 1, BookArray[i], selectionIndex);
	}
}

void displayBookMenuInterface() {
	DisplayPage(&displayBookMenu, false);
	bool exit = false;
	bool actionMade = false;

	long long int selectionIndex = -1;
	bool sort = false;
	bookProperty sortProperty = ISBN;

	while (!exit) {
		while (!actionMade) {
			switch (listenForChar()) {
			case 's':
				cout << "Sort";
				selectionIndex = -1; //just deselect the selected book unless you want the selection to follow where the book was sorted to
				sort = true;
				sortProperty = selectBookProperty();
				actionMade = true;
				break;
			case 'x':
				cout << "Input selection index" << endl;
				selectionIndex = getNumber();
				if (selectionIndex < 0 || selectionIndex > BooksIndexed - 1) {
					selectionIndex = -2;
				}
				actionMade = true;
				break;
			case 'v':
				cout << "View selected" << endl;
				break;
			case '0':
				actionMade = true;
				exit = true;	
			}
		}

		displayAllBooks(sort, sortProperty, selectionIndex);
		DisplayPage(&displayBookMenu, false);

		if (sort) {
			cout << "Displaying sorted" << endl;
		}
		if (selectionIndex == -2) {
			cout << "INDEX OUT OF RANGE" << endl;
		}

		if (selectionIndex != -1) selectionIndex = -1; 
		sort = false;
		sortProperty = ISBN;
		actionMade = false;
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
	int num;
	while (true) {
		num = listenForInt();
		if (num > 6) {
			continue;
		}
		break;
	}
	
	return bookProperty(num - 1);
}

void searchBookProperty() {
	system("CLS");
	bookProperty BookProperty = selectBookProperty();
	cout << "Search by : ";
	getBooksBy(BookProperty, listenForString());
	system("CLS");
	if (BooksIndexed == 0) {
		cout << "No entries found." << endl << "Press enter to exit.";
		cin.ignore();
		return;
	}
	else {
		cout << BooksIndexed << " entries found." << endl;
		displayHeader();
		cout << endl;
		for (int i = 0; i < BooksIndexed; i++) {
			displayBookDetails(i + 1, BookArray[i]);
		}
	}
	displayBookMenuInterface();
	return;
}

//----------------------------------------------------------------------------------------------------------

static bool hasOnlyInt(string str) {
	for (int i = 0; i < str.length(); i++) {
		if (!valueInRange((int)str[i], 48, 57)) {
			return false;
		}
	}
	return true;
}

static bool containsDelimiter(string str) {
	for (int i = 0; i < str.length(); i++) {
		if (str[i] == '|') {
			return true;
		}
	}
	return false;
}

void addBook() {
	book newBook;

	do {
		cout << "Enter book ISBN: ";
		string ISBN = listenForString();
		if (!(ISBN.length() == 10 || ISBN.length() == 13)) {
			cout << "INVALID ISBN, ISBN must be 10 (ISBN10) or 13 (ISBN13) numbers long." << endl;
			continue;
		}
		if (!hasOnlyInt(ISBN)) {
			cout << "INVALID ISBN, ISBN cannot contain non numeric values." << endl;
			continue;
		}
		newBook.ISBN = ISBN;
	} while (newBook.ISBN == "");
	
	do {
		cout << "Enter book title: ";
		string TITLE = listenForString();
		if (containsDelimiter(TITLE)) {
			cout << "ILLEGAL CHARACTER, system does not allow usage of \'|\' (delimiter)" << endl;
			continue;
		}
		newBook.title = TITLE;
	} while (newBook.title == "");

	
	do {
		cout << "Enter book genre: ";
		string GENRE = listenForString();
		if (containsDelimiter(GENRE)) {
			cout << "ILLEGAL CHARACTER, system does not allow usage of \'|\' (delimiter)" << endl;
			continue;
		}
		newBook.genre = GENRE;
	} while (newBook.genre == "");

	do {
		cout << "Enter book author: ";
		string AUTHOR = listenForString();
		if (containsDelimiter(AUTHOR)) {
			cout << "ILLEGAL CHARACTER, system does not allow usage of \'|\' (delimiter)" << endl;
			continue;
		}
		newBook.author = AUTHOR;
	} while (newBook.author == "");

	do {
		cout << "Enter book language: ";
		string LANGUAGE = listenForString();
		if (containsDelimiter(LANGUAGE)) {
			cout << "ILLEGAL CHARACTER, system does not allow usage of \'|\' (delimiter)" << endl;
			continue;
		}
		newBook.language = LANGUAGE;
	} while (newBook.language == "");

	do {
		cout << "Enter number of pages: ";
		long long int PAGECOUNT = getNumber();
		if (PAGECOUNT == -1) {
			cout << "INVALID PAGECOUNT, PAGECOUNT cannot contain, negative, decimal or non numeric values." << endl;
			continue;
		}
		newBook.pageCount = PAGECOUNT;
		appendNewBook(newBook);
	} while (newBook.pageCount == 0);
	

	cout << "Book added successfully." << endl;
	cin.ignore();
};

void removeBook() {
	cout << "Enter ISBN of book to remove: ";
	removeBook(listenForString());
}

//----------------------------------------------------------------------------------------------------------
void initializeFiles() {
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

void appendNewAccount(string username, string password) {
	ofstream accounts; accounts.open(ACCOUNTS_PATH, ios_base::app);
	accounts << username << Delimiter << password << ", 0" << endl;  //unsure if ", " should be used as a seperator. structure exp?: "[USR], [PWD], [PERMISSIONLVL]\n" 
	accounts.close();
}

void appendNewBook(book newBook)
{
	ofstream booksFile;

	booksFile.open(BOOKS_PATH, ios_base::app);

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

int splitByDelimiter(string str, string delimiter) {
	size_t pos = 0;
	string token;
	int i = 0;
	while ((pos = str.find(delimiter)) != string::npos) {
		token = str.substr(0, pos);
		SplitString[i] = token;
		str.erase(0, pos + delimiter.length());
		i++;
	}
	SplitString[i] = str;
	return i + 1;
}

account getAccountByName(string username) { //name pass role
	ifstream accounts(ACCOUNTS_PATH, ios::in);
	string line;
	while (getline(accounts, line)) {
		int splits = splitByDelimiter(line, Delimiter);
		if (splits < 3) {
			cout << "Invalid account format, skipping..." << endl << endl;
			continue;
		}
		if (SplitString[0] == username) {
			account givenAcc;
			givenAcc.exists = true;
			givenAcc.permissionLevel = stoi(SplitString[2]);
			givenAcc.username = SplitString[0];
			return givenAcc;
		}
	}

	return account(); //return default exists? false
}

void getBooksBy(bookProperty BookProperty, string Property) {
	ifstream books(BOOKS_PATH, ios::in);
	string line;
	int i = 0;
	while (getline(books, line)) {
		int splits = splitByDelimiter(line, BookDelimiter);
		
		if (SplitString[(int)BookProperty] == Property) {
			book Book;
			Book.ISBN = SplitString[0];
			Book.title = SplitString[1];
			Book.author = SplitString[2];
			Book.genre = SplitString[3];
			Book.language = SplitString[4];
			Book.pageCount = stoll(SplitString[5]);
			BookArray[i] = Book;
			i++;
		}
	}
	BooksIndexed = i;
	books.close();
	return;
	
}

void getAllBooks() {
	ifstream books(BOOKS_PATH, ios::in);
	string line;
	int i = 0;
	while (getline(books, line)) {
		splitByDelimiter(line, BookDelimiter);
		book Book;
		Book.ISBN = SplitString[0];
		Book.title = SplitString[1];
		Book.author = SplitString[2];
		Book.genre = SplitString[3];
		Book.language = SplitString[4];
		Book.pageCount = stoll(SplitString[5]);
		BookArray[i] = Book;
		i++;
	}
	BooksIndexed = i;
	books.close();
	return;
}

void removeBook(string ISBN) { //remove line with the matched ISBN
	ifstream books(BOOKS_PATH, ios::in);
	ofstream newBooks;
	newBooks.open("temp.txt");
	string line;
	while (getline(books, line)) {
		splitByDelimiter(line, BookDelimiter);
		if (SplitString[0] != ISBN) {
			newBooks << line << endl;
		}
	}

	newBooks.close();
	books.close();

	remove("books.txt");
	rename("temp.txt", fs::current_path() / "books.txt");
}

bool validateAccount(string username, string password) {
	ifstream accounts(ACCOUNTS_PATH, ios::in);
	string line;
	while (getline(accounts, line)) {
		int splits = splitByDelimiter(line, Delimiter);
		if (splits < 3) {
			cout << "Invalid account format, skipping..." << endl << endl;
			continue;
		}
		if (SplitString[0] == username && SplitString[1] == password) {
			return true;
		}
	}

	return false;
}

//----------------------------------------------------------------------------------------------------------

static int keyToInt(int key) {
	switch (key) {
	case 48:
		return 0;
	case 49:
		return 1;
	case 50:
		return 2;
	case 51:
		return 3;
	case 52:
		return 4;
	case 53:
		return 5;
	case 54:
		return 6;
	case 55:
		return 7;
	case 56:
		return 8;
	case 57:
		return 9;
	default:
		return -1;
	}
}

int listenForInt() {
	while (true) {
		char key = _getch();
		if (keyToInt((int)key) != -1) {
			cin.clear();
			return keyToInt(key);
		}
	}
}

char listenForChar() {
	return char(_getch());
}

long long int getNumber() {
	try {
		return stoll(listenForString());
	}
	catch (...) {
		return -1;
	}
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

void SetTitle(menu* Menu, string title) {
	Menu->PageTitle = title;
}

void SetDescription(menu* Menu, string desc) {
	Menu->PageDesc = desc;
}

void AppendNav(menu* Menu, string navText, int overwriteIndex, char overwriteChar) {
	if (overwriteChar != '?') {
		string str = "[] " + navText + "\n"; str.insert(str.begin() + 1, overwriteChar);
		Menu->PageNav.append(str);
		return;
	}
	if (overwriteIndex != -1) {
		Menu->PageNav.append("[" + to_string(overwriteIndex) + "] " + navText + "\n");
		return;
	}
	Menu->PageNav.append("[" + to_string(Menu->PageNavIndex) + "] " + navText + "\n");
	Menu->PageNavIndex++;
}

void DisplayPage(menu* Menu, bool clearScreen) {
	if (clearScreen) {
		system("cls");
	}
	cout << Menu->PageTitle << endl << Menu->PageDesc << endl << Menu->PageNav << endl;
}

//----------------------------------------------------------------------------------------------------------

static long long int convertToComparisonINT(bookProperty BookProperty, string Str) { //only uses first letter of string ASCII code, or entire int
	if (Str.length() == 0 || Str == "") {
		return 0;
	}
	switch (BookProperty) {
	case ISBN:
	case PageCount:
		return stoll(Str);

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

static string getPropertyFromBook(bookProperty BookProperty, book Book) {
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

void sortBooksBy(bookProperty BookProperty) {
	if (BooksIndexed < 2) return;
	int position = 0;
	long long int comparisonArray[MAX_BOOKS];
	long long int startComparisonInt;
	for (int i = 0; i < BooksIndexed; i++) {
		comparisonArray[i] = convertToComparisonINT(BookProperty, getPropertyFromBook(BookProperty, BookArray[i]));
	}

	do {
		long long int smallest = comparisonArray[position];
		startComparisonInt = smallest;
		int positionSmall = 0;

		for (int i = position + 1; i < BooksIndexed; i++) {

			if (smallest > comparisonArray[i]) {
				smallest = comparisonArray[i];
				positionSmall = i;
			}
		}
		if (startComparisonInt == smallest) {
			position++; continue;
		}
		
		swap(BookArray[position], BookArray[positionSmall]);
		
		swap(comparisonArray[position], comparisonArray[positionSmall]);
		position++;

	} while (!(position == BooksIndexed - 1));		
}