# RaspberryClient

## UI
using 1920 * 1080
font 32pt

## pinmap (BCM)
gpio 22 = ultra trig pin
gpio 23 = ultra echo pin
gpio 17 = piezo buzzer pin
gpio 18 = led pin //for debug

### rc522
Vcc 3.3
gnd gnd
sda = gpio 8  = ce0
sck = gpio 11 = sclk
MOSI = MOSI
MISO = MISO
rst = gpio 25

## RC522 code
https://github.com/lue7/rc522-RPi
-> 위 위치에서 comm commspi MFRC 가져와서 사용했습니다.
-> 코드상에 들어있으니 참고만 하면 됩니다.
