# Chaos Hoodie
## Introduction
### What is this project?
Hello and welcome to my tutorial for making the Chaos Hoodie! 

The Chaos Hoodie is an e-textiles projt designed to produce an unpredictable lighting and and sound display using sensor input and randomized behavior. The system is built using an Adafruit FLORA microcontroller alongside NeoPixels, Lilypad sewable LEDs, a Lilypad light sensor, a Lilypad buzzer and a ball bearing tilt sensor. 

Instead of responding to sensor values in a predictable way by directly mapping sensor input to actuator output, this project instead uses the sensors to generate randomness. Sensor readings from the light sensor and tilt sensor are used to continously calculate a random seed, which then directly affects the output of Arduino's `random()` function.

### Who is this for?
This project is great for anyone wishing to play around with experimental design. This system is quite atypical as while the code is written to be interactive in a way, it is still not the same kind of interactivity many other
## Materials
## Tools
## Prepare for Construction
## Construction Guide
## Reviewing the Code
## Troubleshooting
## Conclusion