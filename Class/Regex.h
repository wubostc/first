#ifndef __REGEX_H__
#define __REGEX_H__
#include <regex>
#include "Config.h"

class Regex {
private:
	Regex() {}
	Regex(const Regex &) {}
	Regex &operator=(const Regex &) {}
public:
	typedef enum {
		UNKNOW, ACCOUNT, PHONE, PWD
	} E_TYPE;

public:
	inline static E_TYPE verifyUID(const char *uid);

	inline static E_TYPE verifyPWD(const char *pwd);
};

typedef Regex::E_TYPE TYPE_INPUT;

TYPE_INPUT Regex::verifyPWD(const char *pwd) {
	return std::regex_match(pwd, std::regex(REGEX_PWD)) ? PWD : UNKNOW;
}

TYPE_INPUT Regex::verifyUID(const char *uid) {
	return std::regex_match(uid, std::regex(REGEX_ACCOUNT)) ? ACCOUNT : (std::regex_match(uid, std::regex(REGEX_PHONE)) ? PHONE : UNKNOW);
}
#endif // __REGEX_H__