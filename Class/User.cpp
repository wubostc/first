#include "User.h"

User *User::instance_ = nullptr;

User *User::getInstance() {
	if (!instance_) {
		instance_ = new User;
	}
	return instance_;
}

User::User() :
uid_(""), 
pwd_("") {

}

User::~User() {
	if (instance_) {
		delete instance_;
		instance_ = nullptr;
	}
}