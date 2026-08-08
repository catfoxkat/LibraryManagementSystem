#include<iostream>
#include<string>
#include"menu.h"
#include"inputListener.h"
#include"accountHandler.h"
#include"fileHandler.h"
#include"bookHandler.h"

using std::cout;
using std::endl;

accountHandler AccountHandler;
account currentUser;
bool guest;

bookHandler BookHandler;

int main() {
	fileHandler().initializeFiles();

	std::cin.ignore();

	LABEL_START_MENU:
	menu Menu_Main;
	Menu_Main.SetTitle("Welcome to the library of [name here]!");
	Menu_Main.AppendNav("Login");
	Menu_Main.AppendNav("Register");
	Menu_Main.AppendNav("Browse as guest");
	Menu_Main.AppendNav("Quit");
	Menu_Main.DisplayPage();

	while (true) {
		switch (listenForInt()) {
			case 1:
				cout << "entering login function";
				AccountHandler.loginFunction(&currentUser);
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
				BookHandler.openInterface();
				break;

			default: continue;
		}
		cout << "left case";
		Menu_1.DisplayPage();
	}

	return 0;
}