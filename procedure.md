
## Hardware Setup Installation Procedure

## 1. Workspace Preparation

1. Select a flat, stable working platform.
2. Ensure proper lighting and ventilation.
3. Keep all tools ready: screwdriver set, wire stripper, multimeter, insulation tape, cable ties.
4. Verify availability of regulated power supplies (5V for logic, 12V for motor if required).
5. Disconnect power sources before starting installation.

### Circuit Diagram:
![alt text](image-1.png)

## 2. Mechanical Assembly of Robotic Arm

### Step 1: Base Mounting

* Secure the robotic arm base to a rigid platform using bolts.
* Ensure the base does not wobble during movement.

### Step 2: Servo Motor Installation

* Mount servo motors at designated joints (base, shoulder, elbow, gripper).
* Align servo horns to 90° (neutral position) before tightening screws.
* Avoid overtightening to prevent gear damage.

### Step 3: Gripper Installation

* Attach the gripper mechanism to the final servo shaft.
* Manually test open/close movement before electrical connection.

### Step 4: Motion Clearance Check

* Manually rotate joints to ensure free movement.
* Confirm the arm can reach pickup and placement zones.


## 3. Conveyor Belt Installation

### Step 1: Frame Setup

* Assemble conveyor frame securely.
* Ensure rollers rotate freely.

### Step 2: Motor Mounting

* Fix the DC motor firmly to the frame.
* Align motor shaft with drive roller.
* Tighten coupling screws properly.

### Step 3: Belt Installation

* Place belt over rollers.
* Adjust belt tension to avoid slipping.
* Ensure smooth manual rotation.

### Step 4: Alignment with Robotic Arm

* Position conveyor such that object pickup point lies within arm reach.
* Maintain proper height alignment between conveyor surface and gripper.


## 4. Electrical Installation

### A. ESP32 Mounting

* Mount ESP32 board on insulated base.
* Avoid placing directly on metal surfaces.

### B. Servo Motor Connections

* Connect servo signal wires to assigned ESP32 GPIO pins.
* Connect servo VCC to regulated 5V external supply.
* Connect all servo grounds to common ground.

Important: Do not power multiple servos directly from ESP32.



### C. Color Sensor (TCS3200) Installation

1. Mount sensor above conveyor detection zone.
2. Maintain consistent distance between sensor and object (2–5 cm recommended).
3. Connect:

   * VCC to 5V
   * GND to Ground
   * OUT to ESP32 input pin
   * S0–S3 to ESP32 GPIO pins
4. Secure sensor to prevent vibration.


### D. Relay Module Installation (Conveyor Control)

1. Connect relay VCC to 5V.
2. Connect relay GND to common ground.
3. Connect relay IN to ESP32 GPIO (example GPIO 23).
4. Connect motor power line through COM and NO terminals.
5. Ensure high-voltage wiring is insulated.


### E. Power Supply Setup

* Use separate regulated 5V supply for:

  * ESP32
  * Relay
  * Sensor
* Use separate 12V supply for:

  * Conveyor motor
* Ensure all grounds are common.
* Use fuse protection for motor circuit.



## 5. Wiring Management

1. Use proper color coding for wires.
2. Keep motor wiring separate from signal wiring.
3. Use cable ties to organize connections.
4. Avoid loose connections.
5. Verify continuity using multimeter before powering.


## 6. Initial Power-On Procedure

1. Double-check all connections.
2. Ensure no exposed wires.
3. Power ON logic supply first.
4. Upload ESP32 program.
5. Monitor Serial output.
6. Power ON motor supply.
7. Observe initial movement carefully.


## 7. Calibration Procedure

1. Place red object under sensor.
2. Note RGB frequency values.
3. Repeat for green and blue.
4. Adjust threshold values in program.
5. Adjust servo angles for accurate pickup.
6. Test multiple cycles.



## 8. Final System Testing

* Run conveyor.
* Verify object detection.
* Confirm conveyor stops at detection.
* Observe robotic arm movement.
* Ensure correct sorting.
* Test minimum 10–15 continuous cycles.


## 9. Safety Precautions

* Do not touch moving parts during operation.
* Avoid overloading servos.
* Use insulated connectors for high voltage.
* Disconnect power during maintenance.
* Ensure relay rating matches motor current.



## 10. Installation Completion Checklist

* Robotic arm securely mounted
* Conveyor aligned correctly
* Sensor calibrated
* Relay functioning properly
* Wiring insulated and organized
* System tested successfully
