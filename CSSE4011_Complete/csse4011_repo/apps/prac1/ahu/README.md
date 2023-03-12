# **Title: Bluetooth Controlled Weather Station (4011 Prac 1)**
##  First Name : Gabriel
## Last Name: Lippiatt
##  Student ID: 45296710
### Folder Structure: (AHU main)csse4011repo/apps/prac1/ahu
```bash
├── ahu
│   ├── blinky.zip
│   ├── build
│   ├── CMakeLists.txt
│   ├── data.json
│   ├── dtc_shell.overlay
│   ├── j.py
│   ├── mq.py
│   ├── prj.conf
│   ├── README.md
│   ├── shell.conf
│   ├── src
│   ├── sub.py
│   └── usb.conf
```
### Oslib: csse4011repo/apps/ahurivers
### Source Instructions: 


step1. put the dongle into bootloader mode by pressing the button on side


step2. build and flash using


    west  build  -b nrf52840dongle_nrf52840 && nrfutil pkg generate --hw-version 52 --sd-req=0x00 --application build/zephyr/zephyr.hex --application-version 1 
    blinky.zip && nrfutil dfu usb-serial -pkg blinky.zip -p /dev/ttyACM0


### User Instructions:
In Linux: sudo screen /dev/ttyACM0



The Set of runnable commands:


hts221 r t - read temperature


hts221 r h - read humidity


lps22 r - read air pressure


ccs811 r - read VOC


lis2dh r x - read X


lis2dh r y - read Y


lis2dh r z - read Z


rgb w <rgb> - o turns respective led, f turns off and t toggles
example: otf - red on, green is toggled from previous state and f is off


pb r - reads pushbutton state


dc w - configure duty cycle


sample w - set the sample time


all o/f - starts sampling after button is pressed


## References
https://github.com/zephyrproject-rtos/zephyr

https://github.com/uqembeddedsys-projects/csse4011_zephyr_guides/

https://www.emqx.com/en/blog/how-to-use-mqtt-in-python
        