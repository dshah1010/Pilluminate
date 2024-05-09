/**
 * @file main.cpp
 * @brief Entry point for the Qt application that opens a user interface window.
 * @details This file contains the main function that initializes a QApplication, creates a UserInterface instance, and controls the application's execution flow. The application initializes with the QApplication object, sets up the UserInterface, and enters the event loop until exit.
 * @author Group 3
 */

// Including necessary modules.
#include <QApplication>
#include <QDebug>
#include "include/interfaces/UserInterface.h"

/**
 * @brief Main function of the application.
 * @details This function serves as the starting point of the application. It sets up the necessary QApplication environment needed for any Qt GUI application. The UserInterface class is instantiated to create the main window of the application. The main event loop is started by calling exec() on the QApplication object, which waits for events such as user input, and processes them until the application is closed. Debug messages are used to indicate the application's state at key points.
 * @param argc Number of command-line arguments.
 * @param argv Array of command-line argument strings.
 * @return Integer exit code of the application. Returns 0 upon successful completion.
 * @author Group 3
 */
int main(int argc, char *argv[]) {
    QApplication app(argc, argv); // Initializes the application with command-line arguments.
    UserInterface ui; // Creates the user interface.
    ui.showMaximized(); // Displays the user interface window maximized.
    qDebug() << "Application window opened."; // Debug message indicating window is open.
    int result = app.exec(); // Enters the main event loop and waits until exit.
    qDebug() << "Application window closed."; // Debug message indicating window has been closed.
    return result; // Returns the result of the event loop execution.
}
