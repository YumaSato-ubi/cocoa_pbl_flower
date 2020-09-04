#include <Arduino.h>
#include <M5StickC.h>
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEScan.h>
#include <BLEAdvertisedDevice.h>

int scanTime = 2; // スキャンする時間（秒）
BLEScan* pBLEScan;

// 接触確認アプリのUUID
const char* uuid = "0000fd6f-0000-1000-8000-00805f9b34fb";

int deviceNum_all = 0; // アプリの数のカウンター
int deviceNum_cocoa = 0; // COCOAの数のカウンター
int rssiNum = -50; // RSSI(Received Signal Strength Indicator)電波強度の初期値（狭いエリア）

class MyAdvertisedDeviceCallbacks: public BLEAdvertisedDeviceCallbacks {
    void onResult(BLEAdvertisedDevice advertisedDevice) {
      int rssi = advertisedDevice.getRSSI();
      if (rssi > rssiNum) { // rssiNum の範囲内のとき
        deviceNum_all++;
        Serial.println(advertisedDevice.toString().c_str());
//        if (advertisedDevice.haveServiceUUID()) {
//          if (strncmp(advertisedDevice.getServiceUUID().toString().c_str(), uuid, 36) == 0) { // 検出したUUIDが接触確認アプリのUUIDと同じとき
//            Serial.print("RSSI: ");
//            Serial.println(rssi);
//            Serial.print("ADDR: ");
//            Serial.println(advertisedDevice.getAddress().toString().c_str());
//            deviceNum_cocoa++; // カウンターに１加算
//            deviceNum_all--;
//            
//          }
//        }
//        Serial.print("All: ");
//        Serial.println(deviceNum_all);
//        Serial.print("COCOA: ");
//        Serial.println(deviceNum_cocoa);
      }
    }
};

void drawScreen() {
  M5.Lcd.fillScreen(BLACK);
  M5.Lcd.setTextSize(2);
  M5.Lcd.setCursor(0, 0);
  M5.Lcd.setTextColor(GREEN);
  if (rssiNum < -50) { // 広いエリアを受信したとき
    M5.Lcd.print("Area:wide\n");
  } else {
    M5.Lcd.print("Area:narrow\n");
  }
  M5.Lcd.setTextSize(7);
  M5.Lcd.setTextColor(RED);
  M5.Lcd.printf(" %2d", deviceNum_all); // カウンターを表示
  M5.Lcd.setTextSize(1);
  M5.Lcd.setCursor(0, M5.Lcd.height() - 10);
  M5.Lcd.printf("Bat:%4.1fV  ", M5.Axp.GetBatVoltage());
  M5.Lcd.printf("Charge:%5.1f\n", M5.Axp.GetBatCurrent());
}

void Task1(void *pvParameters) {
  // loop()に書くとBLEスキャン中M5.update()が実行されずボタンが取れないためマルチスレッド化されている
  while (1) {
    // カウンターを初期化
    deviceNum_all = 0;
    deviceNum_cocoa = 0;
    BLEScanResults foundDevices = pBLEScan->start(scanTime, false); // 指定した時間スキャンする
//    Serial.print("Devices found: ");
//    Serial.println(deviceNum);
//    Serial.println("Scan done!");
    pBLEScan->clearResults();   // delete results fromBLEScan buffer to release memory
    drawScreen(); // 画面に表示する
  }
}

void setup() {
  M5.begin();
  Serial.begin(115200);
  Serial.println("Scanning...");
  M5.Lcd.setRotation(1);
  M5.Axp.ScreenBreath(8);

  BLEDevice::init("");             // BLEモジュールの初期化
  pBLEScan = BLEDevice::getScan(); // スキャンオブジェクトを取得
  pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks()); // コールバック関数（アドバタイジング受信）
  pBLEScan->setActiveScan(false);  // active scan uses more power, but get results faster
  pBLEScan->setInterval(1000);     // スキャン間隔1秒
  pBLEScan->setWindow(999);       // less or equal setInterval value
  xTaskCreatePinnedToCore(         // コア指定タスク作成
    Task1,
    "Task1",
    4096, //スタックサイズ(Byte)
    NULL,
    3,    //作成タスクの優先順位(0:低 - 25:高)
    NULL,
    1     //利用するCPUコア(0-1)
  );
}

void loop() {
  M5.update();
  if ( M5.BtnA.wasPressed() ) { // Aボタン１回押し
    rssiNum = -90; // 広いエリア
  }
  if ( M5.BtnB.wasPressed() ) { // Bボタン１回押し
    rssiNum = -50; // 狭いエリア
  }
}
