#include <ESP8266WiFi.h>
#include <ESP_Mail_Client.h>
#include <WiFiUdp.h>
#include <NTPClient.h>

// WiFi Credentials
const char* ssid = "ID";
const char* password = "PASSWORD";

// SMTP Server Details
#define SMTP_HOST "smtp.gmail.com"
#define SMTP_PORT 465 // Use 587 for TLS

// Your Email & App Password
#define AUTHOR_EMAIL "-----"
#define AUTHOR_PASSWORD "----"
#define RECIPIENT_EMAIL "-----"

// NTP Client for Time & Date
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", 19800, 60000); // UTC+5:30 (IST), updates every 60 sec

// SMTP Client
SMTPSession smtp;
ESP_Mail_Session session;
SMTP_Message message;

const int buttonPin = 5; // Button to manually send email
bool buttonPressed = false;

void sendEmail() {
    Serial.println("Preparing to send email...");
    
    // Update the time
    timeClient.update();
    String formattedTime = timeClient.getFormattedTime();
    time_t epochTime = timeClient.getEpochTime();
    struct tm *timeInfo = localtime(&epochTime);
    int day = timeInfo->tm_mday;
    int month = timeInfo->tm_mon + 1;
    int year = timeInfo->tm_year + 1900;

    String emailBody = "ESP8266 Time & Date Update:\n";
    emailBody += "Time: " + formattedTime + "\n";
    emailBody += "Date: " + String(day) + "-" + String(month) + "-" + String(year) + "\n";
    
    // SMTP Server Settings
    session.server.host_name = SMTP_HOST;
    session.server.port = SMTP_PORT;
    session.login.email = AUTHOR_EMAIL;
    session.login.password = AUTHOR_PASSWORD;
    session.login.user_domain = "smtp.gmail.com";
    
    // Email Message
    message.sender.name = "ESP8266";
    message.sender.email = AUTHOR_EMAIL;
    message.subject = "ESP8266 Real-Time Clock Update";
    message.addRecipient("User", RECIPIENT_EMAIL);
    message.text.content = emailBody.c_str();
    
    // Start Sending Email
    if (!smtp.connect(&session)) {
        Serial.println("SMTP Connection Failed!");
        return;
    }

    if (!MailClient.sendMail(&smtp, &message)) {
        Serial.println("Email Sending Failed!");
    } else {
        Serial.println("Email Sent Successfully!");
    }

    smtp.closeSession();
}

void IRAM_ATTR handleButtonPress() {
    buttonPressed = true;
}

void setup() {
    Serial.begin(115200);
    pinMode(buttonPin, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(buttonPin), handleButtonPress, FALLING);
    
    // Connect to Wi-Fi
    WiFi.begin(ssid, password);
    Serial.print("Connecting to WiFi");
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("\nConnected to WiFi");
    Serial.println("IP Address: " + WiFi.localIP().toString());

    // Start NTP Client
    timeClient.begin();
}

void loop() {
    static unsigned long lastEmailTime = 0;
    unsigned long currentMillis = millis();

    // Send an email every 1 hour (3600000 ms)
    if (currentMillis - lastEmailTime >= 3600000) {
        sendEmail();
        lastEmailTime = currentMillis;
    }

    // Send an email on button press
    if (buttonPressed) {
        sendEmail();
        buttonPressed = false;
    }

    delay(1000); // Avoid excessive processing
}

