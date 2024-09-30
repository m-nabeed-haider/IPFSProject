#pragma once

#include <iostream>
#include<fstream>
#include<string>
#include<direct.h>
using namespace std;

//-------File class--------
class File
{
public:
    string path; // path to access file
    string data; // actual contents of the file
    string key; // key generated from SHA-1 on data.
    fstream handle; // fstream object used to read and write information to files

    // Constructor
    File(string path = "") {
        this->path = path;
        data = "";
        key = "";

    }
    void readFromFile() {
        handle.open(path, ios::in);
        string str;
        if (handle.is_open()) {
            // getline(handle,str) means read one line of file and store it into str
            while (getline(handle, str)) {
                // appending the string grabbed from the file to data string
                data += str;
                data += '\n';
            }
            handle.close();
        }
    }

    void appendData() {
        handle.open(path, ios::app);
        string str = "hard-coded data";
        if (handle.is_open()) {
            handle << str;
            handle.close();
        }

    }
    void printData() {
        cout << data;
    }
    // Implement Hash function using SHA-I
    /*
    void calckey() {
        // key = SHA-1(data);
    }
    */
};