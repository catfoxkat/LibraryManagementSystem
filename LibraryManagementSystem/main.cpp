#include<iostream>
#include<ios>
#include<iomanip>
#include<string>
#include<filesystem>
#include<fstream>
#include<conio.h> //for inputListener getch
#include<cstdlib>
#include<utility>
#include<format>
#include<chrono> //for getting current year
#include<limits>

using namespace std;

namespace fs = filesystem;

enum userProperties {
	_username,
	_password,
	_permissionLevel
};

enum bookProperty { // do not rearrange!! it will break select menus and file handling, GREATLY!
	ISBN,
	Title,
	Author,
	Genre,
	Language,
	PageCount,
	Available,
	Borrowed
};

const int MAX_BOOKS = 512;
const int SPLIT_BY_MAX = 16;
const int MAX_FEEDBACK = 512;

const fs::path CURRENT_DIRECTORY = fs::current_path();
const fs::path ACCOUNTS_PATH = CURRENT_DIRECTORY / "accounts.txt";
const fs::path BOOKS_PATH = CURRENT_DIRECTORY / "books.txt";
const fs::path BORROWS_PATH = CURRENT_DIRECTORY / "borrows.txt";
const fs::path BOOKINGS_PATH = CURRENT_DIRECTORY / "bookings";
const fs::path FEEDBACK_PATH = CURRENT_DIRECTORY / "feedback.txt";

const string Delimiter = ", ";
const string BookDelimiter = "|";

//----------------------------------------------------------------------------------------------------------

void LoggedIn();

//----------------------------------------------------------------------------------------------------------

struct account {
	string username;
	int permissionLevel = 0; //-1 guest, 0 user, 1 staff
	bool exists = false;
};

string getValidUsername();
string getValidPassword();
void registerFunction();
void loginFunction();

//----------------------------------------------------------------------------------------------------------

const int currentDay = stoi(format("{:%d}", chrono::system_clock::now()));
const int currentMonth = stoi(format("{:%m}", chrono::system_clock::now()));
const int currentYear = stoi(format("{:%Y}", chrono::system_clock::now()));
const string Months[12] = { "Janaury", "Febuary", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December" }; //used in file structure

const string facilityTypes[3] = { "Discussion room", "Study room", "Computer lab" };
const int facilityTypesMaxPax[3] = { 6, 2, 10 }; // max occupancy per room
const int facilityTypeRoomAmount[3] = { 30, 60, 1 }; // how many rooms are there in total, computer lab has only 1 room to test overlapping times
const int maximumBookingDuration = 3; // how long each booking can last
const int bookingHourStart = 8; // earliest a booking can start
const int bookingHourEnd = 20; // latest a booking can end at

int loadedFacilityScheduleFile[91][13]; //first const must be sum of room amounts, 2nd const must be end - start + 1

struct date {
	int day;
	int month;
	int year;
};

int getMonthMaxDate(int month);
bool stringToDate(string str, date* Date);
void facilityBookingInterface();
void createBooking();
void viewBooking();

//----------------------------------------------------------------------------------------------------------

struct book {
	long long int ISBN = 0;
	string title;
	string author;
	string genre; //could be optimized to be an int and get genre from an array
	string language; //similar optimization to above

	int pageCount = -1;
	int available = -1; //how many available to borrow
	int borrowed = -1; //how many currently borrowed
};
static bool hasOnlyInt(string str);
static bool containsChar(string str, char character);
constexpr int digitsLength(long long int x);

void addBook();
void removeBookFunction();
void editBook(book *Book);
void bookInterface();
void bookSearchSubInterface();
void displayAllBooks(bool sort = false, bookProperty BookProperty = ISBN, int selectionIndex = -1);
void displayBookMenuInterface(bookProperty BookProperty, string Property, bool searched = false);
void displayHeader();
void displayBookDetails(int index, book Book, int selectionIndex = -1);
void searchBookProperty();
bookProperty selectBookProperty();
void viewSelectBook(book currentSelectedBook);
void sortBooksBy(bookProperty BookProperty);

//----------------------------------------------------------------------------------------------------------

void initializeFiles();

void initializeBookingFileStructure();

int splitByDelimiter(string str, string delimiter);

void loadORinitializeDateFile(date Date);

void addBooking(date Date, int skipOffset, int startTime, int endTime);

void appendNewBook(book newBook);
void getBooksBy(bookProperty BookProperty, string Property);
void getAllBooks();
bool removeBook(long long int ISBN);
void modifyBook(book* Book, bookProperty BookProperty, string Value);
bool checkIfBorrowed(long long int ISBN);
int countBorrows(long long int ISBN);
void borrowNewBook(long long int ISBN);
void returnBook(long long int ISBN);
void removeBookFromBorrow(long long int ISBN);
void changeBorrowUsername(string oldUsername, string currentUsername);

void appendNewAccount(string username, string password);
account getAccountByName(string username);
bool validateAccount(string username, string password);
bool checkUsername(string username);

void appendNewFeedback(string feedback);
void getAllFeedback();
void removeFeedback(int index);

//----------------------------------------------------------------------------------------------------------

void analysis();
void countBook();
void countGenre();
void countLanguage();
void countUser();

string genres[MAX_BOOKS];
int genreCount[MAX_BOOKS] = {};

string languages[MAX_BOOKS];
int languageCount[MAX_BOOKS] = {};

//----------------------------------------------------------------------------------------------------------

void manageUsers();
void listAllAccounts(bool loadOnly = false);
void selectViewAccount(string username = "");
void modifyPermissionLevel(string username);

void modifyAccountsFile(string username, userProperties Property, string newProperty, bool removeAccount = false);

const int MAX_USER_ACCOUNTS = 512;
string loadedAccounts[MAX_USER_ACCOUNTS];
int currentTotalAccountsLoaded = 0;

//----------------------------------------------------------------------------------------------------------

int listenForInt();
char listenForChar();
string listenForString();
int getINTNumber();
long long int getLLNumber();
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
void initViewSelectedBookMenu(book Book);
void initFacilityBookingMenu();
void initManageUsersMenu();
void initManageUserViewMenu(string accountString);
void initFeedbackMenu();

menu Menu_Main;
menu Menu_Main_Logged;
menu bookMenu;
menu bookSearchMenu;
menu displayBookMenu;
menu viewSelectedBookMenu;
menu facilityBookingMenu;
menu manageUsersMenu;
menu manageUserViewMenu;
menu feedbackMenu;

//----------------------------------------------------------------------------------------------------------

void feedbackInterface();
void createNewFeedback();
void viewFeedback();

string feedbackArray[MAX_FEEDBACK];
int feedbackIndexed = 0;

//----------------------------------------------------------------------------------------------------------

account currentUser;
bool guest;

book BookArray[MAX_BOOKS];
int BooksIndexed = 0;

string SplitString[SPLIT_BY_MAX];

int main() {
	cout << "DATE: " << currentDay << "/" << currentMonth << "/" << currentYear << endl << endl;
	initializeFiles();
	initializeBookingFileStructure();
	initMenu_Main();
	initFacilityBookingMenu();
	initManageUsersMenu();


	cin.ignore(numeric_limits<streamsize>::max(), '\n');
	
	bool exitProgram = false;
	
	while (!exitProgram) {
		DisplayPage(&Menu_Main);
		switch (listenForInt()) {
		case 1:
			cout << "entering login function";
			loginFunction();
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
	initFeedbackMenu();

	bool exitMenu_Main = false;

	while (!exitMenu_Main) {
		DisplayPage(&Menu_Main_Logged);
		switch (listenForInt()) {
		case 0:
			//cout << "CLEARING";
			guest = false;
			currentUser = account();
			exitMenu_Main = true;
			break;
		case 1:
			//cout << "BROWSE";
			bookInterface();
			break;
		case 2:
			if (currentUser.permissionLevel >= 0) {
				//cout << "FACILITY BOOKING";
				facilityBookingInterface();
			}
			break;
		case 3:
			if (currentUser.permissionLevel >= 0) {
				//cout << "FEEDBACK";
				feedbackInterface();
			}
			break;
		case 4:
			if (currentUser.permissionLevel == 1) { 
				//cout << "Manage users";
				manageUsers();
			}
			else if (currentUser.permissionLevel == 0) {
				selectViewAccount(currentUser.username);
			}
			break;
		case 5:
			if (currentUser.permissionLevel == 1) {
				//cout << "Analysis";
				analysis();
			}
			break;
		}
	}
	return;
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

string getValidUsername() {
	string username;
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
		else if (checkUsername(username)) {
			cout << "Username already taken.\n\n";
		}
		else {
			pass = true;
		}
	}
	return username;
}

string getValidPassword() {
	string password;
	bool pass = false;
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
	return password;
}

void registerFunction() {
	if (!promptYesNo("Continue to registration? [Y/N]\n")) {
		return;
	}
	system("cls");

	string username = getValidUsername(), password = getValidPassword();
	
	cout << "REGISTER ACCOUNT WITH USERNAME OF " << username << " AND PASSWORD OF " << password << endl;
	appendNewAccount(username, password);
}

void loginFunction() {
	if (!promptYesNo("Continue to login? [Y/N]\n")) {
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

int getMonthMaxDate(int month) {
	switch (month) {
	case 1:
	case 3:
	case 5:
	case 7:
	case 8:
	case 10:
	case 12:
		return 31;
	case 4:
	case 6:
	case 9:
	case 11:
		return 30;
	case 2:
		return 28;
	default:
		throw "INVALID MONTH";
	}
}

bool stringToDate(string str, date* Date) { // DD/MM/YYYY // DOES NOT ACCOUNT FOR LEAP YEARS
	if (splitByDelimiter(str, "/") != 3) {
		return false;
	}

	try {
		Date->day = stoi(SplitString[0]);
		Date->month = stoi(SplitString[1]);
		Date->year = stoi(SplitString[2]);
		if (Date->day < 1 || Date->month < 1)
			return false;

		if (!valueInRange(Date->day, 1, getMonthMaxDate(Date->month)))
			return false;
		
		if (!valueInRange(Date->month, 1, 12))
			return false;

		//if (Date->year != currentYear) //must be same as current year
		//	return false;
	}
	catch (...) {
		return false;
	}
	return true;
	
}

void facilityBookingInterface() {
	DisplayPage(&facilityBookingMenu);
	bool exit = false;
	while (!exit) {
		switch (listenForInt()) {
		case 1: {
			createBooking();
			cout << "Press Enter to return...";
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			break;
		}
		case 2:
			viewBooking();
			cout << "Press Enter to return...";
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			break;
		case 0:
			exit = true;
			break;
		}

		if (!exit) {
			system("CLS");
			DisplayPage(&facilityBookingMenu, false);
		}
	}
}

void createBooking() {
	system("CLS");

	cout << "Please select a facility type" << endl
		<< "[1] Discussion room" << endl //6
		<< "[2] Study room" << endl //2
		<< "[3] Computer lab" << endl; //10

	int chosenType = 0;
	do {
		chosenType = listenForInt();
		if (!(chosenType >= 1 && chosenType <= 3))
			chosenType = 0;

	} while (chosenType == 0);

	system("CLS");
	cout << "-> Selected facility type \t: " << facilityTypes[chosenType - 1] << endl << endl;

	int bookingPersonsAmount = 0;

	do {
		cout << "Number of persons (max: " << facilityTypesMaxPax[chosenType - 1] << "): ";
		bookingPersonsAmount = getINTNumber();
		if (bookingPersonsAmount == -1) {
			cout << "INVALID NUMBER, number of persons cannot be zero, contain, negative, decimal or non numeric values." << endl;
			bookingPersonsAmount = 0;
			continue;
		}
		if (!valueInRange(bookingPersonsAmount, 1, facilityTypesMaxPax[chosenType - 1])) {
			cout << "INVALID NUMBER, number of persons must be at least 1 but no more than set maximum." << endl;
			bookingPersonsAmount = 0;
			continue;
		}
	} while (bookingPersonsAmount == 0);

	system("CLS");
	cout << "-> Selected facility type \t: " << facilityTypes[chosenType - 1] << endl;
	cout << "-> Number of persons \t\t: " << bookingPersonsAmount << endl << endl;
	date Date;
	bool validDate = false;
	do {
		cout << "Input Date of booking (DD/MM/YYYY): ";
		validDate = stringToDate(listenForString(), &Date);
		if (!validDate) {
			cout << "Invalid date or format." << endl;
			continue;
		}

		if (currentDay != 31 && currentMonth != 12 && Date.year != currentYear) {
			validDate = false;
			cout << "Booking date must be current year" << endl;
			continue;
		}

		if (
			(((getMonthMaxDate(Date.month) - Date.day + currentDay > 5) && abs(Date.month - currentMonth) == 1))
			||
			((abs(currentDay - Date.day) > 5)  && (Date.month == currentMonth))
			||
			(abs(Date.month - currentMonth) > 1 && !(currentMonth == 12 && Date.month == 1 && currentYear < Date.year))
			||
			(Date.month < currentMonth && currentYear == Date.year)
			||
			(Date.day < currentDay && Date.month == currentMonth && currentYear == Date.year)
			) {
			validDate = false;
			cout << "Booking date must be in the future and not more than 5 days in advance." << endl;
			continue;
		}
	} while (!validDate);
	loadORinitializeDateFile(Date);


	int skipOffset;
	int endRoomIndex;

	switch (chosenType) {
	case 1:
		skipOffset = 0;
		endRoomIndex = facilityTypeRoomAmount[0];
		break;
	case 2:
		skipOffset = facilityTypeRoomAmount[0];
		endRoomIndex = facilityTypeRoomAmount[0] + facilityTypeRoomAmount[1];
		break;
	case 3:
		skipOffset = facilityTypeRoomAmount[0] + facilityTypeRoomAmount[1];
		endRoomIndex = facilityTypeRoomAmount[0] + facilityTypeRoomAmount[1] + facilityTypeRoomAmount[2];
		break;
	}

	bool finishTimeSelection = false;

	while (!finishTimeSelection) {
		int roomSelection = 0;
		while (roomSelection == 0) {
			system("CLS");
			cout << "-> Selected facility type \t: " << facilityTypes[chosenType - 1] << endl;
			cout << "-> Number of persons \t\t: " << bookingPersonsAmount << endl;
			cout << "-> Selected date \t\t: " << Date.day << "/" << Date.month << "/" << Date.year << endl << endl;
			cout << "[ROOM] [HOUR ";
			for (int i = 0; i < bookingHourEnd - bookingHourStart + 1; i++) {
				cout << setw(3) << to_string(i + bookingHourStart);
			}
			cout << "]" << endl;

			int index = 1;
			for (int i = skipOffset; i < endRoomIndex; i++) {
				cout << " " << setw(4) << left << to_string(index++) << " " << "|" << setw(6) << " ";
				for (int k = 0; k < bookingHourEnd - bookingHourStart + 1; k++) {
					if (loadedFacilityScheduleFile[i][k]) {
						cout << setw(3) << right << "x";
					}
					else cout << setw(3) << right << "-";
				}
				cout << endl;
			}
			cout << endl << "Select a room: ";
			int thisRoomSelection = getINTNumber();
			if (thisRoomSelection > 0 && thisRoomSelection <= facilityTypeRoomAmount[chosenType - 1])
				roomSelection = thisRoomSelection;
		}
		int startTime, endTime;

		cout << "Input start time of booking: ";
		startTime = getINTNumber();
		cout << "Input end time of booking: ";
		endTime = getINTNumber();

		if (endTime - startTime> maximumBookingDuration - 1) {
			cout << "Booking duration exceeds maximum hours (" << maximumBookingDuration << ")" << endl;
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			continue;
		}

		bool timeCollision = false;
		for (int k = startTime - bookingHourStart; k < endTime - bookingHourStart + 1; k++) {
			if (loadedFacilityScheduleFile[roomSelection - 1 + skipOffset][k])
				timeCollision = true;
		}

		if (timeCollision) {
			cout << "Given booking time collides with other bookings!";
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			continue;
		}

		addBooking(Date, skipOffset + roomSelection - 1, startTime - bookingHourStart, endTime - bookingHourStart);
		finishTimeSelection = true;
	}
}

void viewBooking() {
	system("CLS");

	cout << "Please select a facility type" << endl
		<< "[1] Discussion room" << endl //6
		<< "[2] Study room" << endl //2
		<< "[3] Computer lab" << endl; //10

	int chosenType = 0;
	do {
		chosenType = listenForInt();
		if (!(chosenType >= 1 && chosenType <= 3))
			chosenType = 0;

	} while (chosenType == 0);

	system("CLS");
	cout << "-> Selected facility type \t: " << facilityTypes[chosenType - 1] << endl << endl;

	date Date;
	bool validDate = false;
	do {
		cout << "Input Date of booking (DD/MM/YYYY): ";
		validDate = stringToDate(listenForString(), &Date);
		
		if (!validDate)
			cout << "Invalid date or format." << endl;
	} while (!validDate);
	loadORinitializeDateFile(Date);


	int skipOffset = 0;
	int endRoomIndex = 0;

	switch (chosenType) {
	case 1:
		skipOffset = 0;
		endRoomIndex = facilityTypeRoomAmount[0];
		break;
	case 2:
		skipOffset = facilityTypeRoomAmount[0];
		endRoomIndex = facilityTypeRoomAmount[0] + facilityTypeRoomAmount[1];
		break;
	case 3:
		skipOffset = facilityTypeRoomAmount[0] + facilityTypeRoomAmount[1];
		endRoomIndex = facilityTypeRoomAmount[0] + facilityTypeRoomAmount[1] + facilityTypeRoomAmount[2];
		break;
	}
	system("CLS");
	cout << "-> Selected facility type \t: " << facilityTypes[chosenType - 1] << endl;
	cout << "-> Selected date \t\t: " << Date.day << "/" << Date.month << "/" << Date.year << endl << endl;
	cout << "[ROOM] [HOUR ";
	for (int i = 0; i < bookingHourEnd - bookingHourStart + 1; i++) {
		cout << setw(3) << to_string(i + bookingHourStart);
	}
	cout << "]" << endl;

	int index = 1;
	for (int i = skipOffset; i < endRoomIndex; i++) {
		cout << " " << setw(4) << left << to_string(index++) << " " << "|" << setw(6) << " ";
		for (int k = 0; k < bookingHourEnd - bookingHourStart + 1; k++) {
			if (loadedFacilityScheduleFile[i][k]) {
				cout << setw(3) << right << "x";
			}
			else cout << setw(3) << right << "-";
		}
		cout << endl;
	}
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

static bool containsChar(string str, char character) {
	for (int i = 0; i < str.length(); i++) {
		if (str[i] == character) {
			return true;
		}
	}
	return false;
}

constexpr int digitsLength(long long int x) { // its either this or conversion to string to count length, reference code: https://stackoverflow.com/a/31576530
	return
		(x < 0 ? 0 :
		(x < 10 ? 1 :
		(x < 100 ? 2 :
		(x < 1000 ? 3 :
		(x < 10000 ? 4 :
		(x < 100000 ? 5 :
		(x < 1000000 ? 6 :
		(x < 10000000 ? 7 :
		(x < 100000000 ? 8 :
		(x < 1000000000 ? 9 :
		(x < 10000000000 ? 10 :
		(x < 100000000000 ? 11 :
		(x < 1000000000000 ? 12 :
		(x < 10000000000000 ? 13 : 
			14))))))))))))));
}

void addBook() {
	book newBook;
	if (!promptYesNo("Add a new book? [Y/N]"))
		return;
	do {
		cout << "Enter book ISBN: ";
		long long int ISBN = getLLNumber();
		if (!(digitsLength(ISBN) == 10 || digitsLength(ISBN) == 13)) {
			cout << "INVALID ISBN, ISBN must be 10 (ISBN10) or 13 (ISBN13) numbers long." << endl;
			continue;
		}
		newBook.ISBN = ISBN;
	} while (newBook.ISBN == 0);
	
	do {
		cout << "Enter book title: ";
		string TITLE = listenForString();
		if (containsChar(TITLE, '|')) {
			cout << "ILLEGAL CHARACTER, system does not allow usage of \'|\' (delimiter)" << endl;
			continue;
		}
		newBook.title = TITLE;
	} while (newBook.title == "");

	
	do {
		cout << "Enter book genre: ";
		string GENRE = listenForString();
		if (containsChar(GENRE, '|')) {
			cout << "ILLEGAL CHARACTER, system does not allow usage of \'|\' (delimiter)" << endl;
			continue;
		}
		newBook.genre = GENRE;
	} while (newBook.genre == "");

	do {
		cout << "Enter book author: ";
		string AUTHOR = listenForString();
		if (containsChar(AUTHOR, '|')) {
			cout << "ILLEGAL CHARACTER, system does not allow usage of \'|\' (delimiter)" << endl;
			continue;
		}
		newBook.author = AUTHOR;
	} while (newBook.author == "");

	do {
		cout << "Enter book language: ";
		string LANGUAGE = listenForString();
		if (containsChar(LANGUAGE, '|')) {
			cout << "ILLEGAL CHARACTER, system does not allow usage of \'|\' (delimiter)" << endl;
			continue;
		}
		newBook.language = LANGUAGE;
	} while (newBook.language == "");

	do {
		cout << "Enter number of pages: ";
		int PAGECOUNT = getINTNumber();
		if (PAGECOUNT == -1 || PAGECOUNT < 1) {
			cout << "INVALID PAGECOUNT, PAGECOUNT cannot be zero, contain, negative, decimal or non numeric values." << endl;
			continue;
		}
		newBook.pageCount = PAGECOUNT;
	} while (newBook.pageCount == -1);
	
	do {
		cout << "Enter available amount of book: ";
		int AVAILABLE = getINTNumber();
		if (AVAILABLE == -1 || AVAILABLE < 1) {
			cout << "INVALID AVAILABLE, AVAILABLE cannot be zero, contain, negative, decimal or non numeric values." << endl;
			continue;
		}
		newBook.available = AVAILABLE;
	} while (newBook.available == -1);

	appendNewBook(newBook);
	cout << "Book added successfully." << endl;
	cout << "Press Enter to return...";
	cin.ignore(numeric_limits<streamsize>::max(), '\n');
};

void removeBookFunction() {
	if (!promptYesNo(" Remove a book? [Y/N]"))
		return;
	cout << "Enter ISBN of book to remove: ";
	if (removeBook(getLLNumber()))
		cout << "Removal of book successful." << endl;
	else
		cout << "Could not remove book." << endl;
		
}

void editBook(book *Book) {
	cout << "Select data to modify" << endl
		<< "[1] ISBN" << endl
		<< "[2] Title" << endl
		<< "[3] Author" << endl
		<< "[4] Genre" << endl
		<< "[5] Language" << endl
		<< "[6] Pagecount" << endl
		<< "[7] Available" << endl
		<< "[8] Borrowed" << endl;
	int sel = 0;
	do {
		sel = listenForInt();
		if (!valueInRange(sel, 1, 8))
			sel = 0;
	} while (sel == 0);
	bookProperty BookProperty = bookProperty(sel - 1);

	
	
	string value = "";
	do {
		cout << "Input new value: ";
		value = listenForString();
		if (BookProperty == ISBN && !((value.length() == 10 || value.length() == 13) && hasOnlyInt(value))) {
			value = "";
			cout << "Invalid Value! ISBN Value be 10 or 13 numbers long and only have numeric values." << endl;
		}
		else continue;
		
		if ((BookProperty == PageCount || BookProperty == Available || BookProperty ==  Borrowed) && !hasOnlyInt(value)) {
			value = "";
			cout << "Invalid Value! Value can only have numeric values." << endl;
		}

		if (containsChar(value, '|')) {
			value = "";
			cout << "Invalid Value! Value cannot contain delimiter." << endl;
		}
		else continue;
	} while (value == "");

	modifyBook(Book, BookProperty, value);
	return;
}

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
			removeBookFunction();
			cout << "Press Enter to return..." << endl;
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			break;
		case 0:
			exit = true;
			break;
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
			displayBookMenuInterface(ISBN, "", false);
			break;
		}
		case 2:
			searchBookProperty();
			break;
		case 0:
			exit = true;
			break;
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
		<< setw(13) << to_string(Book.ISBN).substr(0, 13)
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

void displayBookMenuInterface(bookProperty BookProperty, string Property, bool searched) {
	DisplayPage(&displayBookMenu, false);
	bool exit = false;
	bool actionMade = false;

	int selectionIndex = -1;
	book currentSelectedBook;
	bool selectedBook = false;
	bool sort = false;
	bookProperty sortProperty = ISBN;

	while (!exit) {
		while (!actionMade) {
			switch (listenForChar()) {
			case 's':
				selectionIndex = -1; //just deselect the selected book unless you want the selection to follow where the book was sorted to
				currentSelectedBook = book();
				sort = true;
				sortProperty = selectBookProperty();
				actionMade = true;
				break;
			case 'x':
				cout << "Input selection index" << endl;
				selectionIndex = getINTNumber();
				if (selectionIndex <= 0 || selectionIndex > BooksIndexed) {
					selectionIndex = -2;
					currentSelectedBook = book();
				}
				else {
					currentSelectedBook = BookArray[selectionIndex - 1];
				}

				actionMade = true;
				break;
			case 'v':
				if (selectionIndex <= 0 || selectionIndex > BooksIndexed) break;
				viewSelectBook(currentSelectedBook);
				if (searched)
					getBooksBy(BookProperty, Property);
				else
					getAllBooks();
				
				if (selectionIndex > BooksIndexed) {
					selectionIndex = -2;
					currentSelectedBook = book();
				}
				else {
					currentSelectedBook = BookArray[selectionIndex - 1];
				}
				actionMade = true;
				break;
			case '0':
				actionMade = true;
				exit = true;
			}
		}

		if (!exit) {
			if (searched)
				getBooksBy(BookProperty, Property);
			else
				getAllBooks();
			displayAllBooks(sort, sortProperty, (int)selectionIndex);
			DisplayPage(&displayBookMenu, false);
		}

		if (sort) {
			cout << "Displaying sorted" << endl;
		}
		if (selectionIndex == -2) {
			cout << "INDEX OUT OF RANGE" << endl;
		}
		//sort = false;
		//sortProperty = ISBN;
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
	int num = 0;
	while (num == 0) {
		num = listenForInt();
		if (num > 6 || num < 1) {
			num = 0;
			continue;
		}
	}

	return bookProperty(num - 1);
}

void searchBookProperty() {
	system("CLS");
	bookProperty BookProperty = selectBookProperty();
	cout << "Search by : ";
	string Property = listenForString();
	getBooksBy(BookProperty, Property);
	system("CLS");
	if (BooksIndexed == 0) {
		cout << "No entries found." << endl << "Press enter to return.";
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
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
	displayBookMenuInterface(BookProperty, Property, true);
	return;
}

void viewSelectBook(book currentSelectedBook) {
	bool exit = false;
	while (!exit) {
		initViewSelectedBookMenu(currentSelectedBook);
		DisplayPage(&viewSelectedBookMenu);
		switch (listenForInt()) {
		case 1:
			if (currentUser.permissionLevel >= 0) {
				if (currentSelectedBook.available < 1) {
					cout << "There is no available copies of this book to borrow." << endl;
					cin.ignore(numeric_limits<streamsize>::max(), '\n');
					break;
				}

				if (!checkIfBorrowed(currentSelectedBook.ISBN)) {
					borrowNewBook(currentSelectedBook.ISBN);
					cout << "Sucessfully borrowed book.";
					modifyBook(&currentSelectedBook, Borrowed, to_string(currentSelectedBook.borrowed + 1));
					modifyBook(&currentSelectedBook, Available, to_string(currentSelectedBook.available - 1));
				}
				else {
					cout << "You are already borrowing this book." << endl;
					cin.ignore(numeric_limits<streamsize>::max(), '\n');
					break;
				}
					
				cin.ignore(numeric_limits<streamsize>::max(), '\n');
			}
			break;
		case 2:
			if (checkIfBorrowed(currentSelectedBook.ISBN)) {
				returnBook(currentSelectedBook.ISBN);
				modifyBook(&currentSelectedBook, Borrowed, to_string(currentSelectedBook.borrowed - 1));
				modifyBook(&currentSelectedBook, Available, to_string(currentSelectedBook.available + 1));
			}
			break;
		case 3:
			if (currentUser.permissionLevel == 1) {
				editBook(&currentSelectedBook);
			}
			break;
		case 4:
			if (currentUser.permissionLevel == 1) {
				removeBook(currentSelectedBook.ISBN);
				exit = true;
			}
			break;
		case 0:
			exit = true;
			break;
		}
	}
}

//----------------------------------------------------------------------------------------------------------

static long long int convertToComparisonINT(bookProperty BookProperty, string Str) { //only uses first letter of string ASCII code, or entire int
	if (Str.length() == 0 || Str == "") {
		return 0;
	}
	switch (BookProperty) {
	case ISBN:
		return stoll(Str);
	case PageCount:
		return stoi(Str);

	case Title:
	case Genre:
	case Author:
	case Language:
		return (int)(Str[0]); //use first letter for sorting unless you want to sort through the whole string using recursion

	default:
		cout << "Error with bookProperty";
		return 0;
	}
}

static string getPropertyFromBook(bookProperty BookProperty, book Book) {
	switch (BookProperty) {
	case ISBN:
		return to_string(Book.ISBN);
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
		throw "Error with book property";
	}
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

//----------------------------------------------------------------------------------------------------------

void initializeFiles() {
	cout << "=== FILEHANDLER ===" << endl;
	cout << "CURRENT DIRECTORY: \t" << CURRENT_DIRECTORY << endl;
	cout << "ACCOUNTS FILEPATH: \t" << ACCOUNTS_PATH << endl;
	cout << "BOOKS FILEPATH: \t" << BOOKS_PATH << endl;
	cout << "BOOKINGS PATH: \t\t" << BOOKINGS_PATH << endl;

	cout << "\nInitialize Accounts file\n";

	if (fs::exists(ACCOUNTS_PATH))
		cout << "\t- accounts.txt already exists.\n";
	else {
		cout << "\t- accounts.txt does not exist, creating new file.\n";
		ofstream accounts("accounts.txt");
		accounts.close();
	}

	cout << "\nInitialize Books file\n";

	if (fs::exists(BOOKS_PATH))
		cout << "\t- books.txt already exists.\n";
	else {
		cout << "\t- books.txt does not exist, creating new file.\n";
		ofstream books("books.txt");
		books.close();
	}

	cout << "\nInitialize Borrows file\n";

	if (fs::exists(BORROWS_PATH))
		cout << "\t- borrows.txt already exists.\n";
	else {
		cout << "\t- borrows.txt does not exist, creating new file.\n";
		ofstream borrows("borrows.txt");
		borrows.close();
	}

	cout << "\nInitialze Feedback file\n";

	if (fs::exists(FEEDBACK_PATH))
		cout << "\t- feedback.txt already exists.\n";
	else {
		cout << "\t- feedback.txt does not exist, creating new file.\n";
		ofstream feedbackFile(FEEDBACK_PATH);
		feedbackFile.close();
	}
}

void initializeBookingFileStructure() {
	cout << "\nInitialize Bookings file structure\n";
	if (fs::is_directory(BOOKINGS_PATH))
		cout << "\t- bookings directory already exists.\n";
	else {
		cout << "\t- bookings directory does not exist, creating new directory.\n";
		fs::create_directory(BOOKINGS_PATH);
	}

	cout << endl << "CURRENT YEAR: " << currentYear << endl;

	if (fs::is_directory(BOOKINGS_PATH / (to_string(currentYear)))) {
		cout << "\t- current year directory already exists\n";
		for (int i = 0; i < 12; i++) {
			if (fs::is_directory(BOOKINGS_PATH / (to_string(currentYear)) / Months[i])) {
				cout << "\t\t- month " << Months[i] << " directory exists\n";
			}
			else {
				cout << "\t\t- month " << Months[i] << " directory does not exist, creating new directory\n";
				fs::create_directory(BOOKINGS_PATH / (to_string(currentYear)) / Months[i]);
			}
		}
	}
	else {
		cout << "\t- current year directory does not exist, creating new directory\n";
		fs::create_directory(BOOKINGS_PATH / (to_string(currentYear)));
		cout << "\t- filling month directories\n";
		for (int i = 0; i < 12; i++) {
			fs::create_directory(BOOKINGS_PATH / (to_string(currentYear)) / Months[i]);
			cout << "\t\t- month " << Months[i] << " directory created\n";
		}
	}
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

void loadORinitializeDateFile(date Date) {
	string FileName = to_string(Date.day) + ".txt";
	fs::path FilePath = BOOKINGS_PATH / (to_string(currentYear)) / Months[Date.month - 1] / FileName;
	if (!fs::exists(FilePath)) {
		ofstream dateFile(FilePath);
		for (int i = 0; i < facilityTypeRoomAmount[0] + facilityTypeRoomAmount[1] + facilityTypeRoomAmount[2]; i++) {
			string line;
			for (int k = 0; k < bookingHourEnd - bookingHourStart + 1; k++) {
				line.append("0");
			}
			dateFile << line << endl;
		}
		dateFile.close();
	}
	ifstream dateFile(FilePath);
	string line;
	int index = 0;
	while (getline(dateFile, line)) {
		for (int i = 0; i < bookingHourEnd - bookingHourStart + 1; i++) {
			loadedFacilityScheduleFile[index][i] = ((int)line[i] - 48);
		}
		index++;
	}
	dateFile.close();
}

void addBooking(date Date, int skipOffset, int startTime, int endTime) {
	string FileName = to_string(Date.day) + ".txt";
	fs::path FilePath = BOOKINGS_PATH / (to_string(currentYear)) / Months[Date.month - 1] / FileName;
	ofstream newBooking;
	newBooking.open("temp.txt");

	if (!fs::exists(FilePath)) {
		ofstream dateFile(FilePath);
		for (int i = 0; i < facilityTypeRoomAmount[0] + facilityTypeRoomAmount[1] + facilityTypeRoomAmount[2]; i++) {
			string line;
			for (int k = 0; k < bookingHourEnd - bookingHourStart + 1; k++) {
				line.append("0");
			}
			dateFile << line << endl;
		}
		dateFile.close();
	}
	ifstream dateFile(FilePath);
	string line;
	int index = 0;
	while (getline(dateFile, line)) {
		if (index == skipOffset) {
			for (int k = 0; k < line.length(); k++)
				if (k >= startTime && k <= endTime)
					line[k] = '1';
		}
		newBooking << line << endl;
		index++;
	}
	dateFile.close();

	newBooking.close();

	remove(FilePath);
	rename("temp.txt", FilePath);
	return;
}

void appendNewBook(book newBook) {
	ofstream booksFile;

	booksFile.open(BOOKS_PATH, ios_base::app);

	booksFile
		<< newBook.ISBN << BookDelimiter
		<< newBook.title << BookDelimiter
		<< newBook.author << BookDelimiter
		<< newBook.genre << BookDelimiter
		<< newBook.language << BookDelimiter
		<< newBook.pageCount << BookDelimiter
		<< newBook.available << BookDelimiter
		<< 0 << BookDelimiter
		<< endl;

	booksFile.close();
}

void getBooksBy(bookProperty BookProperty, string Property) {
	ifstream books(BOOKS_PATH, ios::in);
	string line;
	int i = 0;
	while (getline(books, line)) {
		int splits = splitByDelimiter(line, BookDelimiter);

		if (SplitString[(int)BookProperty] == Property) {
			book Book;
			Book.ISBN = stoll(SplitString[0]);
			Book.title = SplitString[1];
			Book.author = SplitString[2];
			Book.genre = SplitString[3];
			Book.language = SplitString[4];
			Book.pageCount = stoi(SplitString[5]);
			Book.available = stoi(SplitString[6]);
			Book.borrowed = stoi(SplitString[7]);
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
		Book.ISBN = stoll(SplitString[0]);
		Book.title = SplitString[1];
		Book.author = SplitString[2];
		Book.genre = SplitString[3];
		Book.language = SplitString[4];
		Book.pageCount = stoi(SplitString[5]);
		Book.available = stoi(SplitString[6]);
		Book.borrowed = stoi(SplitString[7]);
		BookArray[i] = Book;
		i++;
	}
	BooksIndexed = i;
	return;
}

bool removeBook(long long int ISBN) { //remove line with the matched ISBN
	bool removed = false;
	ifstream books(BOOKS_PATH, ios::in);
	ofstream newBooks;
	newBooks.open("temp.txt");
	string line;
	while (getline(books, line)) {
		splitByDelimiter(line, BookDelimiter);
		if (stoll(SplitString[0]) != ISBN) {
			newBooks << line << endl;
		} else
			removed = true;
	}

	newBooks.close();
	books.close();

	remove(BOOKS_PATH);
	rename("temp.txt", BOOKS_PATH);

	removeBookFromBorrow(ISBN);
	return removed;
}

void modifyBook(book* Book, bookProperty BookProperty, string Value) {
	string comparisonISBN = "";
	switch (BookProperty) {
	case ISBN:
		comparisonISBN = to_string(Book->ISBN);
		Book->ISBN = stoll(Value);
		break;
	case Title:
		Book->title = Value;
		break;
	case Author:
		Book->author = Value;
		break;
	case Genre:
		Book->genre = Value;
		break;
	case Language:
		Book->language = Value;
		break;
	case PageCount:
		Book->pageCount = stoi(Value);
		break;
	case Available:
		Book->available = stoi(Value);
		break;
	case Borrowed:
		Book->borrowed = stoi(Value);
		break;
	}

	ifstream books(BOOKS_PATH, ios::in);
	ofstream newBooks;
	newBooks.open("temp.txt");
	string line;
	if (comparisonISBN == "")
		comparisonISBN = to_string(Book->ISBN);

	while (getline(books, line)) {
		splitByDelimiter(line, BookDelimiter);
		if (SplitString[0] != to_string(Book->ISBN) && SplitString[0] != comparisonISBN) {
			newBooks << line << endl;
		}
		if (SplitString[0] == to_string(Book->ISBN) || SplitString[0] == comparisonISBN)
			newBooks
			<< Book->ISBN << BookDelimiter
			<< Book->title << BookDelimiter
			<< Book->author << BookDelimiter
			<< Book->genre << BookDelimiter
			<< Book->language << BookDelimiter
			<< Book->pageCount << BookDelimiter
			<< Book->available << BookDelimiter
			<< Book->borrowed
			<< endl;

	}
	newBooks.close();
	books.close();

	remove(BOOKS_PATH);
	rename("temp.txt", BOOKS_PATH);
}

bool checkIfBorrowed(long long int ISBN) {
	ifstream borrows(BORROWS_PATH, ios_base::in);
	string line;
	while (getline(borrows, line)) {
		splitByDelimiter(line, BookDelimiter);
		if (SplitString[0] == to_string(ISBN) && SplitString[1] == currentUser.username)
			return true;
	}

	borrows.close();
	return false;
}

int countBorrows(long long int ISBN) {
	ifstream borrows(BORROWS_PATH, ios_base::in);
	string line; 
	int amount = 0;
	while (getline(borrows, line)) {
		splitByDelimiter(line, BookDelimiter);
		if (SplitString[0] == to_string(ISBN))
			amount++;
	}

	borrows.close();
	return amount;
}

void borrowNewBook(long long int ISBN) {
	ofstream borrows(BORROWS_PATH, ios_base::app);
	borrows << ISBN << BookDelimiter << currentUser.username << endl;
	borrows.close();
}

void returnBook(long long int ISBN) {
	ifstream borrows(BORROWS_PATH, ios_base::in);
	ofstream newBorrows("temp.txt");

	string line;
	while (getline(borrows, line)) {
		splitByDelimiter(line, BookDelimiter);
		if (SplitString[0] != to_string(ISBN) && SplitString[1] != currentUser.username)
			newBorrows << line << endl;
	}

	borrows.close();
	newBorrows.close();
	remove(BORROWS_PATH);
	rename("temp.txt", BORROWS_PATH);
}

void removeBookFromBorrow(long long int ISBN) { //could just use add an argument to returnBook to delete all instances but it would be dangerous
	ifstream borrows(BORROWS_PATH, ios_base::in);
	ofstream newBorrows("temp.txt");

	string line;
	while (getline(borrows, line)) {
		splitByDelimiter(line, BookDelimiter);
		if (SplitString[0] != to_string(ISBN))
			newBorrows << line << endl;
	}

	borrows.close();
	newBorrows.close();
	remove(BORROWS_PATH);
	rename("temp.txt", BORROWS_PATH);
}

void changeBorrowUsername(string oldUsername, string currentUsername) {
	ifstream borrows(BORROWS_PATH, ios_base::in);
	ofstream newBorrows("temp.txt");

	string line;
	while (getline(borrows, line)) {
		splitByDelimiter(line, BookDelimiter);
		if (SplitString[1] == oldUsername)
			newBorrows << SplitString[0] << BookDelimiter << currentUsername << endl;
		else
			newBorrows << line << endl;
	}

	borrows.close();
	newBorrows.close();
	remove(BORROWS_PATH);
	rename("temp.txt", BORROWS_PATH);
}


void appendNewAccount(string username, string password) {
	ofstream accounts; accounts.open(ACCOUNTS_PATH, ios_base::app);
	accounts << username << Delimiter << password << ", 0" << endl; //structure: "[USR], [PWD], [PERMISSIONLVL] ENDL" 
	accounts.close();
}

account getAccountByName(string username) { //name pass role
	ifstream accounts(ACCOUNTS_PATH, ios::in);
	string line;
	while (getline(accounts, line)) {
		int splits = splitByDelimiter(line, Delimiter);
		if (splits < 3) {
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

bool checkUsername(string username) {
	ifstream accounts(ACCOUNTS_PATH, ios::in);
	string line;
	while (getline(accounts, line)) {
		int splits = splitByDelimiter(line, Delimiter);
		if (splits < 3) {
			cout << "Invalid account format, skipping..." << endl << endl;
			continue;
		}
		if (SplitString[0] == username) {
			return true;
		}
	}

	return false;
}

void appendNewFeedback(string feedback) {
	ofstream feedbackFile(FEEDBACK_PATH, ios_base::app);
	feedbackFile << currentUser.username << BookDelimiter << feedback << endl;
	feedbackFile.close();
}

void getAllFeedback() {
	ifstream feedbackFile(FEEDBACK_PATH, ios::in);
	int i = 0;
	string line;
	while (getline(feedbackFile, line)) {
		feedbackArray[i] = line;
		i++;
	}
	feedbackIndexed = i;
	feedbackFile.close();
}

void removeFeedback(int index) {
	ifstream feedbackFile(FEEDBACK_PATH, ios::in);
	ofstream temp("temp.txt");
	int i = 0;
	string line;
	while (getline(feedbackFile, line)) {
		if (i == index) {
			i++;
			continue;
		}
		temp << line << endl;
		i++;
	}
	feedbackFile.close();
	temp.close();

	remove(FEEDBACK_PATH);
	rename("temp.txt", FEEDBACK_PATH);
}

//----------------------------------------------------------------------------------------------------------

void analysis() {
	bool exit = false;

	while (!exit) {
		system("CLS");

		cout << "====== ANALYSIS ======" << endl;
		cout << endl;
		cout << "[1] Total number of books" << endl;
		cout << "[2] Number of books by genre" << endl;
		cout << "[3] Number of books by language" << endl;
		cout << "[4] Number of users" << endl;
		cout << "[0] Exit" << endl;

		switch (listenForInt()) {
		case 1:
			countBook();
			break;

		case 2:
			countGenre();
			break;

		case 3:
			countLanguage();
			break;
		case 4:
			countUser();
			break;
		case 0:
			exit = true;
			break;
		}
	}
}

void countBook() {
	system("CLS");

	ifstream books(BOOKS_PATH);

	string line;
	int count = 0;

	while (getline(books, line)) {
		count++;
	}

	books.close();

	cout << "======= TOTAL NUMBER OF BOOKS =======" << endl;
	cout << endl;
	cout << "Total number of books: " << count << endl;

	cout << endl;
	cout << "Press Enter to return...";
	cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

void countGenre() { //should separate into more functions as it takes too much memory here
	system("CLS");

	ifstream books(BOOKS_PATH);

	string line;
	string genre;
	int count = 0;

	int genreIndex = 0;

	while (getline(books, line)) {
		splitByDelimiter(line, BookDelimiter);

		genre = SplitString[2];

		bool found = false;

		for (int i = 0; i < genreIndex; i++) {
			if (genres[i] == genre) {
				genreCount[i]++;
				found = true;
				break;
			}
		}

		if (!found) {
			genres[genreIndex] = genre;
			genreCount[genreIndex] = 1;
			genreIndex++;
		}
	}

	books.close();

	cout << "======= NUMBER OF BOOKS BY GENRE =======" << endl;
	cout << endl;

	for (int i = 0; i < genreIndex; i++) {
		cout << genres[i] << ": " << genreCount[i] << endl;
	}

	cout << endl;
	cout << "Press Enter to return...";
	cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

void countLanguage() {
	system("CLS");

	ifstream books(BOOKS_PATH);

	string line;
	string language;

	int languageIndex = 0;

	while (getline(books, line)) {
		splitByDelimiter(line, BookDelimiter);

		language = SplitString[4];

		bool found = false;

		for (int i = 0; i < languageIndex; i++) {
			if (languages[i] == language) {
				languageCount[i]++;
				found = true;
				break;
			}
		}

		if (!found) {
			languages[languageIndex] = language;
			languageCount[languageIndex] = 1;
			languageIndex++;
		}
	}

	books.close();

	cout << "========== NUMBER OF BOOKS BY LANGUAGE ==========" << endl;
	cout << endl;

	for (int i = 0; i < languageIndex; i++) {
		cout << languages[i] << ": " << languageCount[i] << endl;
	}

	cout << endl;
	cout << "Press Enter to return...";
	cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

void countUser() {
	system("CLS");

	ifstream accounts(ACCOUNTS_PATH);

	string line;
	int count = 0;

	while (getline(accounts, line)) {
		splitByDelimiter(line, Delimiter);

		if (SplitString[2] == "0") {
			count++;
		}
	}

	accounts.close();

	cout << "========== NUMBER OF USERS ==========" << endl;
	cout << endl;
	cout << "Total number of users: " << count << endl;

	cout << endl;
	cout << "Press Enter to return...";
	cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

//----------------------------------------------------------------------------------------------------------

void manageUsers() {
	bool exit = false;

	system("CLS");
	listAllAccounts();
	DisplayPage(&manageUsersMenu, false);
	while (!exit) {
		switch (listenForChar()) {
		case 'x':
			if (currentTotalAccountsLoaded == 0)
				break;
			selectViewAccount();
			break;
		case '0':
			exit = true;
			break;
		}

		system("CLS");
		listAllAccounts();
		DisplayPage(&manageUsersMenu, false);
	}
}

void listAllAccounts(bool loadOnly) {
	ifstream accounts("accounts.txt");
	string line;
	int index = 1;
	int amount = 0;
	while (getline(accounts, line)) {
		if (!loadOnly) {
			splitByDelimiter(line, Delimiter);
			cout << "[" << setw(3) << index++ << "] "
				<< SplitString[0] << endl;
		}
		loadedAccounts[amount] = line;
		amount++;
	}
	accounts.close();
	currentTotalAccountsLoaded = amount;
}

void selectViewAccount(string username) {
	int selectionIndex;
	if (username == "") {
		cout << "Input selection index: ";
		selectionIndex = getINTNumber();
		if (!valueInRange(selectionIndex, 1, currentTotalAccountsLoaded))
			return;
		selectionIndex--;
	}
	else {
		listAllAccounts(true);
		for (int i = 0; i < currentTotalAccountsLoaded; i++) {
			splitByDelimiter(loadedAccounts[i], Delimiter);
			if (SplitString[0] == username) {
				selectionIndex = i;
				break;
			}
				
		}
			
	}

	bool exit = false;
	bool actionMade = true;
	while (!exit) {
		if (actionMade) {
			actionMade = false;
			listAllAccounts(true);
			string accountString = loadedAccounts[selectionIndex];
			splitByDelimiter(accountString, Delimiter);
			if (username == "")
				username = SplitString[0];
			initManageUserViewMenu(accountString);
			DisplayPage(&manageUserViewMenu);
		}

		switch (listenForInt()) {
		case 1:	{
			string newUsername = getValidUsername();
			changeBorrowUsername(currentUser.username, newUsername);
			modifyAccountsFile(username, _username, newUsername, false);
			username = newUsername;
			actionMade = true;
			break;
		}
		case 2:
			modifyAccountsFile(username, _password, getValidPassword(), false);
			actionMade = true;
			break;
		case 3:
			if (currentUser.permissionLevel == 1) {
				modifyPermissionLevel(username);
				actionMade = true;
			}
			break;
		case 4:
			modifyAccountsFile(username, _username, "", true);
			exit = true;
			break;
		case 0:
			exit = true;
			break;
		}
	}
}

void modifyPermissionLevel(string username) {
	int level = -1;
	while (level < 0) {
		cout << "Enter Permission level" << endl;
		level = getINTNumber();
		if (!valueInRange(level, 0, 1)) {
			level = -1;
			cout << "Invalid Permission level" << endl;
		}
			
	}
	modifyAccountsFile(username, _permissionLevel, to_string(level));
}

void modifyAccountsFile(string username, userProperties Property, string newProperty, bool removeAccount) {
	ifstream accounts(ACCOUNTS_PATH, ios::in);
	ofstream newAccounts;
	newAccounts.open("temp.txt");
	string line;
	while (getline(accounts, line)) {
		splitByDelimiter(line, Delimiter);
		if (SplitString[0] != username) {
			newAccounts << line << endl;
		}
		else if (SplitString[0] == username && !removeAccount) {
			SplitString[(int)userProperties(Property)] = newProperty;
			newAccounts
				<< SplitString[0] << Delimiter
				<< SplitString[1] << Delimiter
				<< SplitString[2]
				<< endl;
		}
	}
	newAccounts.close();
	accounts.close();

	remove(ACCOUNTS_PATH);
	rename("temp.txt", ACCOUNTS_PATH);
}

//----------------------------------------------------------------------------------------------------------

static int keyToInt(int key) { //usage of maps (dictionary) is not allowed
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
	char key; bool gotKey = false;
	while (!gotKey) {
		key = _getch();
		if (keyToInt((int)key) != -1) {
			gotKey = true;
		}
	}
	cin.clear();
	return keyToInt(key);
}

char listenForChar() {
	return char(_getch());
}

int getINTNumber() {
	try {
		string str = listenForString();
		if (!hasOnlyInt(str))
			return -1;
		return stoi(str);
	}
	catch (...) {
		return -1;
	}
}

long long int getLLNumber() {
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
	char letter = 'x';
	cout << message;
	while (letter != 'y' && letter != 'n') {
		letter = listenForChar();
	}
	if (letter == 'y') {
		cin.clear();
		return true;
	}
	else {
		cin.clear();
		return false;
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
	if (currentUser.permissionLevel >= 0) {
		AppendNav(&Menu_Main_Logged, "Facility booking");
		AppendNav(&Menu_Main_Logged, "Feedback");
	}
	if (currentUser.permissionLevel == 1) {
		AppendNav(&Menu_Main_Logged, "Manage users"); //"Manage account" for Perm level 0
		AppendNav(&Menu_Main_Logged, "Analysis");
	}
	else if (currentUser.permissionLevel == 0) {
		AppendNav(&Menu_Main_Logged, "Manage account");
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

void initViewSelectedBookMenu(book Book) {
	viewSelectedBookMenu = menu();
	SetTitle(&viewSelectedBookMenu, "Selected book: " + Book.title);
	string Description =
		"\n ISBN code\t: " + to_string(Book.ISBN) +
		"\n Title\t\t: " + Book.title +
		"\n Author\t\t: " + Book.author +
		"\n Genre\t\t: " + Book.genre +
		"\n Language\t: " + Book.language +
		"\n Page count\t: " + to_string(Book.pageCount) +
		"\n Available\t: " + to_string(Book.available) +
		"\n Borrowed\t: " + to_string(Book.borrowed) +
		"\n";
	SetDescription(&viewSelectedBookMenu, Description);

	if (currentUser.permissionLevel >= 0) {
		AppendNav(&viewSelectedBookMenu, "Borrow book");
	}
	if (checkIfBorrowed(Book.ISBN)) {
		AppendNav(&viewSelectedBookMenu, "Return book");
	}
	if (currentUser.permissionLevel == 1) {
		AppendNav(&viewSelectedBookMenu, "Edit book details", 3);
		AppendNav(&viewSelectedBookMenu, "Delete book", 4);
	}
	AppendNav(&viewSelectedBookMenu, "Exit", 0);
}

void initFacilityBookingMenu() {
	facilityBookingMenu = menu();
	SetTitle(&facilityBookingMenu, "Facility booking");
	AppendNav(&facilityBookingMenu, "Create booking");
	AppendNav(&facilityBookingMenu, "View booking");
	AppendNav(&facilityBookingMenu, "Exit", 0);
}

void initManageUsersMenu() {
	manageUsersMenu = menu();
	AppendNav(&manageUsersMenu, "Select and view", -1, 'x');
	AppendNav(&manageUsersMenu, "Exit", 0);
}

void initManageUserViewMenu(string accountString) {
	splitByDelimiter(accountString, Delimiter);
	manageUserViewMenu = menu();
	SetTitle(&manageUserViewMenu, "Viewing user");
	SetDescription(&manageUserViewMenu, "Username \t\t: " + SplitString[0] + "\nPermission level \t: " + SplitString[2] + "\n");
	AppendNav(&manageUserViewMenu, "Change Username");
	AppendNav(&manageUserViewMenu, "Change Password");
	if (currentUser.permissionLevel == 1)
		AppendNav(&manageUserViewMenu, "Modify permission level");
	AppendNav(&manageUserViewMenu, "Remove account", 4);
	AppendNav(&manageUserViewMenu, "Exit", 0);
}

void initFeedbackMenu() {
	feedbackMenu = menu();
	SetTitle(&feedbackMenu, "Feedback menu");
	AppendNav(&feedbackMenu, "Send feedback");
	if (currentUser.permissionLevel == 1)
		AppendNav(&feedbackMenu, "View feedback");
	AppendNav(&feedbackMenu, "Exit", 0);
}

//----------------------------------------------------------------------------------------------------------

void feedbackInterface() {
	bool exit = false;
	while (!exit) {
		DisplayPage(&feedbackMenu);

		switch (listenForInt()) {
		case 1:
			createNewFeedback();
			break;
		case 2:
			if (currentUser.permissionLevel == 1)
				viewFeedback();
			break;
		case 0:
			exit = true;
			break;
		}
	}
}

void createNewFeedback() {
	cout << "Please insert your feedback: " << endl;
	string feedback;
	bool validFeedback = false;
	while (!validFeedback) {
		feedback = listenForString();
		validFeedback = true;
		if (containsChar(feedback, '|')) {
			validFeedback = false;
			cout << "Feedback cannot contain the character \"|\"" << endl;
		}
	}
	appendNewFeedback(feedback);
	cout << "Thank you for your feedback!" << endl 
		<< "It is very valuable to us and helps further improve our services." << endl;
	cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

void viewFeedback() {
	bool exit = false;
	bool actionMade = true;

	while (!exit) {
		if (actionMade) {
			system("CLS");
			getAllFeedback();
			cout << left << "      " << setw(16) << "Username  " << setw(0) << " | " << "Description" << endl;

			for (int i = 0; i < feedbackIndexed; i++) {
				splitByDelimiter(feedbackArray[i], "|");
				cout << right << "[" << setw(3) << i + 1  << "] " << left <<  setw(16) << SplitString[0] << setw(0) << " | " << SplitString[1] << endl;
			}
			cout << endl << "[x] Remove feedback" << endl
				<< "[0] Exit" << endl;
			actionMade = false;
		}

		switch (listenForChar()) {
		case 'x':
			cout << "Input selection index: ";
			removeFeedback(getINTNumber() - 1);
			actionMade = true;
			break;
		case '0':
			exit = true;
			break;
		}
	}
}