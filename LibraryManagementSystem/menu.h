#pragma once

#include<string>

using std::string;

class menu {
public:
	void SetTitle(string title);

	void AppendNav(string navText);

	void DisplayPage();
private:
	string PageTitle;
	string PageNav = "";
	int PageNavIndex = 1;
};