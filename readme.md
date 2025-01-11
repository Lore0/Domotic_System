### ToDo

- fare vari `handle()`su `handleInput()` per evitare tutti gli `if-else` annidati
- rindondanza di check
- `try-catch` di Time (ogni volta che metto un ora)

### Struttura progetto

```
Domotic System:
├── README.md
├── docs
│   ├── device.txt
│   └── progetto.pdf
├── include
│   ├── Device.h
│   ├── DomoticSystem.h
│   ├── Manual.h
│   ├── PrefixCycle.h
│   ├── Scheduler.h
│   └── Clock.h
└── src
    ├── Device.cpp
    ├── DomoticSystem.cpp
    ├── Manual.cpp
    ├── PrefixCycle.cpp
    ├── Scheduler.cpp
    └── Clock.cpp
```

### Dispositivi (tabella con caratteristiche)
| **Dispositivo**              | **Durata** | **Potenza** |
|------------------------------|:----------:|:-----------:|
| Impianto_fotovoltaico        |   Manuale  |   +1.5 kW   |
| Lavatrice                    |    01:50   |    -2 kW    |
| Lavastoviglie                |    03:15   |   -1.5 kW   |
| Pompa_di_calore_+_termostato |   Manuale  |    -2 kW    |
| Tapparelle_elettriche        |    00:01   |   -0.3 kW   |
| Scaldabagno                  |   Manuale  |    -1 kW    |
| Frigorifero                  |   Manuale  |   -0.4 kW   |
| Forno_a_microonde            |    00:02   |   -0.8 kW   |
| Asciugatrice                 |    01:00   |   -0.5 kW   |
| Televisore                   |    01:00   |   -0.2 kW   |
