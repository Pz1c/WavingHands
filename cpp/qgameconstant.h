#ifndef QGAMECONSTANT
#define QGAMECONSTANT

#define SALT "(casdDasfa4242323f$CKASDf*0#2222@*$Y&_(a)-FAASS23"
#define ORGANIZATION_NAME "Steel Programming"
#define APPLICATION_NAME "WarloksDuel"
#define APPLICATION_TITLE "Warloks Duel"
#define APPLICATION_VERSION_PRINT "0.1"
#define APPLICATION_VERSION       "1"
#define APPLICATION_VERSION_INT   1
#define APPLICATION_PACKAGE "net.is.games.WarlocksDuel"
#define APPLICATION_INSTALLER_PACKAGE ""

#define GAME_SERVER_URL "https://games.ravenblack.net/"
#define GAME_SERVER_URL_LOGIN "https://games.ravenblack.net/login"
#define GAME_SERVER_URL_PLAYER "https://games.ravenblack.net/player"
#define GAME_SERVER_URL_CHALLENGES "https://games.ravenblack.net/challenges"
#define GAME_SERVER_URL_PLAYERS "https://games.ravenblack.net/players"
#define GAME_SERVER_URL_ACCEPT_CHALLENGE "https://games.ravenblack.net/accept?back=%1&num=%2"
#define GAME_SERVER_URL_REFUSE_CHALLENGE "https://games.ravenblack.net/refuse?back=player&num=%1"
#define GAME_SERVER_URL_DELLMESS "https://games.ravenblack.net/delmess?from=%1"
#define GAME_SERVER_URL_SUBMIT "https://games.ravenblack.net/warlocksubmit"
#define GAME_SERVER_URL_GET_BATTLE "https://games.ravenblack.net/warlocks?num=%1"
#define GAME_SERVER_URL_GET_PROFILE "https://games.ravenblack.net/player/%1.html"
#define GAME_SERVER_URL_NEW_CHALLENGE "https://games.ravenblack.net/newchallenge"
#define GAME_SERVER_URL_NEW_PLAYER "https://games.ravenblack.net/newplayer?action=new"
#define GAME_SERVER_URL_SENDMESS "https://games.ravenblack.net/sendmess"
#define GAME_SERVER_URL_GET_FINISHED_BATTLE "https://games.visk.in.ua/robot_gateway/wh/index.php?battle_id=%1&show_data=1"


#define intToStr QString::number
#define boolToInt(val) (val?1:0)
#define boolToIntS(val) (val?"1":"0")

#define DEBUG_MESSAGE_PATTERN "%{time} %{function} %{message}"

#include <QDebug>

#endif // QGAMECONSTANT

