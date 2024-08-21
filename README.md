# ESP32 WiFi Hijack

## Overview
The "ESP32 WiFi Hijack" project is a cybersecurity demonstration using an ESP32 microcontroller. The project creates a fake WiFi network named "Free Wifi airport," which redirects connected users to a custom login page that mimics an airport WiFi portal. The login page captures and stores the credentials (email and password) entered by the users. This project serves as an educational tool to highlight the potential security risks of connecting to unknown WiFi networks.

## Features
- **Fake WiFi Network**: The ESP32 broadcasts a WiFi network named "Free Wifi airport" without requiring a password.
- **Custom Login Page**: Connected users are redirected to a login page designed to look like an official airport WiFi login portal.
- **Data Capture**: The login credentials entered by the users are captured and stored in the ESP32's memory.
- **Download Feature**: The stored login data can be downloaded as a text file.
- **Hidden Admin Page**: An admin page is available to view the captured login data and the number of connected clients.

## Project Structure
- `/data`: Contains static files such as images (e.g., `plane.jpg`).
- `/src`: Contains the main source code (`ESP32_WiFi_Hijack.ino`).
- `README.md`: This file, explaining the project and how to use it.
- `.gitignore`: Optional file to exclude unnecessary files from the repository.
- `LICENSE`: Information about the project's license (e.g., MIT License).

## How It Works
1. **WiFi Setup**: The ESP32 is configured as a WiFi Access Point (AP) with the SSID "Free Wifi airport" and no password.
2. **DNS Server**: A DNS server is set up to intercept DNS requests and redirect all traffic to the ESP32.
3. **Web Server**: The ESP32 hosts a web server that serves the custom login page and handles user requests.
4. **Login Capture**: When users attempt to log in via the fake portal, their credentials are captured and stored in an array.
5. **Admin Access**: The admin can access a hidden page to view the captured credentials and download them as a text file.

### Code Explanation

- **Initialization**: 
  - The ESP32 is initialized with WiFi, DNS, and web server settings.
  - `SPIFFS` is used to store and serve static files such as images.
  - The Access Point is started, and the IP address is printed to the Serial Monitor.

- **Handling Requests**:
  - `handleRoot()`: Redirects users to the custom login page.
  - `handleRedirectPage()`: Serves the login page HTML content.
  - `handleLogin()`: Captures the username and password entered by the user and stores them in the array.
  - `handleHiddenPage()`: Displays the stored login data and the number of connected clients.
  - `handleDownload()`: Allows the admin to download the captured data as a text file.
  - `serveImage()`: Serves the image file used in the login page.
  - `handleNotFound()`: Redirects all other requests to the login page.

## Setup Instructions

### 1. Hardware Requirements
- **ESP32 Microcontroller**: The main hardware component used in this project.

### 2. Software Requirements
- **Arduino IDE**: The development environment used for writing and uploading the code.
- **Libraries**:
  - `WiFi.h`
  - `WebServer.h`
  - `DNSServer.h`
  - `SPIFFS.h`
  - `base64.h`

### 3. Upload Files to SPIFFS
- Place `plane.jpg` or any other image in the `/data` folder.
- Upload the image to the ESP32's SPIFFS using the "ESP32 Sketch Data Upload" tool in Arduino IDE.

### 4. Upload the Code
- Compile and upload the `ESP32_WiFi_Hijack.ino` file to your ESP32 using Arduino IDE.

### 5. Connecting to the Network
- Search for the "Free Wifi airport" network and connect using any WiFi-enabled device.
- You will be redirected to the fake login page.

## Usage
1. **Connecting to the Fake Network**:
   - When users connect to the "Free Wifi airport" network, they are automatically redirected to the login page.

2. **Entering Credentials**:
   - Users are prompted to enter their email and password on the login page.

3. **Accessing the Admin Page**:
   - The admin can access the hidden page by navigating to `/hidden-page` on the network to view the captured login data and connected clients.

4. **Downloading Data**:
   - The captured data can be downloaded as a text file from the admin page.

## Legal Disclaimer
This project is intended for educational purposes only. Unauthorized or malicious use of this project may be illegal. The author is not responsible for any misuse of this project.

## License
This project is licensed under the MIT License. See the [LICENSE](LICENSE) file for more information.
