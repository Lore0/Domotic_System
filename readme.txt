Domotic System - ASD Bojon

Componenti:
    * Chellin Riccardo
    * Compagno Federico
    * Panizzolo Lorenzo


- Struttura file:
Domotic_System/
├── CMakeLists.txt
├── docs
│   └── device.txt
├── include
│   ├── Clock.h
│   ├── Device.h
│   ├── DomoticSystem.h
│   ├── InvalidTimeFormat.h
│   ├── Manual.h
│   ├── PrefixCycle.h
│   └── Scheduler.h
├── log1.txt
├── log2.txt
├── readme.txt
└── src
    ├── Clock.cpp
    ├── Device.cpp
    ├── DomoticSystem.cpp
    ├── Manual.cpp
    ├── PrefixCycle.cpp
    ├── Scheduler.cpp
    └── main.cpp


- Dispositivi possibili

| Dispositivo                   |   Durata   |   Potenza   |
| ----------------------------- | :--------: | :---------: |
| Impianto_fotovoltaico         |  Manuale   |   +1.5 kW   |
| Lavatrice                     |   01:50    |    -2 kW    |
| Lavastoviglie                 |   03:15    |   -1.5 kW   |
| Pompa_di_calore+termostato    |  Manuale   |    -2 kW    |
| Tapparelle_elettriche         |   00:01    |   -0.3 kW   |
| Scaldabagno                   |  Manuale   |    -1 kW    |
| Frigorifero                   |  Manuale   |   -0.4 kW   |
| Forno_a_microonde             |   00:02    |   -0.8 kW   |
| Asciugatrice                  |   01:00    |   -0.5 kW   |
| Televisore                    |   01:00    |   -0.2 kW   |


- Dettagli implentativi:
    * fine del programma
        ~ >> exit : uscita forzata dal programma (azioni in corso non vengono completate)
        ~ >> set time $(time < current_time) : finisce la giornata completando le azione programmate
    * nome dispositivi
        ~ tabella sopra (lo spazio viene sostituito da "_")
    * reset time
        ~ per scelta implementativa si mantengono solo i timer non completamente consumati 
    * potenza raggiunge kMaxPower (3.5 kW) o accensione di un nuovo dispositivo provoca sovraccarico
        ~ non permette l'accensione del dispositivo in quanto il testo recita "il sistema inzia a spegnere i 
        dispositivi nell'ordine inverso rispetto all'accensione"


- Tool utilizzati:
    * github
    * vscode / neovim
    * CMake
    * Google C++ Style Guide
