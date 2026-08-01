#pragma once

#include"accountHandler.h"

class fileHandler {
	public:
		void initializeFiles();

		void appendNewAccount(std::string username, std::string password);

		account getAccountByName(std::string username);

		bool validateAccount(std::string username, std::string password);
};