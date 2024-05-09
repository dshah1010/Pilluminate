/**
 * @file UserInterface.h
 * @brief Defines the UserInterface class for managing VirtualLED objects through a graphical interface.
 * @details This header file contains the declaration of the UserInterface class, which facilitates interaction with VirtualLED objects. It supports adding, removing, and manipulating VirtualLEDs, including changing their color, turning them on or off, setting their blink speed, and more. The UserInterface class is built on the QWidget base class and utilizes Qt's layout managers for organizing its components.
 * @author Group 3
 */

#ifndef USERINTERFACE_H
#define USERINTERFACE_H

#include "include/models/VirtualLED.h"

// Including necessary modules.
#include <QGridLayout>
#include <QHBoxLayout>
#include <QList>
#include <QPushButton>
#include <QScrollArea>
#include <QVBoxLayout>
#include <QWidget>

/**
 * @class UserInterface
 * @brief Represents the main user interface for managing VirtualLED objects. Provides functionality for adding, removing, and manipulating the state and appearance of VirtualLEDs through a graphical interface.
 * @details This class inherits from QWidget and makes use of Qt's layout management to organize UI components. It allows the user to interact with VirtualLED objects in a visual and intuitive manner.
 * @author Group 3
 */
class UserInterface : public QWidget { 

    Q_OBJECT

public:

    /**
     * @brief Constructor for UserInterface.
     * @details Initializes the user interface, setting up the initial layout and connecting signals and slots for UI interaction. Prepares the UI for LED management.
     * @param parent The parent widget.
     */
    explicit UserInterface(QWidget *parent = nullptr); 

    /**
     * @brief Destructor for UserInterface.
     * @details Cleans up allocated resources, ensuring no memory leaks. This includes deleting UI components that were dynamically allocated.
     */
    virtual ~UserInterface() override; 

private slots:

    /**
     * @brief Adds a new LED to the interface.
     * @details Dynamically creates a new VirtualLED object and adds it to the interface for display and interaction.
     */
    void addNewLED(); 

    /**
     * @brief Turns all LEDs on.
     * @details Iterates through all VirtualLED objects managed by the interface and sets their state to 'on'.
     */
    void turnAllLEDsOn(); 

    /**
     * @brief Turns all LEDs off.
     * @details Iterates through all VirtualLED objects managed by the interface and sets their state to 'off'.
     */
    void turnAllLEDsOff(); 

    /**
     * @brief Removes all LEDs from the interface.
     * @details Clears the list of VirtualLED objects and updates the display to reflect the removal of all LEDs.
     */
    void removeAllLEDs(); 

    /**
     * @brief Removes a specific LED identified by its ID.
     * @details Finds and removes the VirtualLED object with the specified ID from the interface.
     * @param id The ID of the LED to remove.
     */
    void removeLED(int id); 

    /**
     * @brief Reassigns IDs to all LEDs following a removal to ensure continuity.
     * @details Ensures that after any LED removal, the remaining LEDs have continuous IDs, with no gaps in the sequence.
     */
    void reassignLEDIds(); 

    /**
     * @brief Changes the color of a specific LED.
     * @details Sets the color of the specified VirtualLED object to the new color provided.
     * @param id The ID of the LED.
     * @param color The new color for the LED.
     */
    void changeLEDColor(int id, const QColor &color); 

    /**
     * @brief Changes the color of all LEDs.
     * @details Opens a color picker dialog allowing the user to select a color, which is then applied to all VirtualLED objects.
     */
    void changeAllLEDsColor(); 

    /**
     * @brief Sets the blink speed for all LEDs.
     * @details Opens a dialog for the user to select a blink speed, which is then applied to all VirtualLED objects to control their blinking pattern.
     */
    void setAllLEDsBlinkSpeed(); 

    /**
     * @brief Sets a duration for which the LEDs will remain on.
     * @details Allows the user to set a specific duration for the LEDs to remain in the 'on' state before automatically turning off.
     */
    void setDurationForOnLEDs();

    /**
     * @brief Displays a help dialog explaining the interface.
     * @details Shows a dialog box containing instructions and information about how to use the user interface effectively.
     */
    void showHelpDialog(); 

private:

    QVBoxLayout *mainLayout; // Main layout of the user interface.
    QHBoxLayout *controlLayout; // Layout for control buttons.
    QGridLayout *ledsGrid; // Grid layout for displaying LEDs.
    QScrollArea *ledsContainer; // Scroll area containing the grid of LEDs.
    QPushButton *addButton, *allOnButton, *allOffButton, *removeAllButton, *changeAllColorButton, * setAllBlinkSpeedButton, *setDurationButton, *helpButton; ///< Control buttons. 
    QList<VirtualLED*> leds; // List of current VirtualLED objects.
    int nextLedId = 1; // ID to be assigned to the next added LED.

    /**
     * @brief Initializes and sets up the control panel.
     * @details Creates and arranges the control buttons within the interface, connecting each button to its corresponding slot for handling user interactions. This method establishes the foundation for the user's ability to manage LEDs.
     */
    void createControlPanel();

    /**
     * @brief Finds an LED by its ID.
     * @details Searches the list of VirtualLED objects for one with the specified ID and returns a pointer to it. If no such LED exists, nullptr is returned.
     * @param id ID of the LED to find.
     * @return Pointer to the VirtualLED object, or nullptr if not found.
     */
    VirtualLED* findLEDById(int id);

    /**
     * @brief Updates the layout to reflect the current state of the LEDs list.
     * @details Adjusts the grid layout to accurately display all VirtualLED objects according to their current states and positions. This method is called after modifications to the LED list to ensure the UI remains up to date.
     */
    void updateGridLayout(); 

};

#endif // USERINTERFACE_H
