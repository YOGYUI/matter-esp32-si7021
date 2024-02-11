# Matter Temperature/Humidity Sensor Example (ESP32 + Si7021)
Matter 온습도 센서(Temperature/Humidity Sensor) 예제 프로젝트<br>
다음 2종류의 Matter 클러스터들에 대한 코드 구현 방법을 알아본다
- Temperature Measurement (Cluster Id: `0x0402`)
- Relative Humidity Measurement (Cluster Id: `0x0405`)

Hardware
---
[Si7021](https://www.silabs.com/documents/public/data-sheets/Si7021-A20.pdf): I2C 통신 방식의 온습도 센서 IC 사용<br>
<p style="text-align:center"><img src="https://github.com/adafruit/Adafruit_Si7021/raw/master/assets/board.jpg?raw=true" width="300"></p><br>

I2C GPIO 핀번호 변경은 /main/include/definition.h에서 아래 항목을 수정<br>
default: `SDA` = GPIO`18` / `SCL` = GPIO`19`
```c
#define GPIO_PIN_I2C_SCL 19
#define GPIO_PIN_I2C_SDA 18
```

SDK Version
---
- esp-idf: [v5.1.2](https://github.com/espressif/esp-idf/tree/v5.1.2)
- esp-matter: [fe4f9f69634b060744f06560b7afdaf25d96ba37](https://github.com/espressif/esp-matter/commit/fe4f9f69634b060744f06560b7afdaf25d96ba37)
- connectedhomeip: [d38a6496c3abeb3daf6429b1f11172cfa0112a1a](https://github.com/project-chip/connectedhomeip/tree/d38a6496c3abeb3daf6429b1f11172cfa0112a1a)
  - Matter 1.1 released (2023.05.18)
  - Matter 1.2 released (2023.10.23)

Helper Scripts
---
SDK 클론 및 설치
```shell
$ source ./scripts/install_sdk.sh
```
SDK (idf.py) 준비
```shell
$ source ./scripts/prepare_sdk.sh
```

Build & Flash Firmware
---
1. Factory Partition (Matter DAC)
    ```shell
    $ source ./scripts/flash_factory_dac_provider.sh
    ```
2. Configure project
    ```shell
    $ idf.py set-target esp32
    ```
3. Build Firmware
    ```shell
    $ idf.py build
    ```
4. Flash Firmware
    ```shell
    $ idf.py -p ${seiral_port} flash monitor
    ```

QR Code for commisioning
---
![qrcode.png](./resource/DACProvider/qrcode.png)

References
---
[Matter 온습도 센서 개발 예제 (ESP32)](https://yogyui.tistory.com/entry/PROJ-Matter-%EC%98%A8%EC%8A%B5%EB%8F%84-%EC%84%BC%EC%84%9C-%EA%B0%9C%EB%B0%9C-%EC%98%88%EC%A0%9C-ESP32)<br>
