#include"inputListener.h"
#include"fileHandler.h"
#include"menu.h"

class bookHandler {
public:
	void openInterface();

private:
	account currentUser;
	bool guest;
};

class book {
public:
	std::string title;
	std::string author;
	std::string description;
	std::string genre;
	std::string language;
	std::string ISBN;

	int pageCount;
	int available;
	int borrowed;
};