# smart_container
hanium project 2019


# smart container project
* 3D printing, Arduino, raspberrypi를 활용한 smart container project
* 온도, 습도, LOCK정보를 displaying
* GPS sensor를 통한 위치 정보 수집
* 비밀번호와 RFID를 이용한 이중보안
* 펠티어 소자를 이용한 온도 조절
* ESP8266 wifi 통신을 통해 container에 부착된 Arduino와 server역할을 하는 raspberrypi 간의 통신
* 각 container의 정보를 DB에 upload하여 관리
* container의 정보를 열람할 수 있는 webpage 제작/관리

# role

### Sihyun

* server management on raspberrypi
* raspberrypi setting(raspbian)
* apache web server setting
* mariadb setting
* user 정보/접근할 수 있는 container ID 관리
* TCP/IP socket programming을 통해 data 수신 및 DB에 upload
* container 정보를 열람하는 webpage 제작



### Jisu

- Arduino Mega 동작 구현
  - DHT11로 온,습도 측정 후 LCD 출력
  - 온도 기준에 맞춰 펠티어 소자 ON/OFF(Mosfet 활용)
  - Servo Motor, RFID(RC522), Keypad 사용한 LOCK/UNLOCK 동작
- 회로 구성


# server.c
* TCP/IP socket programming
* data 수신
* DB의 해당 container ID table로 data upload


# Webpage
* PHP files
* DB와의 connection을 통해 container data를 보여줄 수 있다

* main page는 login page
* user/administrator login
* administrator login 상태에서는 user list와 활성화 상태 수정 가능
* user login 후, access 가능한 container list displaying
* container 선택 시, 해당 container의 각 항목별 최신 정보를 보여줌
* temperature/humidity/security(LOCK,UNLOCK)/location
* 각 항목을 선택 시 해당 항목의 시간별 value list
* temperature 항목의 경우, 펠티어 소자의 ON/OFF를 같이 출력
* location 항목의 경우, google map 상에 위치를 띄워 가시성 확보
