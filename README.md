# ESP32 LED Snake
A fun little side project of mine. If you are only interested in how the final thing looks, then [media](#media) section is for you, otherwise I will try to explain a bit more.

# Build process

## Screen
The main thing I wanted to do, was of course to play around with LEDs. I bought a 5m ARGB LED strip - WS2812B, 5v, 60 led/m, IP2X. The first thing I did was glue half of it to my desk from the bottom. The second thing was the screen. I cut 10 stripes of 10 LEDs and soldered them with some wires in a serpentine pattern:
```
> > > > V
V < < < <
> > > > V
V < < < <
> > > > >
```
Bought two plexi panels - black and white. Sticked LEDs to the black one, made some holes for screws, put white on top and called it a day.

## Controller
For a controller I bought an ESP-WROOM-32 and a simple joystick (two axis and a button). Connected it with some wires and dupont connectors, put everything in a cheap... food box? In the end, it's got joystick on top and on the side a DC-in port and a 3-pin dupont connector for a cable to the screen.

## Code
Last but not least, the logic of it all. I think I started coding with [Arduino IDE](https://www.arduino.cc/en/software), but then moved to [VS Code](https://code.visualstudio.com) with [PlatformIO](https://platformio.org) extenstion. Probably just didn't like Arduino IDE experience, I don't really remember. 'Cause yeah, I'm doing this project for 10 months now. I mean, not 10 months, just within 10 months. For the most part it lay down in a drawer 95% ready, just didn't feel like finishing it.

# Media
As this project is kinda in real life, I have to show photos - not screenshots, and the only thing I have, that makes photos is my cheap phone. So yeah, quality of th... sorry, what quality?

A few shots of the game "screen":
![Play button](https://i.imgur.com/QgU0B3D.jpeg)
![Brightness setting](https://i.imgur.com/hvgoeTG.jpeg)
![In-game shot](https://i.imgur.com/hZkCwbQ.jpeg)

Screen all around:
![](https://i.imgur.com/7yO09jy.jpeg)
![](https://i.imgur.com/hrmFcPj.jpeg)
Yeah, legs are different length, cause screw holes aren't perfect. That's how real DIY project looks like \*shrug\*
![](https://i.imgur.com/P3PsCmM.jpeg)

And a short demonstration video (click the thumbnail):<br />
[![Demonstration video](https://img.youtube.com/vi/hBAqEBxcC9g/0.jpg)](https://www.youtube.com/watch?v=hBAqEBxcC9g)

# License
This project uses [MIT](https://github.com/Ynfuien/esp32-led-snake/master/blob/LICENSE) license.