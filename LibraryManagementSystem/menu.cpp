#include<iostream>
#include<string>
#include"menu.h"

using std::cout;
using std::endl;
using std::string;

void menu::SetTitle(string title) {
	PageTitle = title;
}

void menu::AppendNav(string navText) {
	PageNav.append("[" + std::to_string(PageNavIndex) + "] " + navText + "\n");
	PageNavIndex++;
}

void menu::DisplayPage() {
	system("cls");
	cout << PageTitle << endl << PageNav << endl;
}