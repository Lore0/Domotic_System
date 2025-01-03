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
│   └── Time.h
└── src
    ├── Device.cpp
    ├── DomoticSystem.cpp
    ├── Manual.cpp
    ├── PrefixCycle.cpp
    ├── Scheduler.cpp
    └── Time.cpp
```

### ToDo

- ~~su `DomoticSystem.cpp` ricordati di deallocare puntatori~~
- fare vari `handle()`su `handleInput()` per evitare tutti gli `if-else` annidati
- rindondanza di check
- log
- sistemare che set i set cercano sempre un device
    - da non fare perche set time non vuole device

