#ifndef __JSON_H__
#define	__JSON_H__
#include <json\document.h>
#include <json\error\en.h>
#include <cstdlib>
#include "Config.h"
#include "Garbo.h"
#ifdef  DEBUG_FIRST
#include <cocos2d.h>
#endif


/*
管理全局唯一的json格式字符串

@author	ub
@since	2016/2/15
*/
class JSON : public Garbo {
private:
	JSON() {
#ifdef  DEBUG_FIRST
		CCLOG("%s", __FUNCTION__);
#endif
		this->autorelease();
	}
	JSON(const JSON &) = delete;
	JSON &operator=(const JSON &) = delete;
public:
	~JSON();
public:
	/*
	解析一个符合json格式标准的字符串

	@param	json格式的字符串
	@return
	\true	解析成功
	\false	解析异常
	@since	2016/2/14
	*/
	inline static bool pares(char *json);

	/*
	获得json里的code

	@return	
	\0	操作失败
	\1	操作成功
	@since	2016/2/14
	*/
	inline static int getCode(); 

	/*
	获得json里的message

	@return	
	\字符串或空的字符串
	@since	2016/2/14
	*/
	inline static const char *getMessage();

	/**/
	inline static size_t getScore();

	/**/
	inline static std::string getNickname();

private:
	// DOM树
	static rapidjson::Document document;
	static char *json_;
	
	static bool isJSON_;


public:
	inline static bool isJSON();

	inline static char *getString();
};

inline char *JSON::getString() {
	return json_;
}

inline bool JSON::isJSON() {
	return isJSON_;
}

inline bool JSON::pares(char *json) {
	
	if (json_)free(json_);
	json_ = (char *)malloc(strlen(json) + 1);
	strcpy(json_, json);

	// note:！
	isJSON_ = !document.ParseInsitu(json_).HasParseError();
	
#ifdef  DEBUG_FIRST
	if (!isJSON_) {
		CCLOG("Error offset: %s", document.GetErrorOffset());
		CCLOG("%s", GetParseError_En(document.GetParseError()));
	}
#endif

	return isJSON_;
}

inline int JSON::getCode() {
	return document.HasMember(JSON_CODE) ? document[JSON_CODE].GetInt() : CODE_FATAL;
}

inline const char *JSON::getMessage() {
	return document.HasMember(JSON_MSG) ? document[JSON_MSG].GetString() : MSG_FATAL.c_str();
}

inline size_t JSON::getScore() {
	return document[JSON_EXTE][JSON_SCORE].GetUint();
}

inline std::string JSON::getNickname() {
	return document[JSON_EXTE][JSON_NICKNAME].GetString();
}


#endif //__JSON_H__