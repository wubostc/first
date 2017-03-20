#ifndef  _Inkmoo_Elimination_h_
#define  _Inkmoo_Elimination_h_

// ������Ļ��ߣ�����������ͼƬ�й�
#define GAME_SCREEN_WIDTH	480
#define GAME_SCREEN_HEIGHT	800

// ����ÿ�������С��߿��С
#define SPRITE_WIDTH		48
#define BOADER_WIDTH		2

// ��Ϸ��������������
#define ROWS				10
#define COLS				8

// ������������
#define TOTAL_SPRITE		6

// singleton of UserDefault
#define SHARED_USERDEFAULT	UserDefault::getInstance()

// �������֣���ʼ����
#define KEY_MUSIC_BG1		"music_welcome"

// �������֣���Ϸ����ʱ
#define KEY_MUSIC_BG2		"music_play_game"

// �������֣���Ϸ����ʱ
#define KEY_MUSIC_BG3		"music_game_over"


// �������ʾģʽ��Hor ��������һ�У�Ver��������һ��
enum DisplayMode{
    DISPLAY_MODE_NORMAL = 0,
    DISPLAY_MODE_HORIZONTAL,
    DISPLAY_MODE_VERTICAL,
};

// ��ͨ�ľ���
static const char *spriteNormal[TOTAL_SPRITE] = {
	"icon1.png",
	"icon2.png",
	"icon3.png",
	"icon4.png",
	"icon5.png",
    "icon6.png"
};

// ��ֱ
static const char *sprVertical[TOTAL_SPRITE] = {
	"icon1_z.png",
	"icon2_z.png",
	"icon3_z.png",
	"icon4_z.png",
	"icon5_z.png",
    "icon6_z.png"
};

// ˮƽ
static const char *sprHorizontal[TOTAL_SPRITE] = {
	"icon1_h.png",
	"icon2_h.png",
	"icon3_h.png",
	"icon4_h.png",
	"icon5_h.png",
    "icon6_h.png"
};


// ����

#define SCENE_START			"scene_sta.png"
#define SCENE_END			"scene_sta.png"
#define	SCENE_BG1			"scene_bg.png"

// ��ť

#define	BTN_START_1A		"btn_start01.png"
#define BTN_START_1B		"btn_start02.png"

#define	BTN_BACK_1A			"btn_back01.png"
#define	BTN_BACK_1B			"btn_back02.png"

// �������֣���ʼ����
#define	MUSIC_BG1			"music/music_mainScene.mp3"

// �������֣���Ϸ����ʱ
#define MUSIC_BG2			"music/music_bg.mp3"

// �������֣���Ϸ����
#define MUSIC_BG3			"music/music_gameOver.mp3"

// 
#define MUSIC_WIN			"music/music_win.wav"

// 
#define MUSIC_FAI			"music/music_fail.mp3"

// �Ƴ�����
#define MUSIC_EXPLOAD		"music/music_explode.wav"

// �Ƴ�����
#define MUSIC_EXPLOAD_ONE	"music/music_explodeOne.wav"


// plist�ļ�:icon
#define	PLIST_1				"icon.plist"
// plist�ļ����ͼƬ:icon
#define ICON_1				"icon.png"


//-------------UserDefault----------------
// UserDefault key:score
#define KEY_SCORE			"highest_score"
//-------------UserDefault----------------

#include <SimpleAudioEngine.h>
#define SHARED_AUDIO		SimpleAudioEngine::getInstance()


#define TAG_1				10
#define TAG_2				13

// ����
#define TAG_3				1000
#endif