# 🌕 Lunar Rover Communication Protocol  
**Computer Networks Project – Group 4**

[🔗 GitHub Repository](https://github.com/RTurn17/ComputerNetworks-Group4)

---

## Overview  
This project simulates a robust **communication protocol** between a **Lunar Rover** and **Earth-based mission control**, designed with modularity, fault tolerance, and real-time responsiveness in mind. The system operates across **5 distinct ports**, each handling a specialized task.

### Port Breakdown:

1. ** Movement** – Receives and interprets directional movement commands.
2. ** Telemetry** – Sends rover vitals and sensor readings to Earth.
3. ** Temperature File Transfer** – Transmits log files with internal and external temperature data.
4. ** Error Simulation** – Tests how the system responds to simulated communication errors (e.g., delay, corruption).
5. ** Rover Discovery** – Identifies and pings nearby rovers for collaboration and data exchange.

---

## Features

-  **Multi-Port Communication**  
  Simultaneous handling of multiple data channels with isolated responsibilities.

-  **Error Handling & Simulation**  
  Injection of packet loss, delay, or corruption to stress-test the protocol.

-  **Real-Time Telemetry**  
  Live feedback on rover status, including position, energy levels, and sensor data.

-  **Rover-to-Rover Discovery**  
  Enables local peer discovery for cooperative behavior or load sharing.

-  **Secure Protocol Design**  
  Includes basic authentication and checksum validation to ensure reliable data transmission.

---

## Built With

- **C / C++** for low-level socket programming
- **Python** for test scripts and utilities
- **TCP/UDP Sockets** for communication
