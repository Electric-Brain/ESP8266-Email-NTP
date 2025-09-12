# 📧 ESP8266 Email Notification System with Internet-Synced Date and Time

This project shows how to use the **ESP8266 microcontroller** with Gmail’s **Simple Mail Transfer Protocol (SMTP)** to automatically send emails that include real-time date and time information. The system connects to an **NTP (Network Time Protocol) server** to get accurate clock and calendar data, and then sends that information through email at regular intervals.

---

## ✨ Features

* Connects to Wi-Fi networks (works with both routers and mobile hotspots).
* Uses **NTP servers** for accurate date and time updates.
* Sends secure emails through **Gmail’s SMTP service**.
* Emails include both **time** and **date** information.
* Runs on its own, sending emails at set intervals without manual input.

---

## 📦 Requirements

### Hardware

* ESP8266 board (e.g., NodeMCU, Wemos D1 Mini)
* USB cable

### Software

* [Arduino IDE](https://www.arduino.cc/en/software)
* ESP8266 board support installed through Arduino IDE Board Manager
* Required libraries:

  * `ESP8266WiFi.h`
  * `WiFiUdp.h`
  * `NTPClient.h`
  * `ESP8266SMTP.h`

You can install these libraries using the **Arduino Library Manager** (`Sketch → Include Library → Manage Libraries`).

---

## ⚙️ Setup Steps

1. Open the provided Arduino sketch in the Arduino IDE.
2. Update the Wi-Fi and email details in the code:

   ```cpp
   const char* ssid = "Your_Hotspot_Name";
   const char* password = "Your_Hotspot_Password";

   const char* emailSender = "your_email@gmail.com";
   const char* emailPassword = "your_app_password"; // Use Gmail App Password
   const char* emailRecipient = "recipient_email@gmail.com";
   ```
3. Turn on **two-factor authentication** in Gmail and generate an **App Password**. Use that password instead of your normal Gmail password.
4. Upload the sketch to the ESP8266 using the USB cable.

---

## ▶️ How It Works

* When powered on, the ESP8266 connects to Wi-Fi.
* The **Serial Monitor** (baud rate: `115200`) shows connection details and current time/date.
* Every 60 seconds, the ESP8266:

  * Gets the updated time from the NTP server.
  * Displays the time and date on the Serial Monitor.
  * Sends an email with the time and date information.

---

## 🖼️ Example Serial Monitor Output

```
Connecting to WiFi....
Connected to WiFi
IP Address: 192.168.1.45
Time: 14:23:05 | Date: 12-09-2025
Email sent successfully!
```

---

## 📌 Important Notes

* Gmail will only accept an **App Password**, not your normal account password.
* The code uses **Indian Standard Time (UTC+5:30)** by default:

  ```cpp
  NTPClient timeClient(ntpUDP, "pool.ntp.org", 19800, 60000);
  ```

  Here, `19800` is the time zone offset in seconds. You can change it for your region.
* The default email interval is **60 seconds**, which you can adjust in the code:

  ```cpp
  delay(60000);
  ```

---

## 🚀 Possible Improvements

* Add a physical button to trigger sending emails manually.
* Send emails to multiple recipients.
* Attach sensor data (like temperature or humidity readings) to emails.
