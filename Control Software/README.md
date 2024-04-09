# Plasma Cutter Torch Height Controller (THC) - Code Overview

This repository contains the Arduino sketch for a Torch Height Controller (THC) designed for plasma cutting applications. The software controls a stepper motor to adjust the height of the plasma torch in real-time, ensuring optimal cutting quality by maintaining a consistent distance from the workpiece.

## Main Features

-   **Dynamic Height Adjustment**: Utilizes a PID control loop to adjust the torch height based on voltage feedback from the plasma arc.
-   **Stepper Motor Control**: Employs the `AccelStepper` library for precise control over the stepper motor, facilitating smooth and accurate height adjustments.
-   **Surface Probing**: Integrates a compressible Z-axis design for initial surface detection, ensuring accurate starting conditions for the cutting process.
-   **I2C Communication**: Reads voltage measurements from an ADS1115 ADC module for high-precision feedback.
-   **Watchdog Timer**: Implements a watchdog timer for system reliability, ensuring the controller recovers from any software malfunctions.

## File Structure

-   `THC_Stepper_V0_3_1a.ino`: The main Arduino sketch file that contains the setup and loop functions, along with the implementation of the PID control loop, stepper motor control, and initial surface probing.

## Getting Started

### Prerequisites

-   Arduino IDE (or compatible IDE) for compiling and uploading the sketch to an Arduino Nano.
-   An Arduino Nano, or a compatible microcontroller, flashed with this sketch.
-   A4988/DRV8825/TMC2130 or any other driver stepper motor driver module.
-   NEMA 17 stepper motor.
-   ADS1115 ADC module for arc voltage feedback.
-   Limit switch for the Z-axis homing function and another one for probing.

### Hardware Setup

1.  **Stepper Motor Connection**: Connect the NEMA 17 stepper motor to the DRV8825 driver. Ensure the driver is properly configured for the motor's current requirements.
2.  **ADS1115 Module**: Wire the ADS1115 module to the Arduino Nano via I2C (SDA to A4, SCL to A5 on Nano).
3.  **Limit Switch**: Connect the limit switch to the designated pin on the Arduino Nano for the homing function.
4.  **Power Supply**: Ensure the power supply meets the requirements for the stepper motor and the plasma cutter's control circuit.

### Software Configuration

1.  **Library Installation**: Install the `AccelStepper` library through the Arduino IDE Library Manager.
2.  **Adjust Parameters**: Review the sketch's configurable parameters, such as PID constants, stepper motor speeds, and acceleration settings, to match your specific hardware setup.
3.  **Compile and Upload**: Compile the sketch and upload it to your Arduino Nano.

## Usage

Upon powering up, the system will execute a homing routine to establish a known reference position. During plasma cutting operations, the system dynamically adjusts the torch height based on real-time feedback from the arc voltage sensor, ensuring consistent cut quality.

## Contributing

Contributions to this project are welcome! Please refer to the contributing guidelines for more information on how to report issues, suggest enhancements, or submit pull requests.
## License
This project is licensed under the GNU-v3 License - see the LICENSE file for details.

# Disclaimer
This Plasma Cutter Torch Height Controller (THC) project is provided as-is, without warranty of any kind, express or implied. The authors and contributors to this project assume no liability for any direct, indirect, incidental, special, exemplary, or consequential damages, including but not limited to procurement of substitute goods or services; loss of use, data, or profits; or business interruption, however caused and on any theory of liability, whether in contract, strict liability, or tort (including negligence or otherwise) arising in any way out of the use of this software and hardware, even if advised of the possibility of such damage.

## Safety Warning
The use of this project involves the operation of high-power equipment and potentially hazardous processes. It is intended for use by individuals with a thorough understanding of these risks and the appropriate safety procedures. Before using this project, you should:

- Ensure you are familiar with the operation and safety requirements of plasma cutters and related equipment.
- Comply with all applicable safety standards and regulations for your region and application.
- Wear appropriate personal protective equipment (PPE) and ensure that any bystanders are also protected.
- Verify that the installation, electrical supply, and grounding of your equipment comply with manufacturer specifications and local safety standards.

## Usage Agreement
By using this project, you acknowledge that you have read and understood this disclaimer, and you agree to be bound by its terms. You assume full responsibility and risk for the assembly, installation, and operation of this THC system.

### Support Us:
[Patreon](https://patreon.com/SmartCut?utm_medium=unknown&utm_source=join_link&utm_campaign=creatorshare_creator&utm_content=copyLink)

[Buy me a ☕](https://www.buymeacoffee.com/smartcut)