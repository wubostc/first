#ifndef __GARBO_H__
#define	__GARBO_H__
#include <vector>
class MyConnector;
class User;
class Garbo {
public:
	virtual ~Garbo();
protected:
	Garbo();
protected:
	virtual void autorelease();
private:
	std::vector<Garbo *> pool_;
	static Garbo garbo_;
};

#endif // __REFQ_H__
