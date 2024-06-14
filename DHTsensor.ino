#include <WiFi.h>
#include <HTTPClient.h>
#include <DHT.h>

#define DHTPIN 9      // Pin data sensor DHT
#define DHTTYPE DHT22 // Tipe sensor DHT yang digunakan

const char* ssid = "Wokwi-GUEST";
const char* password = "";
const char* serverUrl = "http://127.0.0.1:5000/dht/sensordata"; // Ganti dengan alamat IP server Flask Anda

DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(115200);
  dht.begin();
  connectToWiFi();
}

void loop() {
  delay(2000);

  // Membaca data dari sensor
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();

  if (isnan(temperature) || isnan(humidity)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  // Mengirim data ke server
  sendData(temperature, humidity);
}

void connectToWiFi() {
  Serial.println("Connecting to WiFi");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("\nWiFi connected");
}

void sendData(float temperature, float humidity) {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;

    // Membuat objek JSON
    String jsonStr = "{\"temperature\":" + String(temperature) + ",\"humidity\":" + String(humidity) + "}";

    // Mengirim data ke server dengan metode POST
    http.begin(serverUrl);
    http.addHeader("Content-Type", "application/json");
    int httpResponseCode = http.POST(jsonStr);

    if (httpResponseCode > 0) {
      Serial.print("HTTP Response code: ");
      Serial.println(httpResponseCode);
    } else {
      Serial.print("Error code: ");
      Serial.println(httpResponseCode);
    }

    // Menutup koneksi HTTP
    http.end();
  } else {
    Serial.println("WiFi Disconnected");
  }
}