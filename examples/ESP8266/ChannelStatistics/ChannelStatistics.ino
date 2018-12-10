/*____________________________⚙_P+E+M_⚙______________________________\
  | Об устройстве: Монитор ютуба   Ver. 0.1.1                      |
  | By Programming + Electronics + Mechanics   (feat. Brian Lough)                        |
  | Info: vk.com/@pr_el_mh-schetchik-podpischikov-yotube-kanala        |                             |
  \___________________________⚙_P+E+M_⚙______________________________*/

//--------------------Инициализируем библиотеки--------------------\\

#include <YoutubeApi.h>   // Работа с API ютуба. Скачать тут: https://github.com/witnessmenow/arduino-youtube-api
#include <ArduinoJson.h>  //Эта библиотека не используется, но должна быть установлена. https://github.com/bblanchon/ArduinoJson
#include <ESP8266WiFi.h>  // Работа с ESP826. Необходимо добавить плату в IDE. Как это сделать: https://habr.com/post/371853/
#include <WiFiClientSecure.h>   //  Работа с ESP8266


//--------------------Настраиваемые параметры---------------------\\

char ssid[] = "name";       // Название точки доступа (роутера)
char password[] = "1234567890";  // Пароль от роутера
#define CHANNEL_ID "UCEjAPx6xgJl85HDCtOoGmUw" // указать свой ID канала. Можно скопировать из адресной строки или узнать в своем кабинете.
#define VIDEO_ID "mxnTXNwZzlo" // Id видео
#define API_KEY "*****"   // Ключ API от Yotube. Инструкция: https://www.slickremix.com/docs/get-api-key-for-youtube/  
//_______________________________________________________________\\

WiFiClientSecure client;
YoutubeApi api(API_KEY, client);

unsigned long api_mtbs = 10000; //  Среднее время между запросами api
unsigned long api_lasttime;   //  Время последнего запроса API


void setup() {
  Serial.begin(115200); //  Устанавливаем скорость с COM портом

  WiFi.mode(WIFI_STA);  //  Устанавливаем ESP8266 в режиме станции
  WiFi.disconnect();  //  Разрываем соединение, если оно было установленно ранее
  delay(100);

  //___________________Подключаемся к точке доступа_____________________\\

  Serial.print("Connecting Wifi: "); // Выводим сообщенеие о попытке подключения к точке доступа
  Serial.println(ssid);  // Имя точки доступа
  WiFi.begin(ssid, password); // Подключаемся
  while (WiFi.status() != WL_CONNECTED) { //  Если подключение не произошло выводим точку и пытаемся ещё
    Serial.print(".");
    delay(500);
  }
  Serial.println("");
  Serial.println("WiFi connected"); //  Подключено
  Serial.println("IP address: ");
  IPAddress ip = WiFi.localIP();  //  Узнаем IP адрес точки
  Serial.println(ip); //  Выводим IP адрес точки
}

void loop() {

  if (millis() - api_lasttime > api_mtbs)  {  // От времени с начала работы программы отнимаем время полного цикла и сверяем со средним временем между запросов API

    // Статистика канала
    if (api.getChannelStatistics(CHANNEL_ID))
    {
      Serial.println("------Channel Stats------");

      // Количество подписчиков
       Serial.print("Subscriber: ");
      Serial.println(api.channelStats.subscriberCount);

      //  Количество просмотров
      Serial.print("View: ");
      Serial.println(api.channelStats.viewCount);

  /*    //  Количество комментариев. Не отображается. Раздел устарел
      Serial.print("Comment: ");
      Serial.println(api.channelStats.commentCount);

      // Указывает доступность отображения количества подписчиков
      Serial.print("hiddenSubscriber: ");
      Serial.println(api.channelStats.hiddenSubscriberCount);*/

      // Количество доступных (не скрытых) видео
      Serial.print("Video: ");
      Serial.println(api.channelStats.videoCount);
    }

    // Статистика 1 видео
    if (api.getVideoStatistics(VIDEO_ID))
    {
      Serial.println("------Video Stats------");
      
      //  Количество просмотров
      Serial.print("View Count: ");
      Serial.println(api.videoStats.oneviewCount);

      // Указывает доступность отображения количества подписчиков
      Serial.print("like Count: ");
      Serial.println(api.videoStats.likeCount);

      // Количество доступных (не скрытых) видео
      Serial.print("dislike Count: ");
      Serial.println(api.videoStats.dislikeCount);

      //  Количество комментариев
      Serial.print("Comment: ");
      Serial.println(api.videoStats.onecommentCount);

      Serial.println(" ");
    }
    api_lasttime = millis();
  }
}
