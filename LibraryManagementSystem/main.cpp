#include<iostream>
#include<string>
#include"menu.h"
#include"inputListener.h"
#include"accountHandler.h"

using std::cout;

accountHandler AccountHandler;
account currentUser;
bool loggedIn;

int main() {
	menu NewMenu;
	NewMenu.SetTitle("Welcome to the library of [name here]!");
	NewMenu.AppendNav("Login");
	NewMenu.AppendNav("Register");
	NewMenu.AppendNav("Browse as guest");
	NewMenu.AppendNav("Quit");
	NewMenu.DisplayPage();

	while (true) {
		switch (listenForInt()) {
			case 1:
				cout << "entering login function";
				AccountHandler.loginFunction();
				break;
			case 2:
				cout << "entering register function";
				AccountHandler.registerFunction();
				break;
			case 3:
				cout << "Not implemented :3";
				break;
			case 4:
				return 0;
			default: continue;
		}
		std::cin.ignore();
		NewMenu.DisplayPage();
	}
	
	return 0;
}