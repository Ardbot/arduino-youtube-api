/*____________________________⚙_P+E+M_⚙______________________________\
  | Об устройстве: Счётчик для видео                                   |
  | By Programming + Electronics + Mechanics   (feat. Brian Lough      |
  | Info:                                                              |
  \___________________________⚙_P+E+M_⚙______________________________*/

//--------------------Инициализируем библиотеки--------------------\\

#include <YoutubeApi.h>   // Работа с API ютуба. Скачать тут: https://github.com/witnessmenow/arduino-youtube-api
#include <ArduinoJson.h>  //Эта библиотека не используется, но должна быть установлена. https://github.com/bblanchon/ArduinoJson
#include <ESP8266WiFi.h>  // Работа с ESP826. Необходимо добавить плату в IDE. Как это сделать: https://habr.com/post/371853/
#include <WiFiClientSecure.h>   //  Работа с ESP8266


//--------------------Настраиваемые параметры---------------------\\

char ssid[] = "name";       // Название точки доступа (роутера)
char password[] = "****";  // Пароль от роутера
#define CHANNEL_ID "mxnTXNwZzlo" //  ID video.
#define API_KEY "zzzz"   // Ключ API от Yotube. Инструкция: https://www.slickremix.com/docs/get-api-key-for-youtube/ 
//_______________________________________________________________\\

WiFiClientSecure client;
YoutubeApi api(API_KEY, client);

unsigned long api_mtbs = 30000; //  Среднее время между запросами api
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
    if (api.getChannelStatistics(CHANNEL_ID))
    {
    
      //  Количество просмотров 
      Serial.print("View Count: ");
      Serial.println(api.channelStats.viewCount);

      //  Количество комментариев 
      Serial.print("Comment: ");
      Serial.println(api.channelStats.commentCount);
      
      Serial.print("like: ");
      Serial.println(api.channelStats.hiddenSubscriberCount);
      
      Serial.print("dislike: ");
      Serial.println(api.channelStats.videoCount);
      
      Serial.println("----------------");

    }
    api_lasttime = millis();



  }
}
