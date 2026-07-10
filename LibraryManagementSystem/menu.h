#pragma once

#include<string>

class menu {
public:
	void SetTitle(std::string title);

	void AppendNav(std::string navText);

	void DisplayPage();
private:
	std::string PageTitle;
	std::string PageNav = "";
	int PageNavIndex = 1;
};