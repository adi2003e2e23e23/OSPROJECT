#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <stdexcept>

using namespace std;

class User {
private:
    string username;
    string password;
    bool isAdmin;

public:
    User(string username, string password, bool isAdmin) {
        this->username = username;
        this->password = password;
        this->isAdmin = isAdmin;
    }

    string getUsername() {
        return this->username;
    }

    string getPassword() {
        return this->password;
    }

    bool isAdminUser() {
        return this->isAdmin;
    }
};

class File {
public:
    virtual string getType() = 0;
    virtual void displayContent() = 0;
};

class TextFile : public File {
private:
    string content;

public:
    TextFile(string content) {
        this->content = content;
    }

    string getType() {
        return "Text File";
    }

    void displayContent() {
        cout << this->content << endl;
    }
};

class ImageFile : public File {
private:
    string imagePath;

public:
    ImageFile(string imagePath) {
        this->imagePath = imagePath;
    }

    string getType() {
        return "Image File";
    }

    void displayContent() {
        cout << "Displaying image from " << this->imagePath << endl;
    }
};

class FileManager {
private:
    map<string, vector<string>> fileAccess;
    vector<User> users;
    map<string, File*> files;

public:
    void addUser(User user) {
        users.push_back(user);
    }

    void addFile(string filename, string type, string content) {
        if (type == "text") {
            files[filename] = new TextFile(content);
        }
        else if (type == "image") {
            files[filename] = new ImageFile(content);
        }

        fileAccess[filename] = vector<string>();
    }

    bool hasAccess(string username, string filename) {
        for (User user : users) {
            if (user.getUsername() == username) {
                if (fileAccess.find(filename) == fileAccess.end()) {
                    return false;
                }

                for (string authorizedUser : fileAccess[filename]) {
                    if (authorizedUser == username || user.isAdminUser()) {
                        return true;
                    }
                }
            }
        }

        return false;
    }

    void grantAccess(string username, string filename) {
        if (fileAccess.find(filename) == fileAccess.end()) {
            throw invalid_argument("File does not exist.");
        }

        for (User user : users) {
            if (user.getUsername() == username) {
                fileAccess[filename].push_back(username);
                cout << "Access granted to " << username << " for file " << filename << endl;
                return;
            }
        }

        throw invalid_argument("User not found.");
    }

    void revokeAccess(string username, string filename) {
        if (fileAccess.find(filename) == fileAccess.end()) {
            throw invalid_argument("File does not exist.");
        }

        for (string& authorizedUser : fileAccess[filename]) {
            if (authorizedUser == username) {
                authorizedUser = "";
                cout << "Access revoked for " << username << " for file " << filename << endl;
                return;
            }
        }

        throw invalid_argument("User did not have access to file.");
    }

    void displayFile(string username, string filename) {
        if (!hasAccess(username, filename)) {
            throw invalid_argument("Access denied.");
        }
    File* file = files[filename];
    file->displayContent();
}
};

int main() {
FileManager fileManager;
User alice("alice", "password", false);
User bob("bob", "password", false);
User admin("admin", "password", true);

fileManager.addUser(alice);
fileManager.addUser(bob);
fileManager.addUser(admin);

fileManager.addFile("file1", "text", "This is the content of file1.");
fileManager.addFile("file2", "image", "path/to/image.jpg");

fileManager.grantAccess("bob", "file1");
fileManager.grantAccess("admin", "file2");

try {
    fileManager.displayFile("alice", "file1");
}
catch (exception& e) {
    cout << e.what() << endl;
}

try {
    fileManager.displayFile("bob", "file1");
}
catch (exception& e) {
    cout << e.what() << endl;
}

try {
    fileManager.displayFile("bob", "file2");
}
catch (exception& e) {
    cout << e.what() << endl;
}

try {
    fileManager.displayFile("admin", "file2");
}
catch (exception& e) {
    cout << e.what() << endl;
}

return 0;
}