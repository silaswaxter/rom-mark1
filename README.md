# About

This project contains the application that runs on the BLE IMU ROM sensor.

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