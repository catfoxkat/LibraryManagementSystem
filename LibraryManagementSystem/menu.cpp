#include<iostream>
#include<string>
#include"menu.h"

using std::cout;
using std::endl;
using std::string;

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