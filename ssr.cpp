#include <iostream>
#include <fstream>
#include <sstream>
#include <filesystem>
#include <thread>
#include <chrono>

using namespace std;

const int MAX_FILENAME_LEN = 256;
const int MAX_FILESIZE = 1024 * 1024; // 1 MB
const int MAX_RECEIVER_LEN = 256;

void displayWelcomeMessage() {
    cout << "========================================================" << endl;
    cout << "               FILE SHARING APPLICATION" << endl;
    cout << "========================================================" << endl;
    cout << endl;
}

void displayMainMenu() {
    cout << "MAIN MENU" << endl;
    cout << "1. Share a file" << endl;
    cout << "2. Exit" << endl;
    cout << endl;
}

bool validateFilename(const string& filename) {
    // Check if the filename is too long
    if (filename.length() > MAX_FILENAME_LEN) {
        cerr << "Error: Filename is too long." << endl;
        return false;
    }

    // Check if the file exists
    if (!filesystem::exists(filename)) {
        cerr << "Error: File does not exist." << endl;
        return false;
    }

    // Check if the file is a directory
    if (filesystem::is_directory(filename)) {
        cerr << "Error: File is a directory." << endl;
        return false;
    }

    // Check if the file is too large
    ifstream file(filename, ios::ate | ios::binary);
    if (file.tellg() > MAX_FILESIZE) {
        cerr << "Error: File is too large." << endl;
        return false;
    }
    return true;
}

bool validateReceiver(const string& receiver) {
    // Check if the receiver name is too long
    if (receiver.length() > MAX_RECEIVER_LEN) {
        cerr << "Error: Receiver name is too long." << endl;
        return false;
    }

    // Check if the receiver is valid
    // In this example, we assume that the receiver is valid if it is not empty
    if (receiver.empty()) {
        cerr << "Error: Invalid receiver name." << endl;
        return false;
    }

    return true;
}

void shareFile(const string& filename, const string& receiver) {
    // Validate the filename and receiver
    if (!validateFilename(filename) || !validateReceiver(receiver)) {
        return;
    }

    // Open the file for reading
    ifstream file(filename, ios::binary);
    if (!file) {
        cerr << "Error: Unable to open file for sharing." << endl;
        return;
    }

    // Read the contents of the file into a string
    stringstream buffer;
    buffer << file.rdbuf();
    string file_contents = buffer.str();

    // Connect to the receiver
    cout << "Connecting to " << receiver << "..." << endl;
    this_thread::sleep_for(chrono::seconds(2));

    // Send the file contents to the receiver
    cout << "Sending file to " << receiver << "..." << endl;
    // In this example, we simply print the file contents to the console
    cout << "File contents:" << endl;
    cout << file_contents << endl;

    // Close the file
    file.close();

    cout << "File sharing completed successfully!" << endl;
}

int main() {
   
    displayWelcomeMessage();

    while (true) {
        displayMainMenu();
        int choice;
        std::cout << "Enter your choice: ";
        std::cin >> choice;

        if (choice == 1) {
            std::string filename;
            std::cout << "Enter the filename: ";
            std::cin >> filename;

            std::string receiver;
            std::cout << "Enter the receiver name: ";
            std::cin >> receiver;

            shareFile(filename, receiver);
        }
        else if (choice == 2) {
            std::cout << "Exiting program..." << std::endl;
            break;
        }
        else {
            std::cerr << "Error: Invalid choice. Please enter a valid choice." << std::endl;
        }
    }

    return 0;
}