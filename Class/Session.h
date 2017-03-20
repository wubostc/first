#ifndef __SESSION_H__
#define __SESSION_H__
#include<string>
/*
���ڹ���ȫ��Ψһ��session

@author	ub
@since	2016/2/12
*/
class Session {
private:
	// �û���ʶ��
	static std::string session_;

private:
	Session() = delete;
	Session(const Session &) = delete;
	Session &operator=(const Session &) = delete;

public:
	/*
	����session
	
	@return
	\��ǰ��session_
	@since	2016/2/15
	*/
	inline static const std::string &getID() {
		return session_;
	}

	/*
	����session

	@param
	\session�ַ���
	@since	2016/2/15
	*/
	inline static void setID(const std::string &id) {
		session_ = id;
	}
};

#endif //__SESSION_H__