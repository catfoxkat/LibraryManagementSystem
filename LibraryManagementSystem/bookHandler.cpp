#include<iostream>
#include<string>
#include"inputListener.h"
#include"fileHandler.h"
#include"menu.h"
#include"bookHandler.h"

using std::cout;
using std::endl;
using std::string;

void bookHandler::openInterface() {
	menu bookMenu;
	bookMenu.SetTitle("Books");
	//from books.txt
	//for (int i; i < books.size(); i++) {
		//add book
	//}

	LABEL_BOOK_MENU:
	while (true) {
		switch (std::stoi(listenForString())) {
			
		}
	}
}