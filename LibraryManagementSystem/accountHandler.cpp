#include<iostream>
#include<string>
#include"inputListener.h"
#include"accountHandler.h"
#include"fileHandler.h"

using std::cout;
using std::endl;
using std::string;

void registerAccount(string username, string password) {

}

bool validateAccountLogin(string username, string password) {
	return false;
}

void accountHandler::registerFunction() {
	system("cls");

	//inputusr:
	cout << "Input username\n"; //probably add requirements like "Must be x chars, no symbols"
	string username = listenForString(); //add check for username conflicts
	//goto inputusr;

	inputpwd:
	cout << "Input password\n";
	string password = listenForString();
	cout << "Input confirm password\n";
	string confirmPassword = listenForString();

	if (!(password == confirmPassword)) { cout << "Passwords do not match!\n\n"; goto inputpwd; }

	cout << "REGISTER ACCOUNT WITH USERNAME OF " << username << " AND PASSWORD OF " << password << endl;
	fileHandler().appendNewAccount(username, password);
}

void accountHandler::loginFunction() {
	if (!promptYesNo("Continue to login? [y/n]\n")) {
		return;
	}

	system("cls");

	cout << "Input username\n";
	string username = listenForString();

	cout << "Input password\n";
	string password = listenForString();

	cout << "LOGIN WITH USERNAME OF " << username << " AND PASSWORD OF " << password << endl;
	validateAccountLogin(username, password);
}