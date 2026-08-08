#ifndef ACCOUNTHANDLER_H_
#define ACCOUNTHANDLER_H_

#include<string>

struct account {
	std::string username;
	int permissionLevel = 0; //-1 guest, 0 user, 1 staff
	bool exists = false;
};

class accountHandler {
public:
	void registerFunction();

	void loginFunction(account* currentUser);
};

#endif // ACCOUNTHANDLER_H_