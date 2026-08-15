// Schedule – exemple de base
// Deux tâches périodiques NON bloquantes (pas de delay()) :
//   - Milli_Scheduler : basée sur millis()   -> clignote toutes les 1 s
//   - Micro_Scheduler : basée sur micros()   -> clignote toutes les 500 ms
//
// Commandes Série (115200 bauds) :
//   'r'  -> Run()          active les deux schedulers
//   's'  -> Stop()         désactive les deux schedulers
//   '1'  -> Change_Delay() période 1 s
//   '2'  -> Change_Delay() période 10 s (millis) / 5 s (micros)

#include <Arduino.h>
#include <Schedule.h>

#ifndef LED_BUILTIN
  #define LED_BUILTIN 2   // LED interne des cartes ESP32 / ESP8266 qui ne définissent pas LED_BUILTIN
#endif

Milli_Scheduler tacheMillis;   // tâche lente  : basée sur millis()
Micro_Scheduler tacheMicros;   // tâche rapide : basée sur micros()

void setup() {
  Serial.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT);

  // Périodes initiales : actifs, 1 s (millis) et 500 ms (micros)
  tacheMillis.Init(_SCHEDULE_1s_, true);
  tacheMicros.Init(500000UL, true);          // 500000 µs = 500 ms

  Serial.println("Schedule : exemple Micro_Scheduler + Milli_Scheduler");
}

void loop() {
  // --- Tâche lente (millis) ------------------------------------------
  if (tacheMillis.TestDelay()) {             // la période est dépassée ?
    tacheMillis.Reset();                     // re-arme pour la période suivante
    digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
    Serial.println("millis  : 1 s écoulée");
  }

  // --- Tâche rapide (micros) -----------------------------------------
  if (tacheMicros.TestDelay()) {
    tacheMicros.Reset();
    Serial.println("micros  : 500 ms écoulées");
  }

  // --- Démonstration des autres méthodes ------------------------------
  if (Serial.available()) {
    switch (Serial.read()) {
      case 'r':
        tacheMillis.Run();
        tacheMicros.Run();
        Serial.println("Run()   -> schedulers actifs");
        break;
      case 's':
        tacheMillis.Stop();
        tacheMicros.Stop();
        Serial.println("Stop()  -> schedulers arrêtés");
        break;
      case '1':
        tacheMillis.Change_Delay(_SCHEDULE_1s_);
        tacheMicros.Change_Delay(1000000UL); // 1000000 µs = 1 s
        Serial.println("Change_Delay() -> 1 s");
        break;
      case '2':
        tacheMillis.Change_Delay(_SCHEDULE_10s_);
        tacheMicros.Change_Delay(5000000UL); // 5000000 µs = 5 s
        Serial.println("Change_Delay() -> 10 s / 5 s");
        break;
    }
  }
}
