#ifndef __CONNECTOR_H__
#define __CONNECTOR_H__

#include <string>
#include <cocos2d.h>
#include "Garbo.h"

class Connector :protected Garbo {
public:
	virtual ~Connector() {}
protected:
	Connector() {}
private:
	Connector(const Connector &) {}
	Connector &operator=(const Connector) {}
protected:
	virtual bool post(const std::string &post_fields);

private:
	// http
	static size_t header_callback(char *buffer, size_t size, size_t nitems, void *userdata);

	// http
	static size_t write_callback(char *ptr, size_t size, size_t nmemb, void *userdata);

};

class User;

class MyConnector : public Connector {
private:
	static MyConnector *instance_;
protected:
	MyConnector();
private:
	MyConnector(const MyConnector &) {}
	MyConnector &operator=(const MyConnector &) {}
public:
	~MyConnector();
	static MyConnector *getInstance();
public:
	/*
	ע��ӿ�

	@param	
	\������11λ�ֻ��Ż�6-11λ���û���
	\6-18λ����
	@return	
	\true	�����ɹ�
	\false	����ʧ��
	@since	2016/2/14
	*/
	bool login(const std::string &username, const std::string &password);

	/*
	��¼�ӿ�

	@param
	\������11λ�ֻ��Ż�6-11λ���û���
	\6-18λ����
	@return
	\true	�����ɹ�
	\false	����ʧ��
	@since	2016/2/14
	*/
	bool signOn(const std::string &username, const std::string &password);

	/*
	�ύ����

	@param
	\������11λ�ֻ��Ż�6-11λ���û���
	\6-18λ����
	\����0����ֵ
	@return
	\true	�����ɹ�
	\false	����ʧ��
	@since	2016/2/15
	*/
	bool upScore(size_t score);

	/*
	�õ���߷�

	@param
	\������11λ�ֻ��Ż�6-11λ���û���
	\6-18λ����
	\����0����ֵ
	@return
	\true	�����ɹ�
	\false	����ʧ��
	@since	2016/2/20
	*/
	bool getHighestScoreFromServer(const std::string &username, const std::string &password);

private:
	User *user_;
};




#endif //__CONNECTOR_H__