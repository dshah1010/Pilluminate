/**
 * @file VirtualLED.h
 * @brief Defines the VirtualLED class.
 * @details This header file contains the declaration of the VirtualLED class, which simulates an LED on a graphical user interface. It includes functionalities for changing LED color, turning it on or off, blinking with adjustable speed, and setting a duration for the LED to stay on. The VirtualLED class extends QWidget, making it suitable for use in Qt-based applications.
 * @author Group 3
 */

#ifndef VIRTUALLED_H
#define VIRTUALLED_H

// Including necessary modules.
#include <QWidget>
#include <QColor>
#include <QContextMenuEvent>
#include <QMenu>
#include <QAction>
#include <QTimer>

/**
 * @class VirtualLED
 * @brief This class represents a virtual LED component.
 * @details A VirtualLED simulates an LED light with customizable properties such as color, blinking speed, and duration control. It provides a visual representation of an LED and can be used in graphical user interfaces.
 * @author Group 3
 */
class VirtualLED : public QWidget { 

    Q_OBJECT

public:

    /**
     * @brief Constructor for VirtualLED.
     * @details Initializes a new instance of VirtualLED with a specified ID and an optional parent widget.
     * @param id The ID of the LED.
     * @param parent The parent widget.
     */
    explicit VirtualLED(int id, QWidget *parent = nullptr); 

    /**
     * @brief Sets the color of the LED.
     * @details Changes the current color of the LED to the specified QColor. This affects the visual representation of the LED.
     * @param color The color to set the LED to.
     */
    void setColor(const QColor &color); 

    /**
     * @brief Gets the ID of the LED.
     * @details Returns the unique identifier of the VirtualLED instance. This ID can be used to distinguish between multiple LEDs.
     * @return int The ID of the LED.
     */
    int getId() const; 

    /**
     * @brief Sets the ID of the LED.
     * @details Updates the ID of the VirtualLED. Useful for reassigning identifiers within a collection of LEDs.
     * @param newId The new ID for the LED.
     */
    void setId(int newId); 

    /**
     * @brief Checks if the LED is on.
     * @details Determines whether the LED is currently in the "on" state. Does not account for blinking state.
     * @return bool True if the LED is on, false otherwise.
     */
    bool isOn() const; 

    /**
     * @brief Turns the LED on.
     * @details Sets the state of the LED to "on", causing it to display its color if it was previously off.
     */
    void turnOn(); 

    /**
     * @brief Turns the LED off.
     * @details Sets the state of the LED to "off", causing it to not display its color.
     */
    void turnOff(); 

    /**
     * @brief Sets the blinking speed of the LED.
     * @details Adjusts how quickly the LED blinks on and off. A lower value results in faster blinking.
     * @param speed The blinking speed in milliseconds.
     */
    void setBlinkSpeed(int speed); 

    /**
     * @brief Gets the blinking speed of the LED.
     * @details Returns the current blinking speed of the LED. A higher value indicates a slower blink rate.
     * @return int The blinking speed in milliseconds.
     */
    int getBlinkSpeed() const; 

    /**
     * @brief Sets the duration for which the LED stays on.
     * @details Specifies how long the LED should remain on before automatically turning off. Useful for timed indicators.
     * @param seconds The duration in seconds.
     */
    void setDuration(int seconds); 

    /**
     * @brief Stops the off timer.
     * @details Stops the timer that is responsible for automatically turning off the LED after a specified duration. This method should be used when the LED's automatic turn-off behavior needs to be halted, for instance, when an LED is manually turned on and should not turn off due to a previously set duration. Calling this method ensures that the LED stays on until explicitly turned off or another duration is set.
     */
    void stopOffTimer();

signals:

    /**
     * @brief Signal emitted when the LED is removed.
     * @details This signal is emitted when an LED is removed from a collection or interface, allowing for cleanup or updates to related components.
     * @param id The ID of the removed LED.
     */
    void removed(int id); 

    /**
     * @brief Signal emitted when the color of the LED changes.
     * @details Notifies observers that the color of an LED has changed, which may be used to trigger UI updates or logging.
     * @param id The ID of the LED whose color changed.
     * @param color The new color of the LED.
     */
    void colorChanged(int id, const QColor &color); 

protected:

    /**
     * @brief Handles mouse press events.
     * @details Overrides the default QWidget handling to provide custom interactions with the VirtualLED, such as turning it on or off with mouse clicks.
     * @param event The mouse event.
     */
    void mousePressEvent(QMouseEvent *event) override; 

    /**
     * @brief Paints the LED.
     * @details Draws the LED on the screen, using its current color and state to determine its appearance.
     * @param event The paint event.
     */
    void paintEvent(QPaintEvent *event) override; 

    /**
     * @brief Creates a context menu.
     * @details Opens a context menu when the user right-clicks on the LED, offering additional options like changing color, turning on/off, or configuring blink settings.
     * @param event The context menu event.
     */
    void contextMenuEvent(QContextMenuEvent *event) override; 

private:

    QColor currentColor; // Current color of the LED.
    int ledId; // ID of the LED.
    bool state; // State of the LED (on/off).
    bool blinkOn; // Whether blinking is enabled.
    QTimer* blinkTimer; // Timer for blinking effect.
    int blinkSpeed; // Speed of blinking in milliseconds.
    QTimer* offTimer; // Timer to turn off the LED after a duration in seconds.

};

#endif // VIRTUALLED_H
