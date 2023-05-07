#include <iostream>
#include <windows.h>
using namespace std;

class FileManagementSystem {
protected:
    string folderPath;
public:
    FileManagementSystem(string path) {
        folderPath = path;
    }

    virtual void createFile() = 0;
    virtual void readFile(int& fileSize) = 0;
    virtual void editFile() = 0;
    virtual void readFileAgain() = 0;
    virtual void checkFilePath() = 0;
    virtual void renameFile(string newFilename) = 0;
    virtual void deleteFile() = 0;
};

class MyFileManagementSystem : public FileManagementSystem {
private:
    HANDLE hFile;
    char buffer[150];
    DWORD bytesToWrite;
    DWORD bytesWritten;
    DWORD bytesRead;
    char readBuffer[150];
    char newBuffer[150];
public:
    MyFileManagementSystem(string path) : FileManagementSystem(path) {}

    void createFile() override {
        // Create a new file in the folder
        hFile = CreateFile((folderPath + "osproject.txt").c_str(),
                            GENERIC_WRITE,
                            0,
                            NULL,
                            CREATE_ALWAYS,
                            FILE_ATTRIBUTE_NORMAL,
                            NULL);

        bool fileCreatedSuccessfully = (hFile != INVALID_HANDLE_VALUE);

        if (!fileCreatedSuccessfully) {
            cout << "Failed to create file.\n";
            return;
        }

        // Write to the file
        strcpy(buffer, "File Management system os project\n");
        bytesToWrite = sizeof(buffer);
        if (!WriteFile(hFile, newBuffer, strlen(newBuffer), &bytesWritten, NULL)) {
            cout << "Failed to write to file.\n";
            CloseHandle(hFile);
            return;
        }

        cout << "File created and written to successfully. " << bytesWritten << " bytes written.\n";

        // Close the file handle
        CloseHandle(hFile);
    }

    void readFile(int& fileSize) override {
        // Read from the file and get file size
        hFile = CreateFile((folderPath + "osproject.txt").c_str(),
                            GENERIC_READ,
                            0,
                            NULL,
                            OPEN_EXISTING,
                            FILE_ATTRIBUTE_NORMAL,
                            NULL);

        if (hFile == INVALID_HANDLE_VALUE) {
            cout << "Failed to open file.\n";
            return;
        }

        // Get the file size
        LARGE_INTEGER size;
        if (!GetFileSizeEx(hFile, &size)) {
            cout << "Failed to get file size.\n";
            CloseHandle(hFile);
            return;
        }
        fileSize = size.QuadPart;

        if (!ReadFile(hFile, readBuffer, sizeof(readBuffer), &bytesRead, NULL)) {
            cout << "Failed to read from file.\n";
            CloseHandle(hFile);
            return;
        }

        cout << "File contents: " << readBuffer << "\n";
        cout << "File size: " << fileSize << " bytes\n";

        // Close the file handle
        CloseHandle(hFile);
    }

    void editFile() override{
        // Edit the file
        hFile = CreateFile((folderPath + "osproject.txt").c_str(),
                            GENERIC_WRITE,
                            0,
                            NULL,
                            OPEN_EXISTING,
                            FILE_ATTRIBUTE_NORMAL,
                            NULL);

        if (hFile == INVALID_HANDLE_VALUE) {
            cout << "Failed to open file.\n";
            return;
        }

        strcpy(newBuffer, "Aditya Pratap Singh Chauhan");
        SetFilePointer(hFile, 0, NULL, FILE_BEGIN);


        if (!WriteFile(hFile, newBuffer, sizeof(newBuffer), &bytesWritten, NULL)) {
            cout << "Failed to write to file.\n";
            CloseHandle(hFile);
            return;
        }

        cout << "Number of bytes written: " << bytesWritten << "\n";
        cout << "File edited successfully.\n";

        // Close the file handle
        CloseHandle(hFile);
    }

    void readFileAgain() override {
    // Read from the file again
    hFile = CreateFile((folderPath + "osproject.txt").c_str(),
                        GENERIC_READ,
                        0,
                        NULL,
                        OPEN_EXISTING,
                        FILE_ATTRIBUTE_NORMAL,
                        NULL);

    if (hFile == INVALID_HANDLE_VALUE) {
        cout << "Failed to open file.\n";
        return;
    }

    if (!ReadFile(hFile, readBuffer, sizeof(readBuffer), &bytesRead, NULL)) {
        cout << "Failed to read from file.\n";
        CloseHandle(hFile);
        return;
    }

    cout << "File contents: " << readBuffer << "\n";

    // Close the file handle
    CloseHandle(hFile);
}

void checkFilePath() override {
    // Check if the file exists
    hFile = CreateFile((folderPath + "osproject.txt").c_str(),
                        GENERIC_READ,
                        0,
                        NULL,
                        OPEN_EXISTING,
                        FILE_ATTRIBUTE_NORMAL,
                        NULL);

    if (hFile == INVALID_HANDLE_VALUE) {
        cout << "The file does not exist.\n";
        return;
    }

    cout << "The file exists.\n";

    // Close the file handle
    CloseHandle(hFile);
}

void renameFile(string newFilename) override {
    // Rename the file
    if (!MoveFile((folderPath + "osproject.txt").c_str(), (folderPath + newFilename).c_str())) {
        cout << "Failed to rename file.\n";
        return;
    }

    cout << "File renamed successfully.\n";
}

void deleteFile() override {
    // Delete the file
    if (!DeleteFile((folderPath + "osproject.txt").c_str())) {
        cout << "Failed to delete file.\n";
        return;
    }

    cout << "File deleted successfully.\n";
}
};
int main(){
    // Specify the folder path
    string folderPath = "C:\\Users\\DELL GUNA\\";

    FileManagementSystem *fms = new MyFileManagementSystem(folderPath);
     while (true) {
    int choice ;
    cout<<"Press (1) To create file , (2) Check file path ,(3) Read File , (4) Edit file , (5)Read Again after edit,  (6)Rename file , (7) Delete file: ";
    cin>>choice;
    switch(choice)
    {
    case 1:
    fms->createFile();
    break;
    case 2:
    fms->checkFilePath();
    break;
    case 3:
    int fileSize;
    fms->readFile(fileSize);
    break;
    case 4:
    fms->editFile();
    break;
    case 5:
    fms->readFileAgain();
    fms->readFile(fileSize);
    break;
    case 6:
    cout << "Space taken by file: " << fileSize << " bytes\n";
    fms->renameFile("new_filename.txt");
    break;
    case 7:
    fms->deleteFile();
    break;
    default:
                cout << "Invalid input, please try again.\n";
            }
     }
    
   

    delete fms;

    return 0;
}