#include <avr/io.h>
#include <avr/interrupt.h>
#include <Wire.h>
#include <AccelStepper.h>
#include <avr/wdt.h>

//--------------------------------------Variables and declerations------------------------------
// Potentiometer used for setpoint and the Plasma Arc to enable the whole code
const int potPin = A0;
const int ArcON = 2;
const int onDelay = 400, stableVoltageDelay = 25;
bool firstScan = true;

// Stepper Motor, Pins, variables and stepper device
const int stepPin = 9;
const int dirPin = 10;
const int homeLimitSwitch = 11;
const int homingAcc = 500, homingMaxSpeed = 1000, homingSpeed = -750;
const int operationAcc = 1500, operationMaxSpeed = 3500, operationSpeed = 3000;
const long minPosition = 0;      // Minimum position limit
const long midPosition = 7500;   // Mid position for the operation
const long maxPosition = 15000;  // Maximum position limit
long currentPos = 0, newPos = 0; // Variable used in PID loop
int posAdjstment = 0;
AccelStepper stepper(AccelStepper::DRIVER, stepPin, dirPin);

// PID parameters
float Kp = 0.5, Ki = 0.01, Kd = 5;
float KpAgr = 10, KiAgr = 1, KdAgr = 50;
float error = 0, lastError = 0, maxError = 15;
float integral = 0, derivative = 0;
float derivativeFilter = 0.1;
const float maxIntegral = 200.0;  // Maximum allowable integral term to prevent windup
const int errorTolerance = 1;
unsigned long lastTime = 0;
const long interval = 20;        // Control loop interval in milliseconds
float averageVoltage = 0, averageArcVoltage = 0;
float setPoint = 100.0;

// ADS1115 Settings
#define ADS1115_ADDRESS 0x49
#define ADS1115_REG_POINTER_CONVERT 0x00
#define ADS1115_REG_POINTER_CONFIG 0x01
// Combined configuration for continuous mode & differential between AIN0 and AIN1
const uint16_t adsConfig = 0x8223;
float readings[3] = { 0.0, 0.0, 0.0 };  // Array to store the last 3 readings
size_t index = 0;                       // Index for inserting the next reading
//--------------------------------------Variables and declerations------------------------------

//--------------------------------------Helper Functions----------------------------------------
void pidIteration() {
  unsigned long currentTime = millis();
  if (currentTime - lastTime >= interval) {
    lastTime = currentTime;

    // Read the arc voltage
    averageArcVoltage = readArcVoltage();
    // skip if the value is not valid
    if (averageArcVoltage == -100.00) { return; }

    // PID error calculation
    error = setPoint - averageArcVoltage;

    // PID controller only activates if error exceeds tolerance
    if (abs(error) > errorTolerance) {
      // Integral term calculation with anti-windup
      integral += error * interval;
      // Prevents integral windup
      integral = constrain(integral, -maxIntegral, maxIntegral);
      // Derivative term calculation with filtering
      derivative = ((error - lastError) / interval) * derivativeFilter + derivative * (1 - derivativeFilter);
      // PID adjustment calculation and application
      if(abs(error) >= maxError){
        posAdjstment = (KpAgr * error + KiAgr * integral + KdAgr * derivative);
      }
      else{
        posAdjstment = (Kp * error + Ki * integral + Kd * derivative);
      }

      // Apply corrections with stepper
      currentPos = stepper.currentPosition();
      newPos = currentPos - posAdjstment;
      if (newPos >= maxPosition) { newPos = maxPosition; }
      if (newPos <= minPosition) { newPos = minPosition; }
      // Perform regular updates
      stepper.moveTo(newPos);
      // stepper.runToPosition(); now done by Timer2 interrupt

      Serial.print("Average Voltage: ");
      Serial.print(averageArcVoltage);
      Serial.print("\tStepper posAdjstment: ");
      Serial.println(posAdjstment);
      
      lastError = error;
    }
  }
}

void configADS1115Continuous() {
  Wire.beginTransmission(ADS1115_ADDRESS);
  Wire.write(ADS1115_REG_POINTER_CONFIG);
  Wire.write((adsConfig >> 8) & 0xFF);  // MSB first
  Wire.write(adsConfig & 0xFF);         // LSB second
  if (Wire.endTransmission() != 0) {
    Serial.println("Failed to configure ADS1115.");
  }
}

float readArcVoltage() {
  float voltage = readADS1115Continuous();  // Read voltage
  if (voltage < 0 || voltage > 4.096) {     // Fail-safe check for voltage
    Serial.println("Voltage failure detected. Voltage out of bounds.");
    return -100.00;  // Skip this loop iteration
  } else {
    averageVoltage = calculateMovingAverage(voltage);  // Calculate moving average
    return mapArcVoltage((averageVoltage * 1000));     // Convert (0V - 4.096V) to Arc Voltage
  }
}

float readADS1115Continuous() {
  const int maxRetries = 3;  // Maximum number of retries for I2C communication
  int retries = 0;
  float voltage = 0.0;

  while (retries < maxRetries) {
    Wire.beginTransmission(ADS1115_ADDRESS);
    Wire.write(ADS1115_REG_POINTER_CONVERT);
    if (Wire.endTransmission() != 0) {
      Serial.println("Error sending read request.");
      retries++;
      continue;  // Try sending the request again
    }

    Wire.requestFrom(ADS1115_ADDRESS, (uint8_t)2);
    if (Wire.available() == 2) {
      int16_t value = (Wire.read() << 8) | Wire.read();
      voltage = (value * 4.096) / 32768.0;  // Convert to voltage assuming +/-4.096V range
      break;                                // Success, exit the loop
    } else {
      Serial.println("Failed to read data.");
      retries++;
    }
  }

  if (retries == maxRetries) {
    Serial.println("I2C communication with ADS1115 failed after retries.");
    return -1.0;  // Indicate failure
  }

  return voltage;
}

float calculateMovingAverage(float newVoltage) {
  // Update the readings array with the new voltage and calculate the moving average
  readings[index] = newVoltage;
  index = (index + 1) % 3;  // Move to the next position in the array, wrapping around

  float sum = 0.0;
  for (int i = 0; i < 3; i++) {
    sum += readings[i];
  }
  return sum / 3.0;
}

// Convert sensor volateg to arc voltage function
float mapArcVoltage(float voltage) {
  return (0.0932 * voltage) - 0.9841;
}

// Home the stepper motor
void homeStepper() {
  Serial.println("Homing...");
  stepper.setAcceleration(homingAcc);
  stepper.setMaxSpeed(homingMaxSpeed);
  stepper.setSpeed(homingSpeed);
  stepper.runSpeed();
  while (digitalRead(homeLimitSwitch) == HIGH) {
    stepper.runSpeed();
  }
  stepper.stop();
  stepper.setCurrentPosition(0);
  stepper.move(400);
  stepper.runToPosition();
  stepper.setCurrentPosition(minPosition);
  Serial.println("Homing complete.");
}

// Set the speed and acceleration of the stepper motor for THC operation
void setupStepperOpr() {
  // Set the speed and acceleration of the stepper motor for THC operation
  stepper.setMaxSpeed(operationMaxSpeed);
  stepper.setAcceleration(operationAcc);
  stepper.setSpeed(operationSpeed);
}

// Move the stepper motor to the middle
void stepperToMidPosition() {
  stepper.moveTo(midPosition);
  stepper.run();
  Serial.println("Stepper at the mid point.");
}

// Simplify the setup and configuration functions for brevity.
void setupTimer2Interrupt() {
  // Configure timer2 interrupts
  // Interval = (OCR2A + 1) / (clock-freq/pre-scaler)
  noInterrupts();  // Disable all interrupts
  TCCR2A = 0;      // Clear Timer2 Control Register A
  TCCR2B = 0;      // Clear Timer2 Control Register B
  TCNT2 = 0;       // Initialize the counter value to 0
  // Set compare match register to 39 (20 uSec)
  OCR2A = 39;
  // Turn on CTC mode
  TCCR2A |= (1 << WGM21);
  // Set CS21 bit for 8 prescaler
  TCCR2B |= (1 << CS21);
  // Enable timer compare interrupt
  TIMSK2 |= (1 << OCIE2A);
}

// Timer2 Interrupt Service Routine
ISR(TIMER2_COMPA_vect) {
  stepper.run();
}
//--------------------------------------Helper Functions----------------------------------------

//--------------------------------------Main Functions------------------------------------------
void setup() {
  wdt_disable();                           // Disable watchdog timer
  Wire.begin();                            // Initialize I2C
  pinMode(potPin, INPUT);                  // Pot enables the user to set the value for the setpoint
  pinMode(ArcON, INPUT);                   // sets the control pin as an input
  pinMode(homeLimitSwitch, INPUT_PULLUP);  // sets the limit switch as input to home the stepper motor
  Serial.begin(115200);                    // Initialize Serial communication
  configADS1115Continuous();               // Configure ADS1115 for continuous mode

  homeStepper();
  setupStepperOpr();
  stepperToMidPosition();

  setupTimer2Interrupt();
  interrupts();                            // Enable all interrupts
  wdt_enable(WDTO_2S);                     // Enable the watchdog timer for 2-second timeout
}

void loop() {
  wdt_reset();  // Regularly reset the watchdog timer to prevent system reset

  // Cut started.
  while (digitalRead(ArcON) == HIGH) {
    if (firstScan == true) {
      delay(onDelay);
      firstScan = false;
      // To get a stable arc voltage
      averageArcVoltage = readArcVoltage();
      delay(stableVoltageDelay);
      averageArcVoltage = readArcVoltage();
      delay(stableVoltageDelay);
      averageArcVoltage = readArcVoltage();
      Serial.println("ArcON First Scan");
    }
    pidIteration();         // PID Loop
    wdt_reset();  // Regularly reset the watchdog timer to prevent system reset
  }

  // Cut completed.
  if (firstScan == false) {
    firstScan = true;
    stepperToMidPosition();  // Move stepper to the middle
    Serial.println("ArcOFF First Scan");
  }
}
//--------------------------------------Main Functions------------------------------------------