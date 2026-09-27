# FreeRTOS Multisensor Room Monitoring System

## Project Overview
An ESP32 room-monitoring system built on native ESP-IDF and simulated in Wokwi. It samples temperature, humidity and ambient light, detects motion with a PIR sensor, lets a user browse the readings on an OLED with a rotary encoder, and sounds a buzzer when temperature leaves a safe 18–30 °C range. The system also tracks an ACTIVE/INACTIVE state based on recent motion, blanking the display after 15 seconds of inactivity.

## Features
- Temperature and humidity sampling from a DHT22 (custom bit-banged driver, protected by a critical section during the timing-sensitive 40-bit read).
- Ambient light sampling from an LDR via ADC2, converted to a calibrated 0–100% scale (`Dark`/`Bright` raw-value constants).
- OLED (SSD1306, I2C) paging between Temperature / Humidity / Light / Motion, navigated with an interrupt-driven rotary encoder.
- Temperature alarm (buzzer via LEDC PWM) with hardware-independent decision logic.
- Motion-based ACTIVE/INACTIVE system state with a 15-second inactivity timeout.
- Six FreeRTOS tasks coordinated through three queues, a `QueueSet`, a mutex, and an event group.

## Learning Objectives

By completing this project, we should be able to:

1. Setup and build an ESP32 project using the native **ESP-IDF framework** through **PlatformIO** in VS Code.

2. Simulate an ESP32-based circuit using the **Wokwi extension**.

3. Implement custom APIs/libraries for sensors and actuators while utilizing built-in **ESP-IDF drivers**.

4. Create and manage multiple **FreeRTOS tasks**, including assigning appropriate task priorities.

5. Understand and apply **Blocking, Non-Blocking Delays, and Polling-based implementations** effectively within FreeRTOS tasks.

6. Implement **Inter-Task Communication (ITC)** using FreeRTOS **Queues, Event Groups, and State Machines**.

7. Utilize **Mutex (Mutual Exclusion)** to synchronize access to shared resources and prevent resource corruption.

8. Apply **unit testing and static analysis** to improve software reliability, maintainability, and code quality.

## System Architecture

in progress...
