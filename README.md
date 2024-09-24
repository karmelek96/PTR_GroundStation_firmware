[Instrukcja w języku polskim](https://github.com/PTR-projects/PTR_GroundStation_firmware/edit/readme_instructions/README_PL.md)
# PTR Groundstation
Firmware for the TTGO family of ESP32 / LoRa / OLED boards, that turns them into powerful groundstations for avionics systems developed by [PTR-Projects](https://github.com/PTR-projects).
## Installation and setup
1. Download and unzip the latest binary for your board from the [releases](https://github.com/PTR-projects/PTR_GroundStation_firmware/releases/latest) section.
2. Flash the binary onto the board, with address offset 0. [You can use this online tool](https://espressif.github.io/esptool-js/) which is compatible with most popular browsers.
To do this, plug your board into your computer, power it on, and select it from the COM port list. Select the .bin file you downloaded, set the flash address offset to `0x0`, and click program.
Note: if you're having problems, try a different browser.
![image](https://github.com/user-attachments/assets/0a8ee731-6ea4-45b8-bbbe-b01a5d15f8f0)

   
At this point you're done. If the display works, but appears broken, follow the next steps.

3. Connect to the board's wifi network, it should start with `PTR-GS`. You may need to disable mobile data when connecting from a phone.
4. Open your browser and go to `192.168.4.1`
5. Scroll down to `Change OLED driver`, change it, and click `Change driver`.

The display should be fixed, and the board is ready to receive telemetry.

## Usage
### Display
Important parameters are displayed on the OLED display.
![image](https://github.com/user-attachments/assets/30f719f9-cf0c-4b11-9134-577d8493df3d)
Note: To be able to use the bearing and distance readouts, you must have a GPS-equipped board.

Note: If the signal with the selected device is lost, the packet rate readout will change into a counter which will display how many seconds have elapsed since a packet was last received.
![image](https://github.com/user-attachments/assets/2a844e23-0c5e-46e3-8cec-db363d5423bc)
### Device ID 
The ID is a way of differentiating devices that transmit on the same frequency. This allows one groundstation to receive many telemetry streams (for example form a multistage vehicle, or an event with many launches).
The ID setting allows you to choose which device you want to receive, effectively acting as a filter. If you want to receive all devices on a given frequency, set the ID to `0`.
### WebGUI
The WebGUI is how the settings on the device can be changed. To access the WebGUI, connect to the wifi network created by the ground station. It starts with `PTR-GS`, but the unique name is displayed on the screen during powerup.
Next, open your browser and go to `192.168.4.1`. If you're on a mobile device, you may need to disable mobile data.

Here are some things you can do in the WebGUI:
- Change the ground station settings, tune the radio, set the ID filter.
- View all telemetry streams in a table, organized by ID.
- View telemetry of a specific device, and open its location in Google maps, for easy recovery.
- Download raw telemetry CSV logs, which contain extra information like apogee and GPS satellite number.
![image](https://github.com/user-attachments/assets/533af8c4-557c-4127-bcf6-4d2ef12ad76f)
