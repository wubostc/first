#ifndef __FILEMANAGER_H__
#define __FILEMANAGER_H__
#include <CCFileUtils.h>
//#include <iconv.h>
#include "Config.h"

/*
Singleton of FileUtils
@return singleton
*/
#define SHARED_FILE_UTILS \
	cocos2d::FileUtils::getInstance()

/*
.plist�ļ����·��
@return "user.plist"
*/
#define USERINFO_PLISTFILE	\
	SHARED_FILE_UTILS->getWritablePath() + FILE_PLIST_USERINFO

/*
@return writeable path
*/
#define WRITEABLE_PATH \
	SHARED_FILE_UTILS->getWritablePath()


/*
д���û��������뵽��д·����
e.g.
\<plist version="1.0">
\	<dict>
\		<key>user_info</key>
\		<dict>
\			<key>username</key>
\			<string>12345678912</string>
\			<key>password</key>
\			<string>123456aaa</string>
\			<key>score</key>
<integer>1000</integer>
\		</dict>
\	</dict>
\</plist>
@param
\char *username
\char *passwodr
\char *filename
*/
//#define WRITE_USERINFO_TO_FILE(username,password,filename)	\
//	cocos2d::ValueMap root;										\
//	cocos2d::ValueMap user_info;								\
//	user_info[USERINFO_KEY_UID] = username;						\
//	user_info[USERINFO_KEY_PWD] = password;						\
//	root[USERINFO_KEYROOT_1] = cocos2d::Value(user_info);		\
//	SHARED_FILE_UTILS->writeValueMapToFile(root, filename);



class FileQ {
private:
	FileQ() {}
	FileQ(const FileQ &) {}
	FileQ &operator=(const FileQ&) {}

public:

	/*
	����key����ȡvalue
	note:�ļ������ڷ��� ""
	@param
	\����USERINFO_KEY_XXXX��ʽ����ȷ��key
	@return
	\string val
	*/
	static std::string read(const char *key) {
		std::string val("");

		if (!SHARED_FILE_UTILS->isFileExist(USERINFO_PLISTFILE)) {
			return val;
		}

		cocos2d::ValueMap read = SHARED_FILE_UTILS->getValueMapFromFile(USERINFO_PLISTFILE);
		cocos2d::ValueMap value_map = read[KEYROOT_USERINFO_1].asValueMap();
		val = value_map[key].asString();
		return val;
	}

	/*
	д���û��������뵽��д·���£��ļ���Ϊuser.plist
	@param
	\char *username
	\char *passwodr
	@return
	\true
	\false
	*/
	static bool wirte(const char *uid,const char *pwd) {
		cocos2d::ValueMap root;			
		cocos2d::ValueMap user_info;	
		user_info[KEY_USERINFO_UID] = uid; 
		user_info[KEY_USERINFO_PWD] = pwd; 
		root[KEYROOT_USERINFO_1] = cocos2d::Value(user_info); 
		return SHARED_FILE_UTILS->writeValueMapToFile(root, USERINFO_PLISTFILE); 
	}
};


#endif //__FILEMANAGER_H__