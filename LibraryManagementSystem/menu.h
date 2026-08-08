#pragma once

#include<string>

class menu {
public:
	void SetTitle(std::string title);

	void SetDescription(std::string title);

	void AppendNav(std::string navText, int overwriteIndex = -1);

	void DisplayPage();
private:
	std::string PageTitle;
	std::string PageDesc;
	std::string PageNav = "";
	int PageNavIndex = 1;
};