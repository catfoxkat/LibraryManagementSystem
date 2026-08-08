#include<iostream>
#include<string>
#include"inputListener.h"
#include"fileHandler.h"
#include"menu.h"
#include"bookHandler.h"

using std::cout;
using std::cin;
using std::endl;
using std::string;

void addBook();
void removeBook();
void editBook();
void searchBook();
void sortBook();

void bookHandler::openInterface() {
	menu bookMenu;
	bookMenu.SetTitle("Books");

	LABEL_BOOK_MENU:
	while (true) {
		bookMenu.DisplayPage();
		switch (listenForInt()) {
		case 1:
			addBook();
			break;
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

