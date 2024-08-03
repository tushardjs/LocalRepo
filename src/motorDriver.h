#include <Arduino.h>
#include <Bounce2.h>

// Defines a class for controlling a motor and monitoring limit switches
class MotorDriver
{
private:
    uint8_t motorPin_1;          // Pin controlling motor direction 1
    uint8_t motorPin_2;          // Pin controlling motor direction 2
    uint8_t openLimitSwitchPin;  // Pin connected to the open limit switch
    uint8_t closeLimitSwitchPin; // Pin connected to the close limit switch

    Bounce2::Button openLimitSwitch;  // Bounce2 object for open limit switch
    Bounce2::Button closeLimitSwitch; // Bounce2 object for close limit switch

public:
    // Constructor to initialize motor and limit switch pins
    MotorDriver(uint8_t motorpin1, uint8_t motorpin2, uint8_t openLimitSwitchPin, uint8_t closeLimitSwitchPin);

    // Method to configure pins and initialize limit switches
    void begin();

    // Method to rotate the valve clockwise
    void valveClockWise();

    // Method to rotate the valve counterclockwise
    void valveCounterClockWise();

    // Method to stop the motor
    void stop();

    // Method to update the state of limit switches
    void loop();

    // Flags indicating the state of limit switches
    bool isOpenLimitSwitchPressed = false;
    bool isCloseLimitSwitchPressed = false;
};

// Constructor implementation
MotorDriver::MotorDriver(uint8_t motorpin_1, uint8_t motorpin_2, uint8_t openLimitSwitchPin, uint8_t closeLimitSwitchPin)
    : motorPin_1(motorpin_1), motorPin_2(motorpin_2), openLimitSwitchPin(openLimitSwitchPin), closeLimitSwitchPin(closeLimitSwitchPin) {}

// Configures motor pins and initializes limit switches
void MotorDriver::begin()
{
    pinMode(motorPin_1, OUTPUT); // Set motorPin_1 as an output
    pinMode(motorPin_2, OUTPUT); // Set motorPin_2 as an output

    // Attach the limit switches to their respective pins with pull-up resistors
    openLimitSwitch.attach(openLimitSwitchPin, INPUT_PULLUP);
    closeLimitSwitch.attach(closeLimitSwitchPin, INPUT_PULLUP);

    // Set debounce interval for the limit switches
    openLimitSwitch.interval(20);
    closeLimitSwitch.interval(20);

    // Set the pressed state for the limit switches
    openLimitSwitch.setPressedState(LOW);  // Set the pressed state LOW for openLimitSwitch
    closeLimitSwitch.setPressedState(LOW); // Set the pressed state LOW for closeLimitSwitch
}

void MotorDriver::loop()
{ // Updates the state of limit switches and sets the corresponding flags
    openLimitSwitch.update();
    closeLimitSwitch.update();
    // Update flags based on the current state of the limit switches
    isOpenLimitSwitchPressed = openLimitSwitch.isPressed();
    isCloseLimitSwitchPressed = closeLimitSwitch.isPressed();
}

// Rotates the valve clockwise if the open limit switch is not pressed
void MotorDriver::valveClockWise()
{
    if (!isOpenLimitSwitchPressed)
    {
        digitalWrite(motorPin_1, HIGH); // Set motorPin_1 high to rotate the valve clockwise
        digitalWrite(motorPin_2, LOW);
        Serial.println("Valve is opening ");
    }
    else
    {
        Serial.println("Valve is opened ");
    }
}

// Rotates the valve counterclockwise if the close limit switch is not pressed
void MotorDriver::valveCounterClockWise()
{
    if (!isCloseLimitSwitchPressed)
    {
        digitalWrite(motorPin_1, LOW);  // Set motorPin_1 low
        digitalWrite(motorPin_2, HIGH); // Set motorPin_2 high to rotate the valve counterclockwise
        Serial.print("Valve is closing");
    }
    else
    {
        Serial.print("Valve is closed");
    }
}

// Stops the motor by setting both motor pins low
void MotorDriver::stop()
{
    digitalWrite(motorPin_1, LOW);
    digitalWrite(motorPin_2, LOW);
    Serial.println("motor is stoped");
}
