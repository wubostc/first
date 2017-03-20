#ifndef __USER_H__
#define __USER_H__
#include <string>
#include "Session.h"
class User {
protected:
	User();
	User(const User &) {}
	User &operator=(const User &) {}
private:
	static User *instance_;
public:
	~User();
	static User *getInstance();
protected:
	std::string uid_;
	std::string pwd_;
public:
	typedef enum {
		UID, PWD
	}E_TYPE;
	
	inline const std::string &getInfo(E_TYPE)const;

	inline void setInfo(const std::string &,E_TYPE);

	inline void setSessionID(const std::string &_Session);

};

inline void User::setSessionID(const std::string &_Session) {
	Session::setID(_Session);
}

typedef User::E_TYPE TYPE_INFO;

inline const std::string &User::getInfo(TYPE_INFO type) const {
	return (type == E_TYPE::UID) ? uid_ : pwd_;
}

inline void User::setInfo(const std::string &info,E_TYPE type) {
	(type == E_TYPE::UID) ? uid_ = info : pwd_ = info;
}

#define SHARED_USERINFO User::getInstance()

#endif // __USER_H__