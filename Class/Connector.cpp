#include <cocos2d.h>
#include <curl.h>
#include "Connector.h"
#include "JSON.h"
#include "Session.h"
#include "User.h"
#include "Config.h"

size_t Connector::write_callback(char *ptr, size_t size, size_t nmemb, void *userdata) {
#ifdef  DEBUG_FIRST
	CCLOG("%s", "```");
	CCLOG("%s", ptr);
	CCLOG("%s", "```");
#endif // DEBUG_FIRST

	// php echo: "\r\n\r\n{...}"，normal
	if (!JSON::pares(ptr + 4)) {
		CCLOG("%s: %s", __FILE__, __FUNCTION__);
	}

	return size * nmemb;
}

size_t Connector::header_callback(char *buffer, size_t size, size_t nitems, void *userdata) {
	/* received header is nitems * size long in 'buffer' NOT ZERO TERMINATED */
	/* 'userdata' is set with CURLOPT_HEADERDATA */
#ifdef  DEBUG_FIRST
	CCLOG("%s", buffer);
#endif // (DEBUG_FIRST == 1)
	std::string tmp(buffer);
	auto pos = tmp.find(HEADER_SESSION_KEY);
	if (pos != std::string::npos && pos == 0) {
		size_t pos = tmp.find_first_of(":");
		std::string tmp2 = tmp.substr(pos + 2, 26);
		Session::setID(tmp2);// length of string is 26
	}

	return nitems * size;
}

bool Connector::post(const std::string &post_fields) {

	CURLcode res;
	struct curl_slist *list = NULL;
	CURL *curl = curl_easy_init();

	CCASSERT(curl, "curl == NULL");

	curl_easy_setopt(curl, CURLOPT_URL, SER_HOST(URL_SIGN_ON).c_str());
	curl_easy_setopt(curl, CURLOPT_POST, true);
	curl_easy_setopt(curl, CURLOPT_POSTFIELDS, post_fields.c_str());
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
	curl_easy_setopt(curl, CURLOPT_HEADERFUNCTION, header_callback);
	curl_easy_setopt(curl, CURLOPT_TIMEOUT, 5L);

	std::string header_session_id(HEADER_SESSION_KEY + Session::getID());
	list = curl_slist_append(list, header_session_id.c_str());
	curl_easy_setopt(curl, CURLOPT_HTTPHEADER, list);

	res = curl_easy_perform(curl);
	if (res != CURLE_OK) {
#ifdef  DEBUG_FIRST
		CCLOG("timeout %i", res);
#endif // DEBUG_FIRST
		curl_easy_cleanup(curl);
		curl_slist_free_all(list);
		return false;
	}
	curl_easy_cleanup(curl);
	curl_slist_free_all(list);
	// good
	return true;
}


MyConnector *MyConnector::instance_ = nullptr;
MyConnector *MyConnector::getInstance() {
	if (!instance_) {
		instance_ = new MyConnector;
		instance_->autorelease();
		curl_global_init(CURL_GLOBAL_ALL);
	}
	return instance_;
}


MyConnector::MyConnector() :
user_(SHARED_USERINFO) {
	
}

MyConnector::~MyConnector() {
	if (instance_) {
		curl_global_cleanup();
		delete instance_;
		instance_ = nullptr;
		user_->~User();
	}
}


bool MyConnector::upScore(size_t score) {
	std::string post;
	//std::string &strscore = std::to_string(score);
	post =
		TODO + "=" + UP_SCORE + "&" +
		USERNAME + "=" + user_->getInfo(TYPE_INFO::UID) + "&" +
		PASSWORD + "=" + user_->getInfo(TYPE_INFO::PWD) + "&" +
		SCORE + "=";
	post += cocos2d::StringUtils::toString<size_t>(score);

	return this->post(post);
}

bool MyConnector::login(const std::string &username, const std::string &password) {
	std::string data;
	data =
		TODO + "=" + LOGIN + "&" +
		USERNAME + "=" + username + "&" +
		PASSWORD + "=" + password;

	user_->setSessionID(HEADER_LOGIN_VAL);
	return post(data);
}

bool MyConnector::signOn(const std::string &username, const std::string &password) {
	std::string data;
	data = 
		TODO + "=" + SIGN_ON + "&" + 
		USERNAME + "=" + username + "&" + 
		PASSWORD + "=" + password;
	user_->setSessionID(HEADER_SIGN_ON_VAL);
	return post(data);
}

bool MyConnector::getHighestScoreFromServer(const std::string &username,const std::string &password) {
	std::string data;
	data =
		TODO + "=" + SEEK_SCORE + "&" +
		USERNAME + "=" + username + "&" +
		PASSWORD + "=" + password;

	return post(data);
}


