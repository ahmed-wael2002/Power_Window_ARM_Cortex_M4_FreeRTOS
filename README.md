# Power Window Control System

### 🚗 Project Overview
This project showcases a **Power Window Control System** designed for modern vehicles using the **Tiva C microcontroller** and **FreeRTOS**. The system is scalable, configurable, and integrates various functionalities such as manual and automatic window control, obstacle detection, and safety features. It ensures compatibility across different vehicle models.

### ✨ Key Features
- **Manual & Automatic Control:**
  - **Manual Mode:** Continuous press for precise window control.
  - **Automatic Mode:** One-touch for full open/close functionality.
- **Driver and Passenger Control Board:**
  - Provides window control from driver control board (higher priority) or passenger control board.
- **Window Lock Function:**
  - Locks all windows except the driver's for enhanced security.
- **Jam Protection:**
  - Detects obstacles during the one-touch auto-close operation then retreats the window for safety.
- **Highly Configurable:**
  - Provides manual configuration through MACROS for adapting the project into any hardware architecture.
- **Templates:**
  - Provides sample demo project for easier integration.

 ---

### ✨ Technologies Used
- FreeRTOS (Project built on ARM Keil)
- TM4C123GH6PM (Tiva C Launchpad - ARM Cortex M4)

---

### 🛠️ System Architecture
- **Application Layer:** 
  - Contains FreeRTOS kernel files, `windows.h`, and `main.c` where window control tasks are defined.
- **Hardware Abstraction Layer:**
  - Interfaces with the motor and interrupt hardware. Supports different motor types.
- **Task Structure:**
  - Includes modular tasks for window control, motor operation, and jam protection, which can be easily instantiated and scaled.
 
---

### 🔧 Demonstration
- **Hardware Used:**
  - Dual H-Bridge Motor Driver
  - DC Gearbox Motors with Back Shaft
  - Micro Limit Switches
  - 3-level Rocker Switch
  - Push Buttons
  - Tiva C Launchpad (ARM Cortex M4-based microcontroller)
- **Circuit Wiring:**
  - Circuit diagram created using Fritzing for easy visualization.
 
 --- 

### 💻 Software System Integration
- **System Configurations:**
  - Supports 2-window (coupe) and 4-window (sedan) setups.
- **Main Function Setup:**
  - Code initializes tasks and hardware components to manage the window control system seamlessly.

### 📂 Additional Resources
- **[📹 Video Demonstration & Documentation](https://drive.google.com/file/d/1O1Iedp_n3LcsQSbOaQz3eCcksW22hzNp/view?usp=sharing)**
- **[💻 GitHub Repository](https://github.com/ahmed-wael2002/Power_Window_ARM_Cortex_M4_FreeRTOS)**

For more detailed documentation and code examples, visit the [GitHub repository](https://github.com/ahmed-wael2002/Power_Window_ARM_Cortex_M4_FreeRTOS).

---
