# 🔐 Secure Product Tracking System

A real-time **Product Tracking System** using **RFID technology** and **ESP32**, designed for **automated data collection**, **secure handling**, and **RESTful API integration**. This project ensures reliable tracking of goods with LED-based status indicators and focuses on **scalability**, **efficiency**, and **data integrity**.

---

## 🚀 Features

- 📡 **RFID-based Product Identification**
- 🌐 **ESP32 for Wireless Communication**
- 🔄 **REST API Integration** for real-time data syncing
- 🔒 **Secure Data Handling** using HTTP authentication
- 💡 **LED Indicators** to show status (Success, Error, Processing)
- ⚙️ **Modular Design** for easy scalability and updates

---

## 🧰 Tech Stack

| Component            | Technology          |
|---------------------|---------------------|
| Microcontroller      | ESP32               |
| Identification       | RFID Reader + Tags  |
| Communication        | HTTP/REST API       |
| Backend API          | Node.js / Express / Flask *(customizable)* |
| Database             |  SQL |
| Indicator Feedback   | LED lights (Red, Green, Yellow) |

---

## 🛠️ How It Works

1. **RFID tag is scanned** via the RFID module.
2. **ESP32 reads the tag** and sends data to the backend using a REST API.
3. The backend **validates and stores the data** securely.
4. Based on the response:
   - ✅ Green LED = Valid Product
   - ❌ Red LED = Invalid Tag
   - ⏳ Yellow LED = Processing/Waiting

---



