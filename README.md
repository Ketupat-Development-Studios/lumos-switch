# Lumos Switch Arduino hardware/firmware - v0.0.1 Prototype

We used a low-cost Arduino Uno micro-controller to prototype our device (mechanical IoT-enabled switch flipper) together with a HC-05 bluetooth module and a TowerPro SG90 servo.

A 3d-printed housing is used to secure the servo onto the switch surface and a custom arduino shield was soldered on a breadboard to reduce the overall profile of our setup.

<img src="Circuit_Diagram.png" width="560">

Final overall setup:

<img src="Setup.jpg" width="100%">

## Files

[**blue_shield**](blue_shield) -> Arduino firmware driving the switch

[**hc05_config**](hc05_config) -> Code to allow Arduino serial terminal to configure hc-05 bluetooth module

[**servo_housing.stl**](servo_housing.stl) -> Stl file to 3d-print servo housing
