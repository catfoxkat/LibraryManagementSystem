#pragma once

#include<string>

using std::string;

struct account {
	string username;
	int permissionLevel; //0 user, 1 staff
};

class accountHandler {
public:
	void registerFunction();

	void loginFunction();
};