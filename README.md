# Home_Work_Timer
A small and colourful project to visually and audibly notify that 15 minutes have expired.

# Warning
This design has not been compliance tested and will not be. If replicated and damage is caused I take no responsibility for this. I accept no responsibility for any damage to any equipment that results from the use of this design and its components. IT IS ENTIRELY AT YOUR OWN RISK!

# Overview
This project was designed to be used as a "home work" timer for pre-school student without the use of countdown numbers. Rather it simply fades a PL9823 8mm LED from Green through to Red over a period of 15 minutes.

At the start and end of the time period a beeper is sounded.

### Appearance
Prototype:
![Prototype](/Images/TimerPrototype.JPG)

My design goals for this design were to:

1. Have a quickly working prototype which could be immediately used.
2. Compact and have the opportunity to be powered via USB or 3 Volts.

For connections, as there are only 3 GPIOs used, refer to the source code for connections. Internal pullups are used where required and this no external resistor were used.

### BOM
For those wanting to build their own hardware, here is the BOM;

| Reference(s) | Value                  |
|--------------|------------------------|
| Micro        | ATTiny85               |
| LED          | PL9823 8mm             |
| Buzzer       | Piezo Passive Buzzer   |
| Switch       | N/O Push Button        |


### SW Libraries
For this design, I used the following open source libraries:

1. https://github.com/pkourany/clickButton
2. WS2811 RGB LED driver by Alan Burlison, alan@bleaklow.com.

These libraries were modified to work with the ATTiny85 and / or the connected Hardware. Please refer to the updated history of the references sources.
