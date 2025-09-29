#ifndef QGACONSTANT_H
#define QGACONSTANT_H
#define GA_ENDPOINT_URL_BASE "https://www.google-analytics.com/mp/collect?api_secret=%1&"
#define GA_ENDPOINT_URL_PAR1 "measurement_id=%2"
#define GA_ENDPOINT_URL_PAR2 "firebase_app_id=%2"
#define GA_ENDPOINT_URL GA_ENDPOINT_URL_BASE GA_ENDPOINT_URL_PAR1

#define GA_APP_ID_TAG_NAME1 "client_id"
#define GA_APP_ID_TAG_NAME2 "app_instance_id"
#define GA_APP_ID_TAG_NAME GA_APP_ID_TAG_NAME1

#define GA_SENDING_FREQUENCY 5000

#define GA_ITEM_TYPE_NONE 0
#define GA_ITEM_TYPE_EVENT 1
#define GA_ITEM_TYPE_PAGEVIEW 2
#define GA_ITEM_TYPE_SCREENVIEW 3

#define GA_ITEM_STATUS_NONE 0
#define GA_ITEM_STATUS_SENDING 1
#define GA_ITEM_STATUS_SEND 2

#define GA_PAUSE_TIME_DEF 5

#endif
