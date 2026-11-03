# Domotic System

A command-line simulator of a home automation controller, written in C++11. It manages ten household appliances over a simulated day, turns them on and off on schedule and keeps the total load under a 3.5 kW power limit.

University project (BSc Computer Engineering, University of Padova, 2024/25), developed by a team of three.

## What it does

- **Simulated clock**: time only moves forward when you tell it to (`set time HH:MM`). Every event in between, such as a cycle ending or a timer firing, is processed in order and logged with its timestamp. Each response starts by printing the current time.
- **Power limit**: a device is turned on only if the new total load stays within the 3.5 kW grid limit plus whatever the solar panels are producing. Otherwise the command is rejected with an error.
- **Two kinds of devices**:
  - *manual* devices stay on until they are turned off or their timer expires;
  - *fixed-cycle* devices run for a fixed duration (e.g. the washing machine runs for 1h50) and then switch off by themselves.
- **Timers**: schedule a device to start, and optionally stop, at a given time.
- **Energy report**: `show` prints the energy produced and consumed by each device so far.
- **Session log**: every command and response is written to `log.txt` when the program ends.

The user interface (commands, device names and messages) is in Italian.

## Devices

The devices are read from [`docs/device.txt`](docs/device.txt):

| Device (name to type) | English | Cycle | Power |
|---|---|:---:|:---:|
| `Impianto_fotovoltaico` | solar panels | manual | +1.5 kW |
| `Lavatrice` | washing machine | 01:50 | -2 kW |
| `Lavastoviglie` | dishwasher | 03:15 | -1.5 kW |
| `Pompa_di_calore+termostato` | heat pump + thermostat | manual | -2 kW |
| `Tapparelle_elettriche` | electric shutters | 00:01 | -0.3 kW |
| `Scaldabagno` | water heater | manual | -1 kW |
| `Frigorifero` | fridge | manual | -0.4 kW |
| `Forno_a_microonde` | microwave | 00:02 | -0.8 kW |
| `Asciugatrice` | dryer | 01:00 | -0.5 kW |
| `Televisore` | TV | 01:00 | -0.2 kW |

Positive power is production, negative power is consumption.

## Build and run

Requires CMake 3.10+ and a C++11 compiler.

```bash
cmake -S . -B build
cmake --build build
cd build && ./main
```

Run the program from inside `build/`: it loads the devices from `../docs/device.txt`.

## Commands

| Command | Effect |
|---|---|
| `set <device> on` / `set <device> off` | turn a device on or off now |
| `set <device> HH:MM [HH:MM]` | set a timer: start time, plus an optional stop time for manual devices |
| `rm <device>` | remove the device's timer |
| `show` | energy produced and consumed by every device |
| `show <device>` | the same, for one device |
| `set time HH:MM` | advance the clock, processing every event up to that time |
| `reset time` | bring the clock back to 00:00, keeping only the timers that have not run to completion |
| `reset timers` | remove every timer |
| `reset all` | both of the above |
| `exit` | stop immediately |

Setting the time to a moment earlier than the current one ends the day: the scheduled actions are completed and the program exits.

## Example

```text
>> set Lavatrice on
[00:00] L'orario attuale è 00:00
[00:00] Il dispositivo Lavatrice si è acceso

>> set Forno_a_microonde on
[00:00] L'orario attuale è 00:00
[00:00] Il dispositivo Forno_a_microonde si è acceso

>> set Pompa_di_calore+termostato on
[00:00] L'orario attuale è 00:00
[00:00] ERRORE: non c'è corrente sufficiente per accendere-> Pompa_di_calore+termostato

>> set Scaldabagno 05:00 12:00
[00:00] L'orario attuale è 00:00
[00:00] Impostato un timer per il dispositivo Scaldabagno dalle 05:00 alle 12:00

>> set time 13:00
[00:00] L'orario attuale è 00:00
[00:02] Il dispositivo Forno_a_microonde si è spento
[01:50] Il dispositivo Lavatrice si è spento
[05:00] Il dispositivo Scaldabagno si è acceso
[12:00] Il dispositivo Scaldabagno si è spento
[13:00] L'orario attuale è 13:00
```

The heat pump is rejected because the washing machine (2 kW) and the microwave (0.8 kW) are already drawing 2.8 kW, and adding 2 kW would exceed the limit. Two complete sessions are in [`examples/`](examples/).

## Design

```
DomoticSystem   command parsing, device registry, timers, logging
 └── Scheduler  running and waiting queues ordered by end time, power accounting
Device          name, power, start/end time, on/off state
 ├── Manual         stays on until turned off or its timer ends
 └── PrefixCycle    fixed duration, switches itself off
Clock           HH:MM time arithmetic and validation (InvalidTimeFormat)
```

## Authors

- **Lorenzo Panizzolo**: `DomoticSystem` (controller, command parser, timers, logging) and `main`
- **Riccardo Chellin**: `Scheduler`
- **Federico Compagno**: `Device`, `Manual`, `PrefixCycle`, `Clock`, `InvalidTimeFormat`

The code follows the [Google C++ Style Guide](https://google.github.io/styleguide/cppguide.html).
