#include <M5Stack.h>

const uint64_t workTime = 25 * 60 * 1000; // msec
const uint64_t breakTime = 5 * 60 * 1000;
uint64_t elapsed = 0;
uint64_t lastMillis = 0;
enum State {
  RUNNING,
  BREAK,
  PAUSED
};

State state, lastState;

void setup() {
  // put your setup code here, to run once:
  M5.begin();
  M5.Power.begin();

  M5.Lcd.setBrightness(10);
  M5.Lcd.clear(BLACK);
  elapsed = 0;
  state = PAUSED;
  lastState = RUNNING;
}

void loop() {
  M5.update();
  // put your main code here, to run repeatedly:
  M5.Lcd.setCursor(50, 50);
  M5.Lcd.setTextColor(WHITE, BLACK);
  M5.Lcd.setTextSize(10);
  M5.Lcd.clear();
  
  if (M5.BtnA.wasReleased()) {
    if (state != PAUSED) {
      lastState = state;
      state = PAUSED;
    } else {
      state = lastState;
    }
    Serial.println("btna");
  }
  if (M5.BtnB.wasReleased()) {
    elapsed = 0;
    Serial.println("btnb");
  }
  if (M5.BtnC.wasReleased()) {
    if (state == RUNNING) {
      state = BREAK;
      elapsed = 0;
    } else if (state == BREAK) {
      state = RUNNING;
      elapsed = 0;
    }
    Serial.print("btnc");
    Serial.println(state == RUNNING ? "running" : "break");
  }

  if (state == RUNNING || state == BREAK) {
    elapsed += millis() - lastMillis;
  }

  uint64_t displayTime;
  if (state == RUNNING) {
    if (elapsed > workTime) {
      state = BREAK;
      elapsed = 0;
    }
    displayTime = workTime - elapsed;
  } else if (state == BREAK) {
    if (elapsed > breakTime) {
      state = RUNNING;
      elapsed = 0;
    }
    displayTime = breakTime - elapsed;
  } else {
    displayTime = lastState == RUNNING ? workTime - elapsed : breakTime - elapsed;
  }

  uint32_t remainingMinutes = (uint32_t)(displayTime / (60 * 1000));
  uint32_t remainingSeconds = (uint32_t)(displayTime % (60 * 1000)) / 1000;
  Serial.printf("%ld, %d\n", displayTime, remainingSeconds);
  if (state == RUNNING) {
    M5.Lcd.setTextColor(WHITE);
  } else if (state == BREAK) {
    M5.Lcd.setTextColor(BLUE);
  } else {
    M5.Lcd.setTextColor(RED);
  }
  M5.Lcd.printf("%02ld:%02ld\n", remainingMinutes, remainingSeconds);
  lastMillis = millis();
  delay(500);
}
