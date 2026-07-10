#pragma once

#include"accountHandler.h"

class fileHandler {
	public:
		void initializeFiles();

		void appendNewAccount(std::string username, std::string password);

		account getAccount(std::string);
};