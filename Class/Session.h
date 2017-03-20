#ifndef __SESSION_H__
#define __SESSION_H__
#include<string>
/*
用于管理全局唯一的session

@author	ub
@since	2016/2/12
*/
class Session {
private:
	// 用户标识符
	static std::string session_;

private:
	Session() = delete;
	Session(const Session &) = delete;
	Session &operator=(const Session &) = delete;

public:
	/*
	返回session
	
	@return
	\当前的session_
	@since	2016/2/15
	*/
	inline static const std::string &getID() {
		return session_;
	}

	/*
	更新session

	@param
	\session字符串
	@since	2016/2/15
	*/
	inline static void setID(const std::string &id) {
		session_ = id;
	}
};

#endif //__SESSION_H__