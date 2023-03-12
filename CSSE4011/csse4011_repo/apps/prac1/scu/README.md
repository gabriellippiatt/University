# **Title: Bluetooth Controlled Weather Station**
## First Name: Fardeen
## Last Name: Bhuiyan
## Student ID: 4526635
## Folder Structure: (SCU main)csse4011repo/apps/prac1/scu
### Oslib: csse4011repo/apps/scudrivers
```bash
└── scu
    ├── build
    ├── CMakeLists.txt
    ├── main.c
    ├── prj.conf
    ├── README.md
    ├── speaker.overlay
    └── src
```


### Source Instructions: 

step1. build using 


    west build -p auto -b thingy52_nrf52832


step2. flash using 


    west flash -r jlink (may need to call twice)


### User Instructions:


Make sure switch is left (this ensures its on)


Pressing the push button in middle will alter PB status


Temperature sensor output 


Humdity sensor output


Airpressure sensor output

VOC sensor output


RGB LED's controlled via BT


Buzzer Controlled bia BT


### References
https://github.com/zephyrproject-rtos/zephyr

https://github.com/uqembeddedsys-projects/csse4011_zephyr_guides/





        