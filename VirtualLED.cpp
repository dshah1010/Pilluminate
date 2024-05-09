/**
 * @file VirtualLED.cpp
 * @brief Implementation of the VirtualLED class.
 * @details This file contains the implementation details of the VirtualLED class, including methods for changing its state, color, and blinking behavior. It uses Qt framework classes for the GUI representation.
 * @see VirtualLED.h for the declaration of the VirtualLED class.
 * @author Group 3
 */

#include "include/models/VirtualLED.h"

// Including necessary modules.
#include <QPainter>
#include <QColorDialog>
#include <QStyle>
#include <QTimer>
#include <QInputDialog>

/**
 * @class VirtualLED
 * @brief The VirtualLED class simulates an LED's behavior with visual representation.
 * @details This class provides methods to simulate an LED's behavior in a graphical user interface, including turning it on/off, changing its color, and making it blink.
 * @author Group 3
 */

/**
 * @brief Constructs a VirtualLED widget.
 * @details Initializes the LED with a specific ID, sets up timers for blinking and turning off, and configures the widget's appearance.
 * @param id The identifier for the VirtualLED.
 * @param parent The parent widget.
 */
VirtualLED::VirtualLED(int id, QWidget *parent) : QWidget(parent), currentColor(Qt::transparent), ledId(id), state(false), blinkOn(true), offTimer(new QTimer(this)) {

    setFixedSize(50, 50); // Set fixed size for the LED widget.
    // Set the LED's default style.
    setStyleSheet("VirtualLED {" 
                  "border: 3px solid black;"  
                  "border-radius: 25px;"      
                  "}");

    blinkTimer = new QTimer(this); // Initialize the blinking timer.
    // Connect the blinkTimer's timeout signal to toggle the blinkOn state and update the widget.
    connect(blinkTimer, &QTimer::timeout, this, [this]() {
        blinkOn = !blinkOn; 
        update(); 
    });

    blinkSpeed = 0; // Initialize blinking speed to 0 (off)
    connect(offTimer, &QTimer::timeout, this, &VirtualLED::turnOff); // Connect the offTimer's timeout signal to the turnOff method.

}

/**
 * @brief Sets the color of the VirtualLED.
 * @details Changes the LED's color and updates its state. If the color is set to something other than transparent, the LED is considered "on".
 * @param color The color to set the LED to.
 */
void VirtualLED::setColor(const QColor &color) {
    currentColor = color;
    bool prevState = state;  
    state = (color != Qt::transparent); // Determine the state based on color.
    update(); // Trigger a repaint to reflect color change.
    if (state && !prevState) {qDebug() << "LED #" << ledId << "turned on.";} // Log LED state change.
}

/**
 * @brief Gets the identifier of the VirtualLED.
 * @details Returns the unique identifier for this LED instance, allowing it to be distinguished from others.
 * @return Returns the identifier of the LED.
 */
int VirtualLED::getId() const {
    return ledId;
}

/**
 * @brief Sets the identifier of the VirtualLED.
 * @details Updates the internal identifier for this LED. This can be used to reassign the LED's ID after creation.
 * @param newId The new identifier for the LED.
 */
void VirtualLED::setId(int newId) {
    ledId = newId; // Update the LED's identifier.
}

/**
 * @brief Checks if the LED is currently on.
 * @details Evaluates the LED's current state based on its color. An LED is considered "on" if its color is not transparent.
 * @return True if the LED is on, false otherwise.
 */
bool VirtualLED::isOn() const {
    return currentColor != Qt::transparent; // The LED is considered on if its currentColor is not transparent.
}

/**
 * @brief Turns the LED on.
 * @details Activates the LED, setting its color to white by default and marking its state as "on".
 */
void VirtualLED::turnOn() {
    if (!state) { // Only turn on if currently off.
        state = true;
        blinkOn = true; // Ensure blinking state is reset to true.
        setColor(Qt::white); // Default color when turning on is white.
        offTimer->stop(); // Stop the off timer to prevent it from turning the LED off immediately.
        qDebug() << "LED #" << ledId << "turned on.";
    }
}

/**
 * @brief Turns the LED off.
 * @details Deactivates the LED by setting its color to transparent, effectively rendering it "off". This also stops any ongoing blinking effect.
 */
void VirtualLED::turnOff() {
    if (state) { // Only turn off if currently on.
        state = false;
        blinkTimer->stop(); // Stop blinking.
        blinkOn = true; // Reset blinking state.
        setColor(Qt::transparent); // Set color to transparent to indicate off state.
        qDebug() << "LED #" << ledId << "turned off.";
    }
}

/**
 * @brief Sets the blinking speed of the LED.
 * @details Adjusts the blinking frequency of the LED. A non-zero speed sets the LED to blink at that interval, while a speed of zero keeps the LED constantly on without blinking.
 * @param speed The blinking speed in milliseconds. A speed of 0 stops the blinking.
 */
void VirtualLED::setBlinkSpeed(int speed) {
    blinkSpeed = speed; // Update blink speed.
    blinkTimer->stop(); // Stop the current blinking.
    if (blinkSpeed > 0) {blinkTimer->start(blinkSpeed);} // Start blinking at the new speed.
    else {
        blinkOn = true; // Ensure the LED is shown as constantly on if speed is 0.
        update(); // Update the LED's appearance.
    }
}

/**
 * @brief Gets the current blinking speed of the LED.
 * @details Returns the current blinking interval of the LED. A return value of 0 indicates that the LED is not blinking.
 * @return The blinking speed in milliseconds. Returns 0 if the LED is not blinking.
 */
int VirtualLED::getBlinkSpeed() const { 
    return blinkSpeed; 
}

/**
 * @brief Sets the duration for how long the LED stays on.
 * @details Schedules the LED to turn off after a specified duration. This allows for automatic deactivation of the LED after a certain period.
 * @param seconds The duration in seconds. After this time, the LED will turn off.
 */
void VirtualLED::setDuration(int seconds) {
    if(seconds > 0) {
        offTimer->start(seconds * 1000); // Start the timer with the specified duration.
    }
}

/**
 * @brief Stops the off timer for the LED.
 * @details This function stops the timer responsible for turning the LED off after a set duration. It is useful when you need to manually turn an LED on and ensure it stays on, regardless of any previous duration settings. This is particularly important for operations that require an LED to remain on without being automatically turned off by the timer.
 */
void VirtualLED::stopOffTimer() {
    offTimer->stop();
}

/**
 * @brief Handles mouse press events to toggle the LED on or off.
 * @details Allows the user to manually toggle the LED's state by clicking on it. A left mouse button click changes the state from on to off, or vice versa.
 * @param event The mouse event.
 */
void VirtualLED::mousePressEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        if (state) {turnOff();} // If the LED is on, turn it off.
        else {turnOn();} // If the LED is off, turn it on.
    }
}

/**
 * @brief Paints the LED widget.
 * @details Renders the LED on the widget, using the current color or a dimmed version of it to indicate the blinking state. The LED is drawn as an ellipse.
 * @param * A pointer to the paint event.
 */
void VirtualLED::paintEvent(QPaintEvent *) {
    QPainter painter(this);
    QColor drawColor = blinkOn ? currentColor : QColor(currentColor.red(), currentColor.green(), currentColor.blue(), 50); // Determine the color to draw, based on the blinking state.
    painter.setBrush(drawColor);
    painter.drawEllipse(rect().adjusted(1, 1, -1, -1)); // Draw the LED as an ellipse with adjusted dimensions for border
}

/**
 * @brief Creates a context menu for the LED widget.
 * @details Generates a right-click context menu with options to change the LED's color, set its blinking speed, specify a duration for it to remain on, or remove the LED entirely. Options are context-sensitive, based on the LED's state.
 * @param event The context menu event.
 */
void VirtualLED::contextMenuEvent(QContextMenuEvent *event) {

    QMenu menu(this); 
    QAction *removeAction = menu.addAction("Remove"); // Option to remove the LED.

    if (state) { // Only show additional options if the LED is on.

        QAction *colorAction = menu.addAction("Change Color");
        connect(colorAction, &QAction::triggered, this, [this](){ 
            QColor selectedColor = QColorDialog::getColor(currentColor, this, "Select LED Color"); 
            if (selectedColor.isValid()) { 
                setColor(selectedColor); // Change the LED's color.
                emit colorChanged(ledId, selectedColor);
            }
        });

        QAction *blinkSpeedAction = menu.addAction("Set Blinking Speed"); 
            connect(blinkSpeedAction, &QAction::triggered, this, [this]() {
            bool ok; 
            int speed = QInputDialog::getInt(this, "Set Blinking Speed", "Speed (ms):", blinkSpeed, 0, 10000, 1, &ok); // Prompt the user to enter a new blinking speed with a dialog.
            if (ok) { // If the user pressed OK, update the blinking speed.
                setBlinkSpeed(speed); 
                qDebug() << "LED #" << ledId << "blinking speed set to" << speed << "ms."; 
            }
        });

        QAction *setDurationAction = menu.addAction("Set Duration");
        connect(setDurationAction, &QAction::triggered, this, [this]() {
            bool ok;
            int duration = QInputDialog::getInt(this, "Set Duration", "Duration (seconds):", 0, 1, 3600, 1, &ok); // Prompt the user to enter a duration after which the LED should turn off.
            if (ok) { // If the user pressed OK, set the duration.
                setDuration(duration);
                qDebug() << "LED #" << ledId << "duration set to" << duration << "seconds."; 
            }
        });

    }

    QAction *selectedAction = menu.exec(event->globalPos()); 
    if (selectedAction == removeAction) {emit removed(ledId);} // If the remove action is selected, emit the removed signal.

}
