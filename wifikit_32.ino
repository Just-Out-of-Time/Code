#include <heltec.h>
#if defined(ESP32)
  #include <WiFi.h>
#endif
#include <ESP_Mail_Client.h>

/* temperatureReadingDevice@gmail.com
 * WSU2021!
 * 
 */

#define SMTP_HOST "smtp.gmail.com"
#define SMTP_PORT 587
#define AUTHOR_EMAIL "temperaturereadingdevice@gmail.com"
#define AUTHOR_PASSWORD "WSU2021!"
#define RECIPIENT_EMAIL "6204417334@vtext.com"

const char ssid[]     = "_&$";
const char password[] = "P0mk7AsD45Z*";

SMTPSession smtp;


void initWiFi(){ 
  WiFi.disconnect();
  WiFi.mode(WIFI_STA);
  
  Serial.println("\nConnecting to: ");
  Serial.println(ssid);  

  WiFi.begin(ssid, password);
  
  while(WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("\nStill connecting...");
  }
  Heltec.display->clear();
  Heltec.display->drawString(0, 0, "Connected to: ");
  Heltec.display->drawString(0, 10, WiFi.SSID());
  //Heltec.display->drawString(0, 20, "with IP: ");
  //Heltec.display->drawString(0, 30, (String) WiFi.localIP());
  Heltec.display->display();
  
  Serial.println("\nDevice IP address: ");
  Serial.println(WiFi.localIP());
}

void sendNotification() {
  smtp.debug(1);
  /* Set the callback function to get the sending results */
  smtp.callback(smtpCallback);

  /* Declare the session config data */
  ESP_Mail_Session session;

  /* Set the session config */
  session.server.host_name = SMTP_HOST;
  session.server.port = SMTP_PORT;
  session.login.email = AUTHOR_EMAIL;
  session.login.password = AUTHOR_PASSWORD;
  session.login.user_domain = "";

  /* Declare the message class */
  SMTP_Message message;

  message.sender.name = "temperatureReadingDevice";
  message.sender.email = AUTHOR_EMAIL;
  String textMsg = "Hi mom, this is a test text from the arduino for the temperature project.";
  message.text.content = textMsg.c_str();
  //message.subject = "ESP automatic test";
  message.addRecipient("Test", RECIPIENT_EMAIL);

  if (!smtp.connect(&session))
    return;

  /* Start sending Email and close the session */
  if (!MailClient.sendMail(&smtp, &message))
    Serial.println("Error sending Email, " + smtp.errorReason());
}

void smtpCallback(SMTP_Status status){
  /* Print the current status */
  Serial.println(status.info());

  /* Print the sending result */
  if (status.success()){
    Serial.println("----------------");
    ESP_MAIL_PRINTF("Message sent success: %d\n", status.completedCount());
    ESP_MAIL_PRINTF("Message sent failled: %d\n", status.failedCount());
    Serial.println("----------------\n");
    struct tm dt;

    for (size_t i = 0; i < smtp.sendingResult.size(); i++){
      /* Get the result item */
      SMTP_Result result = smtp.sendingResult.getItem(i);
      time_t ts = (time_t)result.timestamp;
      localtime_r(&ts, &dt);

      ESP_MAIL_PRINTF("Message No: %d\n", i + 1);
      ESP_MAIL_PRINTF("Status: %s\n", result.completed ? "success" : "failed");
      ESP_MAIL_PRINTF("Date/Time: %d/%d/%d %d:%d:%d\n", dt.tm_year + 1900, dt.tm_mon + 1, dt.tm_mday, dt.tm_hour, dt.tm_min, dt.tm_sec);
      ESP_MAIL_PRINTF("Recipient: %s\n", result.recipients);
      ESP_MAIL_PRINTF("Subject: %s\n", result.subject);
    }
    Serial.println("----------------\n");
  }
}
void setup() {
  Serial.begin(115200);
  Heltec.begin(true /*DisplayEnable Enable*/, false /*LoRa Disable*/, true /*Serial Enable*/, true /*PABOOST Enable*/, 470E6 /**/);
  initWiFi();
  sendNotification();
  
}

void loop() {
  /*
   * The code here with the sensor added will be something like this. 
   * Continuously check temperature
   * if the temperature is out of range:
   * call initWiFi
   * if Wifi fails, print to screen that it cannot connect
   * call sendNotification
   * 
   */
  
}
