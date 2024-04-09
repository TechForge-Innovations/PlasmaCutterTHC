# Plasma Cutter Torch Height Controller (THC)
This project develops a standalone Torch Height Controller (THC) for plasma cutting applications, designed to ensure optimal cutting quality by dynamically adjusting the torch height based on the voltage feedback from the cutting arc. Utilizing an Arduino Nano as the core controller, the system integrates with a DRV8825 stepper motor driver to control the Z-axis movement, providing precise height adjustments.

## Key Features
- Dynamic Height Adjustment: Implements a PID control loop to adjust the torch height in real-time, enhancing cut quality.
- ADS1115 Integration: Utilizes the ADS1115 ADC for high-precision voltage measurement from the plasma arc, serving as the feedback for the PID controller.
- Stepper Motor Control: Employs the AccelStepper library for smooth and accurate control of the stepper motor, ensuring precise and responsive height adjustments.
- Non-Blocking Design: Uses timer interrupts for stepper motor control, allowing the main program loop to focus on PID calculations and sensor readings without delays.
- Robust Homing Routine: Features a homing routine using a limit switch, ensuring the system starts from a known position for reliable operation.
- Configurable Settings: Offers adjustable parameters for PID tuning, stepper motor speeds, and acceleration, allowing customization for different plasma cutters and materials.
- Safety Features: Implements error handling for I2C communication and uses a watchdog timer to enhance the system's reliability and safety.

## System Components
- Arduino Nano
- (A4988/DRV8825/TMC2130 standalone mode) Stepper Motor Driver
- NEMA 17 Stepper Motor
- ADS1115 ADC Module
- Limit Switch for Homing
- Potentiometer for Manual Setpoint Adjustment

## Application
The THC system is designed for hobbyist and professional plasma cutting tables where maintaining an optimal distance between the torch and workpiece is crucial for cut quality. The system is especially beneficial for cutting uneven materials or where the workpiece surface height varies.

## Getting Started
This section should include detailed setup instructions, wiring diagrams, and steps to compile and upload the code to the Arduino Nano. Additionally, explain how to calibrate the system for a specific plasma cutter and material thickness.

## Contributing
We welcome contributions and suggestions! Please see the CONTRIBUTING.md file for how to report issues, submit ideas, or contribute code.

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
