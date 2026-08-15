# Schedule — gestion simplifiée de tâches (micros() / millis())

Bibliothèque **header-only** (un seul fichier `Schedule.h`, pas de `.cpp`) pour exécuter des tâches périodiques **non bloquantes** (busy-check, pas de `delay()`). Deux classes à API identique :

| Classe | Horloge | Unité des périodes | Wrap-around |
|---|---|---|---|
| `Micro_Scheduler` | `micros()` | µs | ~71 min |
| `Milli_Scheduler` | `millis()` | ms | ~49,7 jours |

## Installation

### PlatformIO

```ini
lib_deps =
    https://github.com/Fo170/Schedule.git@^1.0.0
```

### Arduino IDE

Copier `Schedule.h` dans le dossier du sketch puis `#include "Schedule.h"`.

> ⚠️ `Schedule.h` n'inclut **pas** `Arduino.h` : le core Arduino (`micros()` / `millis()`) doit être inclus **avant**. En pratique, Arduino IDE et PlatformIO l'incluent automatiquement — par sûreté, écrire `#include <Arduino.h>` en premier dans votre sketch.

## API

Méthodes identiques pour `Micro_Scheduler` et `Milli_Scheduler` :

| Méthode | Rôle |
|---|---|
| `Init(unsigned long delay, bool etat)` | Période initiale (`delay`) et état actif/inactif |
| `Run()` | Active le scheduler |
| `Stop()` | Désactive le scheduler |
| `TestDelay()` | `true` si la période est dépassée (voir piège ci-dessous) |
| `Change_Delay(unsigned long delay)` | Modifie la période en vol |
| `Reset()` | Re-arme la mesure du temps (nouveau point de départ) |
| `Get_Etat()` | Renvoie l'état actif/inactif |

Macros de périodes prédéfinies (unités selon la classe : µs pour `Micro_Scheduler`, ms pour `Milli_Scheduler`) :
`_SCHEDULE_1s_` = 1000, `_SCHEDULE_10s_` = 10000, `_SCHEDULE_60s_` = 60000.

## Exemple minimal

```cpp
#include <Arduino.h>
#include <Schedule.h>

Milli_Scheduler tache;                 // tâche lente, basée sur millis()

void setup() {
  Serial.begin(115200);
  tache.Init(_SCHEDULE_1s_, true);     // active, période 1 s
}

void loop() {
  if (tache.TestDelay()) {             // 1 s écoulée ?
    tache.Reset();                     // re-arme pour la période suivante
    Serial.println("travail périodique…");
  }
}
```

Exemple complet : `examples/BasicUsage/BasicUsage.ino` (démo des deux classes + `Run`/`Stop`/`Change_Delay` via Série).

## Points d'attention (à ne pas « simplifier »)

- **`TestDelay()` ne re-arme pas** : une fois `delay` dépassé, il renvoie `true` à **chaque** appel tant qu'on n'appelle pas `Reset()`. Toujours appeler `Reset()` juste après avoir traité la tâche.
- **Dépassement d'horloge** : `TestDelay()` fait `micros() - t0` / `millis() - t0`. La soustraction modulo `2^32` est **overflow-safe** (49,7 j pour millis, ~71 min pour micros). Ne pas remplacer par `now() >= t0 + delay` qui **déborde** et rendrait le délai quasi nul.
- **Header-only non `inline`** : ne pas inclure `Schedule.h` depuis plusieurs fichiers sources (`TU`) d'un même programme.
- **API en anglais** (`Run`/`Stop`/`TestDelay`/…) : ne pas renommer les méthodes, utilisées telles quelles par les sketches.

## Licence

GPL-3.0-only — voir `LICENSE`. Auteur : Olivier FOURNET (org [Fo170](https://github.com/Fo170)).
