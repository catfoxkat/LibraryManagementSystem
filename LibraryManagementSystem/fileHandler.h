#ifndef FILEHANDLER_H_
#define FILEHANDLER_H_

#include<vector>
#include"accountHandler.h"
#include"bookHandler.h"

class fileHandler {
	public:
		void initializeFiles();

		void appendNewAccount(std::string username, std::string password);

		void appendNewBook(book newBook);

		account getAccountByName(std::string username);

		bool validateAccount(std::string username, std::string password);

		std::vector<std::string> getBookNames();


};

#endif // FILEHANDLER_H_