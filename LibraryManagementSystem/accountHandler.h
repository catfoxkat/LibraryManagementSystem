#pragma once

#include<string>

struct account {
	std::string username;
	int permissionLevel = 0; //0 user, 1 staff
	bool exists = false;
};

class accountHandler {
public:
	void registerFunction();

	void loginFunction();
};