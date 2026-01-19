# About

This project contains the application that runs on the BLE IMU ROM sensor.

## Project Feature Sequence

This is a sequential roadmap for developing the first revision of the range of motion device. The goal is a prototype close to production-ready, but suitable for demos.

This list is a first-pass plan created between phase 0 and phase 1; it should be updated as the system takes shape.

---

## Phase 0 — Dev Kit Bring-Up

- [x] Build, flash, and run a minimal Zephyr app
- [x] "Hello world" via console (UART/RTT)
- [x] Blinky LED test
- [x] I2C communication test (ping an external device)
- [x] Verify IMU coprocessor responds to basic commands

**Deliverable:** Dev kit is operational, basic peripherals work, IMU is reachable.

---

### Phase 1 — IMU Coprocessor Integration

- [x] Minimal IMU communication
  - [x] Init + FWID verification
- [ ] Quaternion streaming
  - [ ] Start / stop command
  - [ ] Sample fetch
- [ ] IMU Configuration
- [ ] Calibration flows
  - [ ] Accel/Gyro calibration
    - [ ] Calibration status & "goodness" metric
  - [ ] Mag calibration
    - [ ] Calibration status & "goodness" metric
- [ ] IMU health model
  - [ ] Error states

**Deliverable:** Stream clean quaternions continuously and know sensor status.

---

### Phase 2 — Time & Logging

- [ ] RTC
  - [ ] Internal RTC
  - [ ] UTC epoch tracking
  - [ ] Drift handling
- [ ] Logging subsystem
  - [ ] Circular buffer
  - [ ] Overflow tracking
  - [ ] Record metadata (timestamp, type, CRC)
  - [ ] Flash backend
- [ ] Log record model
  - [ ] Boot records
  - [ ] Error records
  - [ ] Calibration records
  - [ ] IMU session records

**Deliverable:** Power-cycle safe logging with recoverable history.

---

### Phase 3 — BLE Services

- [ ] Command & Response Service
  - [ ] Structured commands
  - [ ] Versioning
  - [ ] UTC timestamps
  - [ ] Result codes
- [ ] Device Info Service
  - [ ] FW version
  - [ ] HW revision
  - [ ] Serial number
- [ ] RTC Service
  - [ ] Get time
  - [ ] Set time
- [ ] Battery Service
  - [ ] Level
  - [ ] Charge state
  - [ ] Health (later)
- [ ] IMU Status Service
  - [ ] Calibration status
  - [ ] Error status
  - [ ] Sensor readiness
- [ ] IMU Data Service
  - [ ] Start/stop streaming
  - [ ] Quaternion notifications
  - [ ] Sample rate config
- [ ] Log Transfer Service
  - [ ] Record enumeration
  - [ ] Chunked transfer
  - [ ] CRC validation

**Deliverable:** Full desktop app control over BLE.

---

### Phase 4 — UX & Physical Feedback

- [ ] RGB LED
  - [ ] Boot state
  - [ ] BLE connected
  - [ ] Streaming
  - [ ] Error
  - [ ] Charging
- [ ] Button handling
  - [ ] Power
  - [ ] Reset
  - [ ] Pairing

**Deliverable:** Device state understandable without debugger.

---

### Phase 5 — Power System

- [ ] Battery subsystem
  - [ ] Battery
  - [ ] Charger
  - [ ] Regulator
  - [ ] Fuel gauge
- [ ] Power states
  - [ ] Sleep
  - [ ] Idle
  - [ ] Streaming
  - [ ] Charging
- [ ] Battery telemetry
  - [ ] Voltage
  - [ ] Current
  - [ ] SoC
  - [ ] Charge state

**Deliverable:** Untethered operation with predictable behavior.

---

### Phase 6 — USB & Manufacturing Path

- [ ] USB-to-serial
  - [ ] Bootloader access
  - [ ] DFU
  - [ ] Debug
- [ ] Firmware update paths
  - [ ] JLink
  - [ ] USB DFU
  - [ ] BLE OTA (later)

**Deliverable:** Firmware can be updated like a real product.

---

### Phase 7 — Hardware Revision 1

- [ ] Board design
  - [ ] nRF52
  - [ ] IMU coprocessor
  - [ ] Power
  - [ ] USB
  - [ ] LEDs
  - [ ] Buttons
- [ ] Mechanical
  - [ ] Enclosure
  - [ ] Mounting
  - [ ] Wearability
- [ ] Assembly
  - [ ] Bring-up checklist

**Deliverable:** A real device suitable for hand-off demos.

---

### Phase 8 — Bring-up & Validation

- [ ] JLink flashing
- [ ] USB flashing
- [ ] BLE OTA
- [ ] Power testing
- [ ] Battery runtime
- [ ] IMU validation
- [ ] Desktop integration
- [ ] Long-duration stability test

**Deliverable:** Demo-ready prototype.


## Contributing

TBD

### Setup

1. Install vscode
1. Follow [this guide](https://academy.nordicsemi.com/courses/nrf-connect-sdk-fundamentals/lessons/lesson-1-nrf-connect-sdk-introduction/topic/exercise-1-1/) to install the nordic extension pack, toolchain, segger tools, etc.
1. `cd <project-dir>`
1. Run `west init` from the nrf terminal
1. Run `west update` from the nrf terminal
1. Restart vscode
1. Use the NRF Connect GUI inside vs-code to:
    - `APPLICATIONS <paritition> -> application -> Add build configuration`: 
        - **Select this specific board**: `nrf52DK/nrf52832`
1. Build/Flash the target.

#### (Optional) Recomended

- Install nrf Connect for Desktop
    - install the BLE tool
    - install the programmer tool