/**
 * @file UserInterface.cpp
 * @brief Implementation of the UserInterface class for the LED controller application.
 * @details This file contains the implementation of the UserInterface class, which includes the setup and management of the GUI elements for controlling and displaying the status of LEDs. It involves creating and managing buttons, LED displays, and other interface components. It is part of a larger project aimed at providing a user interface for an LED control system, showcasing object-oriented programming and Qt framework usage.
 * @see UserInterface.h for the declaration of the UserInterface class.
 * @author Group 3
 */

#include "include/interfaces/UserInterface.h"

// Including necessary modules.
#include <QDebug>
#include <QColorDialog>
#include <QMessageBox>
#include <QLabel>
#include <QFont>
#include <QStyle>
#include <QInputDialog>
#include <algorithm>

/**
 * @class UserInterface
 * @brief The UserInterface class manages the main window and interactions in the LED control application.
 * @details This class encapsulates all functionalities related to the graphical user interface of the LED control application. It includes methods for initializing the UI, handling user actions, and updating the display based on the state of the LED system. The class makes extensive use of the Qt framework for creating and managing UI components.
 * @author Group 3
 */

/**
 * @brief Constructs a UserInterface object.
 * @details Initializes the user interface, setting up the main window, configuring the layout, and preparing all interactive elements like buttons and displays for the LEDs. It also initializes the LED ID counter used for assigning unique IDs to LEDs.
 * @param parent Pointer to the parent widget, which defaults to nullptr.
 */
UserInterface::UserInterface(QWidget *parent) : QWidget(parent), nextLedId(1) {

    setWindowTitle("Pilluminate (Group 3)"); // Setting the window title.

    // Main layout setup.
    mainLayout = new QVBoxLayout(this);
    mainLayout->setAlignment(Qt::AlignTop);

    // Title label creation and configuration.
    QLabel* titleLabel = new QLabel("LED Controller", this);
    QFont titleFont = titleLabel->font();
    titleFont.setPointSize(24);
    titleFont.setBold(true);
    titleLabel->setFont(titleFont);
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setStyleSheet("color: #2E8B57;");  
    mainLayout->addWidget(titleLabel);

    createControlPanel(); // Control panel setup.

    // LEDs container setup.
    QWidget *container = new QWidget();
    container->setStyleSheet("background-color: gray;");  
    ledsGrid = new QGridLayout(container);
    container->setLayout(ledsGrid);
    ledsContainer = new QScrollArea(this);
    ledsContainer->setWidget(container);
    ledsContainer->setWidgetResizable(true);
    ledsContainer->setFrameShape(QFrame::NoFrame);
    mainLayout->addWidget(ledsContainer);
    setLayout(mainLayout);

    // Style setup for the application.
    this->setStyleSheet("QPushButton { background-color: #2E8B57; color: white; border-radius: 5px; padding: 6px; margin: 6px; }"
                        "QPushButton:hover { background-color: #3CB371; }"
                        "QScrollArea { border: none; }"
                        "QWidget { font-family: 'Arial'; font-size: 14px; }");

}

/**
 * @brief Destructor of UserInterface.
 * @details Cleans up the resources used by the UserInterface object, ensuring proper memory management. This includes deleting dynamically allocated widgets and clearing up any other resources that may have been used.
 */
UserInterface::~UserInterface() {}

/**
 * @brief Adds a new LED to the interface.
 * @details Creates a new VirtualLED instance, assigns it a unique ID, and adds it to the UI. It also sets up necessary signal-slot connections for the LED to interact with the rest of the interface.
 */
void UserInterface::addNewLED() {

    VirtualLED *newLed = new VirtualLED(nextLedId, this); // Creating a new LED with the next available ID.

    // Setting up signal connections for the new LED.
    connect(newLed, &VirtualLED::removed, this, &UserInterface::removeLED);
    connect(newLed, &VirtualLED::colorChanged, this, &UserInterface::changeLEDColor);

    // Adding the new LED to the list and updating the grid layout.
    leds.append(newLed); 
    updateGridLayout(); 
    qDebug() << "LED #" << nextLedId << "added."; 
    nextLedId++; 

}

/**
 * @brief Turns all LEDs on.
 * @details Iterates over all available LEDs, turning them on by changing their color to white. If no LEDs are available or all are already on, it displays a warning message.
 */
void UserInterface::turnAllLEDsOn() {

    // Check if there are no LEDs to turn on.
    if (leds.isEmpty()) {
        QMessageBox::warning(this, "Operation Failed", "<b>No LEDs available to turn on.</b>"); 
        qDebug() << "No LEDs available to turn on."; 
        return; 
    }

    bool allLedsAlreadyOn = std::all_of(leds.begin(), leds.end(), [](VirtualLED* led){ return led->isOn(); }); // Determine if all LEDs are already on.

    // Handling case where all LEDs are already on.
    if (allLedsAlreadyOn) {
        QMessageBox::warning(this, "Operation Failed", "<b>All LEDs are already turned on.</b>"); 
        qDebug() << "All LEDs were already on."; 
    } else {
        // Turning all LEDs on.
        for (VirtualLED* led : leds) {
            if (!led->isOn()) {led->setColor(Qt::white);}
            led->stopOffTimer(); // Explicitly stop the offTimer to prevent it from turning off the LED.
        } 
        qDebug() << "All LEDs turned on."; 
    }

}

/**
 * @brief Turns all LEDs off.
 * @details Checks each LED in the interface and turns it off if it is currently on. Displays a warning if no LEDs are available to turn off.
 */
void UserInterface::turnAllLEDsOff() {

    // Check if there are no LEDs to turn off.
    if (leds.isEmpty()) {
        QMessageBox::warning(this, "Operation Failed", "<b>No LEDs available to turn off.</b>");
        qDebug() << "No LEDs available to turn off.";
        return;
    }

    bool allAlreadyOff = true; // Flag to check if all LEDs were already off.

    // Turning off each LED if it's on.
    for (VirtualLED *led : leds) {
        if (led->isOn()) {
            led->turnOff(); 
            allAlreadyOff = false;
        }
    }

    // Handling case where all LEDs were already off.
    if (allAlreadyOff) {
        QMessageBox::warning(this, "Operation Ineffective", "<b>All LEDs are already off.</b>");
        qDebug() << "All LEDs were already off.";
    } else {qDebug() << "All LEDs turned off.";}

}

/**
 * @brief Removes all LEDs from the interface.
 * @details Deletes all VirtualLED instances from the interface, clears the internal list maintaining the LEDs, and resets the ID counter.
 */
void UserInterface::removeAllLEDs() {

    // Check if there are LEDs to remove.
    if (leds.isEmpty()) {
        QMessageBox::warning(this, "Operation Failed", "<b>No LEDs available to remove.</b>"); 
        qDebug() << "No LEDs available to remove.";
        return;
    }

    // Deleting all LED objects and clearing the list.
    qDeleteAll(leds); 
    leds.clear(); 
    nextLedId = 1; // Resetting the ID counter.
    updateGridLayout(); // Updating the grid layout.
    qDebug() << "All LEDs have been removed.";

}

/**
 * @brief Removes a specific LED by its ID.
 * @details Locates an LED by its unique ID and removes it from the interface. This involves deleting the VirtualLED instance and updating the layout accordingly.
 * @param id The ID of the LED to remove.
 */
void UserInterface::removeLED(int id) {

    VirtualLED* ledToRemove = findLEDById(id); // Finding the LED by ID.

    if (ledToRemove) {
        leds.removeOne(ledToRemove); // Removing the LED from the list.
        ledsGrid->removeWidget(ledToRemove); // Removing the LED widget from the grid layout.
        ledToRemove->deleteLater(); // Deleting the LED object.
        reassignLEDIds(); // Reassigning IDs to the remaining LEDs.
        updateGridLayout(); // Updating the grid layout.
        qDebug() << "LED #" << id << "removed."; 
    }

}

/**
 * @brief Reassigns IDs to all LEDs, starting from 1.
 * @details Iterates over all LEDs, assigning them new consecutive IDs starting from 1. This is typically called after an LED has been removed to ensure continuous ID sequencing.
 */
void UserInterface::reassignLEDIds() {
    int id = 1; // Starting ID.
    for (VirtualLED* led : leds) {led->setId(id++);} // Assigning new ID to each LED.
    nextLedId = id; // Setting the next LED ID to the next available number.
}

/**
 * @brief Changes the color of a specific LED.
 * @details Finds an LED by its ID and sets its color to a specified value. This allows individual control over each LED's appearance.
 * @param id The ID of the LED to change color.
 * @param color The new color for the LED.
 */
void UserInterface::changeLEDColor(int id, const QColor &color) {
    
    VirtualLED *led = findLEDById(id); // Finding the LED by ID.

    if (led) {
        led->setColor(color); // Setting the new color for the identified LED.
        qDebug() << "LED #" << id << "color changed to" << color.name() << "."; 
    }

}

/**
 * @brief Changes the color of all LEDs that are currently on.
 * @details Opens a color dialog for the user to select a color, which is then applied to all currently on LEDs. If no LEDs are on, it shows a warning message.
 */
void UserInterface::changeAllLEDsColor() {

    // Check if there are any LEDs to change color.
    if (leds.isEmpty()) {
        QMessageBox::warning(this, "Operation Failed", "<b>No LEDs available to change color.</b>");
        qDebug() << "No LEDs available to change color.";
        return;
    }

    bool isAnyLedOn = std::any_of(leds.begin(), leds.end(), [](VirtualLED* led){ return led->isOn(); }); // Ensure at least one LED is on before proceeding.

    if (!isAnyLedOn) {
        QMessageBox::warning(this, "Operation Failed", "<b>At least one LED must be on to change colors.</b>");
        qDebug() << "No LEDs are on, can't change colors.";
        return;
    }

    QColor color = QColorDialog::getColor(Qt::white, this, "Select Color For All LEDs"); // Open color selection dialog.

    if (color.isValid()) {
        for (VirtualLED* led : leds) {if (led->isOn()) {led->setColor(color);}} // Apply the selected color to all LEDs that are on.
        qDebug() << "Changed color of all on LEDs to" << color.name() << ".";
    }
    
}

/**
 * @brief Sets the blinking speed for all LEDs that are currently on.
 * @details Presents a dialog for the user to select a blinking speed, which is then applied to all LEDs that are currently on. This allows for a uniform blinking pattern across all active LEDs. If no LEDs are on or blinking, a warning message is shown to the user.
 */
void UserInterface::setAllLEDsBlinkSpeed() {

    // Check if there are any LEDs to set the blinking speed.
    if (leds.isEmpty()) {
        QMessageBox::warning(this, "Operation Failed", "<b>No LEDs available to set blinking speed.</b>");
        qDebug() << "No LEDs available to set blinking speed.";
        return;
    }

    bool isAnyLEDBlinkingOrOn = std::any_of(leds.begin(), leds.end(), [](VirtualLED* led){ return led->isOn() || led->getBlinkSpeed() > 0; }); // Ensure at least one LED is on or blinking before proceeding.

    if (!isAnyLEDBlinkingOrOn) {
        QMessageBox::warning(this, "Operation Failed", "<b>At least one LED must be on to set blinking speed.</b>");
        qDebug() << "No LEDs are on, can't set blinking speed.";
        return;
    }

    bool ok;
    int speed = QInputDialog::getInt(this, "Set All Blinking Speed", "Speed (ms):", 0, 0, 10000, 1, &ok); // Open dialog to select blinking speed.

    if (ok) {
        for (VirtualLED* led : leds) {if (led->isOn()) {led->setBlinkSpeed(speed);}} // Apply the selected speed to all LEDs that are on.
        qDebug() << "Blinking speed set for all on LEDs to" << speed << "ms.";
    }

}

/**
 * @brief Sets a duration for all LEDs that are currently on.
 * @details Opens a dialog for the user to input a duration in seconds. This duration is then applied to all LEDs that are currently on, allowing them to turn off automatically after the specified time. If no LEDs are on, a warning message is shown.
 */
void UserInterface::setDurationForOnLEDs() {

    // Check if there are any LEDs to set the duration.
    if(leds.isEmpty()) {
        QMessageBox::warning(this, "Operation Failed", "<b>No LEDs available to set duration.</b>");
        qDebug() << "No LEDs available to set duration.";
        return;
    }

    // Ensure at least one LED is on before proceeding.
    if (!std::any_of(leds.begin(), leds.end(), [](VirtualLED* led){ return led->isOn(); })) {
        QMessageBox::warning(this, "Operation Failed", "<b>At least one LED must be on to set duration.</b>");
        qDebug() << "No LEDs are on, can't set duration.";
        return;
    }

    bool ok;
    int duration = QInputDialog::getInt(this, "Set LEDs Duration", "Duration (seconds):", 0, 0, 3600, 1, &ok); // Open dialog to select duration.

    if(ok) {
        for(VirtualLED* led : leds) {if(led->isOn()) {led->setDuration(duration);}} // Apply the selected duration to all LEDs that are on.
        qDebug() << "Duration set for all on LEDs to" << duration << "seconds.";
    }

}

/**
 * @brief Displays a help dialog with information about the application.
 * @details Generates and shows a dialog box containing helpful information about how to use the LED controller application, including descriptions of all available actions and credits to the team members who developed it.
 */
void UserInterface::showHelpDialog() {

    QMessageBox helpBox;
    helpBox.setWindowTitle("Help");

    // Constructing the help dialog content.
    QString helpText = "<h2>Pilluminate (Group 3) - LED Controller</h2>"
                       "<p><b>Add LED:</b> Adds a new LED to the display<br>"
                       "<b>Turn All LEDs On:</b> Turns all the LEDs on<br>"
                       "<b>Turn All LEDs Off:</b> Turns all the LEDs off<br>"
                       "<b>Remove All LEDs:</b> Removes all the LEDs from the display<br>"
                       "<b>Change All Colors:</b> Changes the color of all on LEDs present on the display<br>"
                       "<b>Set All Blink Speed:</b> Changes the blinking speed of all on LEDs present on the display<br>"
                       "<b>Set All Duration:</b> Changes the duration of all on LEDs present on the display<br><br>"
                       "To remove (can be on/off), change color (must be on), set blinking speed (must be on), or set duration (must be on) for an LED individually, right-click on it</p>"
                       "<h3>Team Members:</h3>"
                       "<ul>"
                       "<li>Andy Duly</li>"
                       "<li>Derek Liu</li>"
                       "<li>Jensen Aimery Medeiros</li>"
                       "<li>Deep Ashishkumar Shah</li>"
                       "<li>Alyssa Taylor Tran</li>"
                       "</ul>"; 

    QMessageBox::information(this, "Help", helpText); // Displaying the help dialog.
    qDebug() << "Help button clicked."; 

}

/**
 * @brief Creates the control panel with action buttons for the user interface.
 * @details Sets up a horizontal layout filled with buttons that provide user actions such as adding new LEDs, turning all LEDs on or off, changing colors, and more. This method encapsulates the initialization and configuration of the control panel's buttons and their signal-slot connections.
 */
void UserInterface::createControlPanel() {

    controlLayout = new QHBoxLayout(); // Layout for control buttons.

    // Initializing control buttons.
    addButton = new QPushButton("Add LED", this);
    allOnButton = new QPushButton("Turn All LEDs On", this);
    allOffButton = new QPushButton("Turn All LEDs Off", this);
    removeAllButton = new QPushButton("Remove All LEDs", this);
    changeAllColorButton = new QPushButton("Change All Colors", this);
    setAllBlinkSpeedButton = new QPushButton("Set All Blink Speed", this);
    setDurationButton = new QPushButton("Set All Duration", this);
    helpButton = new QPushButton("Help", this);

    // Adding buttons to the layout.
    controlLayout->addWidget(addButton);
    controlLayout->addWidget(allOnButton);
    controlLayout->addWidget(allOffButton);
    controlLayout->addWidget(removeAllButton);
    controlLayout->addWidget(changeAllColorButton);
    controlLayout->addWidget(setAllBlinkSpeedButton); 
    controlLayout->addWidget(setDurationButton);
    controlLayout->addWidget(helpButton);
    mainLayout->addLayout(controlLayout); 

    // Connecting buttons to their respective slots.
    connect(addButton, &QPushButton::clicked, this, &UserInterface::addNewLED);
    connect(allOnButton, &QPushButton::clicked, this, &UserInterface::turnAllLEDsOn);
    connect(allOffButton, &QPushButton::clicked, this, &UserInterface::turnAllLEDsOff);
    connect(removeAllButton, &QPushButton::clicked, this, &UserInterface::removeAllLEDs);
    connect(changeAllColorButton, &QPushButton::clicked, this, &UserInterface::changeAllLEDsColor);
    connect(setAllBlinkSpeedButton, &QPushButton::clicked, this, &UserInterface::setAllLEDsBlinkSpeed);
    connect(setDurationButton, &QPushButton::clicked, this, &UserInterface::setDurationForOnLEDs);
    connect(helpButton, &QPushButton::clicked, this, &UserInterface::showHelpDialog);

}

/**
 * @brief Finds an LED by its ID.
 * @details Searches the list of LEDs for one with a matching ID. If found, it returns a pointer to the LED. Otherwise, it returns nullptr. This function is essential for operations that target a specific LED, such as changing its color or removing it.
 * @param id The ID of the LED to find.
 * @return A pointer to the found VirtualLED, or nullptr if no LED with the given ID is found.
 */
VirtualLED* UserInterface::findLEDById(int id) {
    for (VirtualLED *led : leds) {if (led->getId() == id) {return led;}} // Returning the found LED.
    return nullptr; // No LED found with the specified ID.
}

/**
 * @brief Updates the layout of LEDs in the grid.
 * @details Clears the current grid layout and re-adds each LED in its appropriate position based on a pre-defined number of columns. This method is called after adding or removing LEDs to ensure the display is updated correctly.
 */
void UserInterface::updateGridLayout() {

    // Clearing existing items in the grid layout.
    while (QLayoutItem* item = ledsGrid->takeAt(0)) { 
        if (QWidget* widget = item->widget()) {widget->hide();} // Hiding the widget before deletion to improve UI performance.
        delete item; // Deleting the layout item, which removes it from the layout.
    }

    int row, col;
    const int cols = 5; // Number of columns in the grid.

    // Adding LEDs to the grid, calculating their position based on the index.
    for (int i = 0; i < leds.size(); ++i) {
        row = i / cols; // Calculating the row index.
        col = i % cols; // Calculating the column index.
        ledsGrid->addWidget(leds.at(i), row, col); // Adding the LED widget to the grid at the calculated position.
        leds.at(i)->show(); // Ensuring the LED widget is visible.
    }

}
