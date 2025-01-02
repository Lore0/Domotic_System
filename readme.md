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