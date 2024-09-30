#include <SFML/Graphics.hpp>
#include <iostream>
#include "sha1.h"
//#include"Btree.h"
#include"Ring_DHT.h"
#include<fstream>
#include<string>
#include<direct.h>

using namespace std;
using namespace sf;



void loadingScreen(RenderWindow& window, const Sprite& background)
{
    Clock clock;
    float t = 1.5f;

    // Loading 1
    Texture l1;
    l1.loadFromFile("images/loading1.png");
    Sprite loading1(l1);
    loading1.setScale(0.025, 0.025);
    loading1.setPosition(800, 450);

    while (clock.getElapsedTime().asSeconds() < t)
    {
        window.clear();
        window.draw(background);
        window.draw(loading1);
        window.display();
    }

    // Loading 2
    Texture l2;
    l2.loadFromFile("images/loading2.png");
    Sprite loading2(l2);
    loading2.setScale(0.025, 0.025);
    loading2.setPosition(800, 450);
    t += 1.5;

    while (clock.getElapsedTime().asSeconds() < t)
    {
        window.clear();
        window.draw(background);
        window.draw(loading2);
        window.display();
    }

    // Loading 3
    Texture l3;
    l3.loadFromFile("images/loading3.png");
    Sprite loading3(l3);
    loading3.setScale(0.025, 0.025);
    loading3.setPosition(800, 450);
    t += 1.5;

    while (clock.getElapsedTime().asSeconds() < t)
    {
        window.clear();
        window.draw(background);
        window.draw(loading3);
        window.display();
    }

    // Loading 4
    Texture l4;
    l4.loadFromFile("images/loading4.png");
    Sprite loading4(l4);
    loading4.setScale(0.025, 0.025);
    loading4.setPosition(800, 450);
    t += 1.5;

    while (clock.getElapsedTime().asSeconds() < t)
    {
        window.clear();
        window.draw(background);
        window.draw(loading4);
        window.display();
    }
    clock.restart();
}

void takeInput(RenderWindow& window, const Sprite& background, int& id, long double& sp, long int& ma, int& num)
{
    RectangleShape rectangle;
    rectangle.setSize(sf::Vector2f(500, 500.f));
    rectangle.setFillColor(sf::Color(255, 0, 0, 128));

    rectangle.setPosition(700, 300);
    rectangle.setOutlineThickness(2.f);
    rectangle.setOutlineColor(Color::White);

    Font font;
    font.loadFromFile("fonts/heavitas/Heavitas.ttf");

    Text identifier, space, machines;
    identifier.setFont(font);
    identifier.setFillColor(Color::White);
    identifier.setString("Enter size of identifier space: ");
    identifier.setCharacterSize(10);
    identifier.setPosition(750, 330);

    RectangleShape inputBox(sf::Vector2f(400, 30));
    inputBox.setPosition(750, 350);
    inputBox.setFillColor(sf::Color::Transparent);
    inputBox.setOutlineColor(sf::Color::Red);
    inputBox.setOutlineThickness(2.0f);

    Text userInput;
    userInput.setFont(font);
    userInput.setCharacterSize(15);
    userInput.setFillColor(sf::Color::Black);
    userInput.setPosition(755, 355);

    space.setFont(font);
    space.setFillColor(Color::White);
    space.setString("Space range: ");
    space.setCharacterSize(10);
    space.setPosition(750, 450);

    RectangleShape inputBox2(sf::Vector2f(400, 30));
    inputBox2.setPosition(750, 470);
    inputBox2.setFillColor(sf::Color::Transparent);
    inputBox2.setOutlineColor(sf::Color::White);
    inputBox2.setOutlineThickness(2.0f);

    Text spacerange;
    spacerange.setFont(font);
    spacerange.setCharacterSize(15);
    spacerange.setFillColor(sf::Color::Black);

    spacerange.setPosition(755, 475);
    machines.setFont(font);
    machines.setFillColor(Color::White);
    machines.setString("Enter Machines: ");
    machines.setCharacterSize(10);
    machines.setPosition(750, 570);

    RectangleShape inputBox3(sf::Vector2f(400, 30));
    inputBox3.setPosition(750, 590);
    inputBox3.setFillColor(sf::Color::Transparent);
    inputBox3.setOutlineColor(sf::Color::White);
    inputBox3.setOutlineThickness(2.0f);

    Text userInput2;
    userInput2.setFont(font);
    userInput2.setCharacterSize(15);
    userInput2.setFillColor(sf::Color::Black);
    userInput2.setPosition(755, 595);

    window.draw(background);
    window.draw(rectangle);

    window.draw(identifier);
    window.draw(inputBox);
    window.draw(inputBox2);
    window.draw(inputBox3);
    window.draw(space);
    window.draw(machines);
    window.display();


    bool isIdentifierActive = true;  // Track the active input box
    bool done = false;
    string identifierInput;
    string machineInput;
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::TextEntered)
            {
                if (event.text.unicode < 128)
                {
                    if (event.text.unicode == 13) // Enter key pressed
                    {
                        // Move to the next input box
                        if (isIdentifierActive) {
                            if (identifierInput.size())
                            {
                                isIdentifierActive = false;
                                inputBox.setOutlineColor(Color::White);
                                inputBox2.setOutlineColor(sf::Color::White);
                                inputBox3.setOutlineColor(sf::Color::Red);  // Highlight the next box
                            }
                        }
                        else
                        {
                            if (machineInput.size())
                            {
                                done = true;
                            }
                        }
                    }
                    else if (event.text.unicode == 8) // Backspace key pressed
                    {
                        // Handle backspace to delete the last character
                        if (isIdentifierActive && !identifierInput.empty()) {
                            identifierInput.pop_back();
                            int val = 0;
                            if (identifierInput.size())
                            {
                                val = stoi(identifierInput);
                            }
                            long double range = pow(2, val);
                            id = val;
                            sp = range;
                            string str = to_string(range);
                            userInput.setString(identifierInput);
                            spacerange.setString(str);
                        }
                        else if (!machineInput.empty()) {
                            machineInput.pop_back();
                            userInput2.setString(machineInput);
                            if (machineInput.size())
                            {
                                ma = stold(machineInput);
                            }
                            else
                            {
                                ma = 0;
                            }
                        }
                    }
                    else if (event.text.unicode >= '0' && event.text.unicode <= '9')
                    {
                        // Append the character to the active input string
                        if (isIdentifierActive) {
                            identifierInput += static_cast<char>(event.text.unicode);
                            int val = stoi(identifierInput);
                            long double range = pow(2, val);
                            id = val;
                            sp = range;
                            string str = to_string(range);
                            userInput.setString(identifierInput);
                            spacerange.setString(str);
                        }
                        else {
                            machineInput += static_cast<char>(event.text.unicode);
                            long double val1 = stold(machineInput);
                            ma = val1;
                            string str = spacerange.getString();
                            long double val2 = stold(str);
                            if (val1 > val2)
                            {
                                machineInput = spacerange.getString();
                                ma = sp;
                            }
                            userInput2.setString(machineInput);
                        }
                    }
                }
            }
        }

        window.clear();
        window.draw(background);
        window.draw(rectangle);
        window.draw(identifier);
        window.draw(inputBox);
        window.draw(inputBox2);
        window.draw(inputBox3);
        window.draw(space);
        window.draw(machines);
        window.draw(userInput);
        window.draw(spacerange);
        window.draw(userInput2);
        window.display();

        if (userInput.getString().getSize() && userInput2.getString().getSize() && done)
        {
            break;
        }
    }

    inputBox.setPosition(770, 500);
    inputBox.setFillColor(sf::Color::Transparent);
    inputBox.setOutlineColor(sf::Color::Red);
    inputBox.setOutlineThickness(2.0f);
    inputBox.setSize(Vector2f(150, 100));

    inputBox2.setPosition(1000, 500);
    inputBox2.setFillColor(sf::Color::Transparent);
    inputBox2.setOutlineColor(sf::Color::White);
    inputBox2.setOutlineThickness(2.0f);
    inputBox2.setSize(Vector2f(150, 100));

    Text t1, t2, select;

    select.setFont(font);
    select.setFillColor(Color::White);
    select.setString("Select an option: ");
    select.setCharacterSize(30);
    select.setPosition(770, 450);

    t1.setFont(font);
    t1.setFillColor(Color::White);
    t1.setString("Manual");
    t1.setCharacterSize(20);
    t1.setPosition(795, 545);

    t2.setFont(font);
    t2.setFillColor(Color::White);
    t2.setString("Automatic");
    t2.setCharacterSize(20);
    t2.setPosition(1010, 545);


    int opt = 1;

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
            }
            if (event.type == sf::Event::KeyPressed)
            {
                if (event.key.code == sf::Keyboard::Left)
                {
                    inputBox.setOutlineColor(Color::Red);
                    inputBox2.setOutlineColor(Color::White);
                    opt = 1;
                }
                else if (event.key.code == sf::Keyboard::Right)
                {
                    inputBox.setOutlineColor(Color::White);
                    inputBox2.setOutlineColor(Color::Red);
                    opt = 2;
                }
                else if (event.key.code == sf::Keyboard::Enter)
                {
                    num = opt;
                    break;
                }
            }
            window.clear();
            window.draw(background);
            window.draw(rectangle);
            window.draw(inputBox);
            window.draw(inputBox2);
            window.draw(select);
            window.draw(t1);
            window.draw(t2);
            window.display();
        }
        if (num == 1 || num == 2)
        {
            break;
        }
    }
    return;
}
//------------------------input ids-----------------------------------------

void InputIds(RenderWindow& window, Sprite& background, long double arr[], long double machines,long double& sp)
{   

    Font font;
    font.loadFromFile("fonts/lemon_milk/LEMONMILK-Regular.otf");

    RectangleShape rectangle;
    rectangle.setSize(sf::Vector2f(500, 500.f));
    rectangle.setFillColor(sf::Color(255, 0, 0, 128));

    rectangle.setPosition(700, 300);
    rectangle.setOutlineThickness(2.f);
    rectangle.setOutlineColor(Color::White);

    Text text;
    text.setFont(font);
    text.setCharacterSize(20);
    text.setFillColor(sf::Color::White);
    text.setString("Enter Machine ID (Press Enter to Submit):");
    text.setPosition(710, 400);

    RectangleShape inputBox(sf::Vector2f(400, 30));
    inputBox.setPosition(750, 500);
    inputBox.setFillColor(Color::Transparent);
    inputBox.setOutlineColor(sf::Color::Red);
    inputBox.setOutlineThickness(2.0f);

    Text userInput;
    userInput.setFont(font);
    userInput.setCharacterSize(15);
    userInput.setFillColor(sf::Color::White);
    userInput.setPosition(755, 505);

    Text msg;
    msg.setFont(font);
    msg.setCharacterSize(20);
    msg.setFillColor(sf::Color::White);
    msg.setPosition(820,650);

    int ids = 0;
    long double enteredID;
    //msg.setString("");

    while (window.isOpen()) {
        Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
            {
                window.close();
            }

            if (event.type == Event::TextEntered) {
                if (event.text.unicode < 128) {
                    if (event.text.unicode == 13) {  // Enter key pressed
                        if(userInput.getString().getSize())
                        {

                            bool isDuplicate = false;

                            for (int i = 0; i < machines; ++i) {
                                if (arr[i] == enteredID) {
                                    isDuplicate = true;
                                    break;
                                }
                            }

                            if (!isDuplicate) {
                                if (ids < machines) {
                                    arr[ids++] = enteredID;
                                    string str = "ID added: " + std::to_string(enteredID);
                                    msg.setString(str);
                                    msg.setFillColor(Color::Green);
                                }
                                else {
                                    msg.setString("Array is full, cannot add more IDs.");
                                    msg.setFillColor(Color::Red);
                                }
                            }
                            else {
                                string str = "ID already exists: " + std::to_string(enteredID);
                                msg.setString(str);
                                msg.setFillColor(Color::Red);
                            }

                            userInput.setString("");
                        }
                    }
                    else if (event.text.unicode == 8) {  // Backspace key pressed
                        string currentInput = userInput.getString();
                        if (!currentInput.empty()) {
                            currentInput.pop_back();
                            userInput.setString(currentInput);
                        }
                    }
                    else if (event.text.unicode >= 32) {  // Printable ASCII characters
                        char inputChar = static_cast<char>(event.text.unicode);
                        string currentInput = userInput.getString();
                        currentInput += inputChar;
                        enteredID = stold(currentInput);
                        if (enteredID > sp)
                        {
                            enteredID = sp-1;
                            currentInput = to_string(enteredID);
                        }
                        userInput.setString(currentInput);
                    }
                }
            }
        }

        window.clear();

        window.draw(background);
        window.draw(rectangle);
        window.draw(text);
        window.draw(inputBox);
        window.draw(userInput);
        window.draw(msg);

        window.display();
        if (ids == machines)
        {
            break;
        }
    }
    return;
}

//=========================================================

void successful(RenderWindow& window, Sprite& background)
{
    Clock clock;
    float t = 1.0f;
    bool done = false;
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
            }

            Font font;
            font.loadFromFile("fonts/lemon_milk/LEMONMILK-Regular.otf");
            Text text;
            text.setFont(font);
            text.setCharacterSize(30);
            text.setFillColor(sf::Color::Green);
            text.setString("Data or Machine inserted successfully!");
            text.setPosition(800, 400);


             while (clock.getElapsedTime().asSeconds() < t)
             {
             window.clear();
             window.draw(background);
             window.draw(text);
             window.display();
             done = true;
             }

             if (done)
             {
                 break;
             }

        }
        if (done)
        {
            break;
        }
    }
    return;
}

void successful_delete(RenderWindow& window, Sprite& background)
{
    Clock clock;
    float t = 1.0f;
    bool done = false;
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
            }

            Font font;
            font.loadFromFile("fonts/lemon_milk/LEMONMILK-Regular.otf");
            Text text;
            text.setFont(font);
            text.setCharacterSize(30);
            text.setFillColor(sf::Color::Green);
            text.setString("Data or Machine deleted successfully!");
            text.setPosition(800, 400);


            while (clock.getElapsedTime().asSeconds() < t)
            {
                window.clear();
                window.draw(background);
                window.draw(text);
                window.display();
                done = true;
            }

            if (done)
            {
                break;
            }

        }
        if (done)
        {
            break;
        }
    }
    return;
}

//--------------------------------------------------------
void selectFile(RenderWindow& window, Sprite& background, DHT_Ring<long int>* r1,long space)
{
    Font font, font2;
    font.loadFromFile("fonts/lemon_milk/LEMONMILK-Regular.otf");
    font2.loadFromFile("fonts/oxford_street/OxfordStreet.ttf");

    RectangleShape rectangle;
    rectangle.setSize(sf::Vector2f(500, 500.f));
    rectangle.setFillColor(sf::Color(255, 0, 0, 128));

    rectangle.setPosition(700, 300);
    rectangle.setOutlineThickness(2.f);
    rectangle.setOutlineColor(Color::White);

    Text text;
    text.setFont(font);
    text.setCharacterSize(30);
    text.setFillColor(sf::Color::White);
    text.setString("Select an option:");
    text.setPosition(800, 400);

    RectangleShape box1, box2, box3;
    box1.setPosition(750, 500);
    box1.setFillColor(sf::Color::Transparent);
    box1.setOutlineColor(sf::Color::Red);
    box1.setOutlineThickness(2.0f);
    box1.setSize(Vector2f(100, 100));

    box2.setPosition(900, 500);
    box2.setFillColor(sf::Color::Transparent);
    box2.setOutlineColor(sf::Color::White);
    box2.setOutlineThickness(2.0f);
    box2.setSize(Vector2f(100, 100));

    box3.setPosition(1050, 500);
    box3.setFillColor(sf::Color::Transparent);
    box3.setOutlineColor(sf::Color::White);
    box3.setOutlineThickness(2.0f);
    box3.setSize(Vector2f(100, 100));

    Text t1;
    t1.setFont(font);
    t1.setCharacterSize(20);
    t1.setFillColor(sf::Color::White);
    t1.setPosition(770, 540);
    t1.setString("Video");

    Text t2;
    t2.setFont(font);
    t2.setCharacterSize(20);
    t2.setFillColor(sf::Color::White);
    t2.setPosition(920, 540);
    t2.setString("Text");

    Text t3;
    t3.setFont(font);
    t3.setCharacterSize(20);
    t3.setFillColor(sf::Color::White);
    t3.setPosition(1070, 540);
    t3.setString("MP3");


    int opt = 0;
    bool done = false;
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
            }
            if (event.type == sf::Event::KeyPressed)
            {
                if (event.key.code == sf::Keyboard::Left)
                {
                    if (opt != 0)
                    {
                        opt--;
                    }
                }
                else if (event.key.code == sf::Keyboard::Right)
                {
                    if (opt != 3)
                    {
                        opt++;
                    }
                }
                else if (event.key.code == sf::Keyboard::Enter)
                {
                    done = true;
                    break;
                }
            }
            if (opt == 0)
            {
                box1.setOutlineColor(Color::Red);
                box2.setOutlineColor(Color::White);
                box3.setOutlineColor(Color::White);
            }
            else if (opt == 1)
            {
                box1.setOutlineColor(Color::White);
                box2.setOutlineColor(Color::Red);
                box3.setOutlineColor(Color::White);
            }
            else if (opt == 3)
            {
                box1.setOutlineColor(Color::White);
                box2.setOutlineColor(Color::White);
                box3.setOutlineColor(Color::Red);
            }
            window.clear();
            window.draw(background);
            window.draw(rectangle);
            window.draw(text);
            window.draw(box1);
            window.draw(box2);
            window.draw(box3);
            window.draw(t1);
            window.draw(t2);
            window.draw(t3);
            window.display();
        }
        if (done)
        {
            break;
        }
    }

    Text text2;
    text2.setFont(font);
    text2.setCharacterSize(20);
    text2.setFillColor(sf::Color::White);
    text2.setString("Enter path");
    text2.setPosition(850, 400);

    RectangleShape inputBox(sf::Vector2f(400, 30));
    inputBox.setPosition(750, 500);
    inputBox.setFillColor(Color::Transparent);
    inputBox.setOutlineColor(sf::Color::Red);
    inputBox.setOutlineThickness(2.0f);

    Text userInput;
    userInput.setFont(font2);
    userInput.setCharacterSize(15);
    userInput.setFillColor(sf::Color::White);
    userInput.setPosition(755, 505);

    done = false;

    Text msg;
    msg.setFont(font);
    msg.setCharacterSize(20);
    msg.setFillColor(sf::Color::Red);
    msg.setPosition(740, 600);

    msg.setString("");

    fstream filehandle2;
    string str1;
    string data = "";
    string filePath;

    while (window.isOpen()) {
        Event event;
        while (window.pollEvent(event)) 
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
            }

            if (event.type == Event::TextEntered) {
                if (event.text.unicode < 128) {
                    if (event.text.unicode == 13 && userInput.getString().getSize()) {  // Enter key pressed
                        filePath = userInput.getString().toAnsiString();

                        ifstream filehandle(filePath);
                        if (filehandle.is_open() && filehandle.peek() != ifstream::traits_type::eof()) 
                        {
                            // File exists and is not empty
                            filehandle2.open(filePath, ios::in);

                            if (filehandle.is_open()) {
                                // getline(handle,str) means read one line of file and store it into str
                                while (getline(filehandle2, str1)) 
                                {
                                    // appending the string grabbed from the file to data string
                                    data += str1;
                                    data += '\n';
                                }
                                cout << "DATA :" << data << endl;
                                filehandle2.close();
                            }

                            done = true;
                        }
                        else {
                            msg.setString("Path does not exist or file is empty!");
                            userInput.setString("");
                        }
                    }
                    else if (event.text.unicode == 8)
                    {  // Backspace key pressed
                        string currentInput = userInput.getString();
                        if (!currentInput.empty()) {
                            currentInput.pop_back();
                            userInput.setString(currentInput);
                        }
                    }
                    else if (event.text.unicode >= 32) 
                    {  // Printable ASCII characters
                        char inputChar = static_cast<char>(event.text.unicode);
                        string currentInput = userInput.getString();
                        currentInput += inputChar;
                        userInput.setString(currentInput);
                    }
                }
            }
        }

        window.clear();

        window.draw(background);
        window.draw(rectangle);
        window.draw(text2);
        window.draw(inputBox);
        window.draw(userInput);
        window.draw(msg);

        window.display();
        if (done)
        {
            string key = "";
            cout << "sha1 :";
            key = sha1(data);
            cout << key << endl;
            long int new_key = 0;

            for (int i = 0; key[i]; i++)
            {
                new_key += key[i];
            }
            cout << "key :" << new_key << endl;
           new_key = new_key % long int(space);
           r1->insert_data(new_key, key, filePath);

           successful(window, background);


            break;
        }
    }

    return;
}
//--------------------------------------------------------
void Mac_Key(long double& key, RenderWindow& window, Sprite& background)
{
    Font font, font2;
    font.loadFromFile("fonts/lemon_milk/LEMONMILK-Regular.otf");
    font2.loadFromFile("fonts/oxford_street/OxfordStreet.ttf");
    RectangleShape rectangle;
    rectangle.setSize(sf::Vector2f(500, 500.f));
    rectangle.setFillColor(sf::Color(255, 0, 0, 128));

    rectangle.setPosition(700, 300);
    rectangle.setOutlineThickness(2.f);
    rectangle.setOutlineColor(Color::White);

    Text text2;
    text2.setFont(font);
    text2.setCharacterSize(30);
    text2.setFillColor(sf::Color::White);
    text2.setString("Enter Key");
    text2.setPosition(850, 400);

    RectangleShape inputBox(sf::Vector2f(400, 30));
    inputBox.setPosition(750, 500);
    inputBox.setFillColor(Color::Transparent);
    inputBox.setOutlineColor(sf::Color::Red);
    inputBox.setOutlineThickness(2.0f);

    Text userInput;
    userInput.setFont(font2);
    userInput.setCharacterSize(15);
    userInput.setFillColor(sf::Color::White);
    userInput.setPosition(755, 505);

    bool done = false;

    Text msg;
    msg.setFont(font);
    msg.setCharacterSize(20);
    msg.setFillColor(sf::Color::Red);
    msg.setPosition(740, 600);

    msg.setString("");

    while (window.isOpen()) {
        Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
            {
                window.close();
            }

            if (event.type == Event::TextEntered) {
                if (event.text.unicode < 128) {
                    if (event.text.unicode == 13 && userInput.getString().getSize()) {  // Enter key pressed

                        if (userInput.getString().getSize()) {
                            // File exists and is not empty
                            done = true;
                        }
                        else {
                            msg.setString("INVALID KEY!");
                            userInput.setString("");
                        }
                    }
                    else if (event.text.unicode == 8) {  // Backspace key pressed
                        string currentInput = userInput.getString();
                        if (!currentInput.empty()) {
                            currentInput.pop_back();
                            userInput.setString(currentInput);
                        }
                    }
                    else if (event.text.unicode >= 32) {  // Printable ASCII characters
                        char inputChar = static_cast<char>(event.text.unicode);
                        string currentInput = userInput.getString();
                        currentInput += inputChar;
                        key = stold(currentInput);
                        userInput.setString(currentInput);
                    }
                }
            }
        }

        window.clear();

        window.draw(background);
        window.draw(rectangle);
        window.draw(text2);
        window.draw(inputBox);
        window.draw(userInput);
        window.draw(msg);

        window.display();
        if (done)
        {
            successful(window, background);
            break;
        }
    }
}

//------------------------------------------------------
void Key(long double& key,RenderWindow& window, Sprite& background)
{
    Font font, font2;
    font.loadFromFile("fonts/lemon_milk/LEMONMILK-Regular.otf");
    font2.loadFromFile("fonts/oxford_street/OxfordStreet.ttf");
    RectangleShape rectangle;
    rectangle.setSize(sf::Vector2f(500, 500.f));
    rectangle.setFillColor(sf::Color(255, 0, 0, 128));

    rectangle.setPosition(700, 300);
    rectangle.setOutlineThickness(2.f);
    rectangle.setOutlineColor(Color::White);

    Text text2;
    text2.setFont(font);
    text2.setCharacterSize(30);
    text2.setFillColor(sf::Color::White);
    text2.setString("Enter Key");
    text2.setPosition(850, 400);

    RectangleShape inputBox(sf::Vector2f(400, 30));
    inputBox.setPosition(750, 500);
    inputBox.setFillColor(Color::Transparent);
    inputBox.setOutlineColor(sf::Color::Red);
    inputBox.setOutlineThickness(2.0f);

    Text userInput;
    userInput.setFont(font2);
    userInput.setCharacterSize(15);
    userInput.setFillColor(sf::Color::White);
    userInput.setPosition(755, 505);

    bool done = false;

    Text msg;
    msg.setFont(font);
    msg.setCharacterSize(20);
    msg.setFillColor(sf::Color::Red);
    msg.setPosition(740, 600);

    msg.setString("");

    while (window.isOpen()) {
        Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
            {
                window.close();
            }

            if (event.type == Event::TextEntered) {
                if (event.text.unicode < 128) {
                    if (event.text.unicode == 13 && userInput.getString().getSize()) {  // Enter key pressed

                        if (userInput.getString().getSize()) {
                            // File exists and is not empty
                            done = true;
                        }
                        else {
                            msg.setString("INVALID KEY!");
                            userInput.setString("");
                        }
                    }
                    else if (event.text.unicode == 8) {  // Backspace key pressed
                        string currentInput = userInput.getString();
                        if (!currentInput.empty()) {
                            currentInput.pop_back();
                            userInput.setString(currentInput);
                        }
                    }
                    else if (event.text.unicode >= 32) {  // Printable ASCII characters
                        char inputChar = static_cast<char>(event.text.unicode);
                        string currentInput = userInput.getString();
                        currentInput += inputChar;
                        key = stold(currentInput);
                        userInput.setString(currentInput);
                    }
                }
            }
        }

        window.clear();

        window.draw(background);
        window.draw(rectangle);
        window.draw(text2);
        window.draw(inputBox);
        window.draw(userInput);
        window.draw(msg);

        window.display();
        if (done)
        {
            successful_delete(window,background);
            break;
        }
    }
}
//------------------------------------------------------
void insetOrDelete(bool& insert, bool& deletee, bool& insert_machine, bool& deletee_machine,RenderWindow& window, Sprite& background)
{
    Font font;
    font.loadFromFile("fonts/lemon_milk/LEMONMILK-Regular.otf");

    RectangleShape rectangle;
    rectangle.setSize(sf::Vector2f(500, 500.f));
    rectangle.setFillColor(sf::Color(255, 0, 0, 128));

    rectangle.setPosition(700, 300);
    rectangle.setOutlineThickness(2.f);
    rectangle.setOutlineColor(Color::White);

    RectangleShape box1, box2, box3, box4;
    box1.setPosition(770, 400);
    box1.setFillColor(sf::Color::Transparent);
    box1.setOutlineColor(sf::Color::Red);
    box1.setOutlineThickness(2.0f);
    box1.setSize(Vector2f(150, 100));

    box2.setPosition(1000, 400);
    box2.setFillColor(sf::Color::Transparent);
    box2.setOutlineColor(sf::Color::White);
    box2.setOutlineThickness(2.0f);
    box2.setSize(Vector2f(150, 100));

    box3.setPosition(770, 600);
    box3.setFillColor(sf::Color::Transparent);
    box3.setOutlineColor(sf::Color::Red);
    box3.setOutlineThickness(2.0f);
    box3.setSize(Vector2f(150, 100));

    box4.setPosition(1000, 600);
    box4.setFillColor(sf::Color::Transparent);
    box4.setOutlineColor(sf::Color::White);
    box4.setOutlineThickness(2.0f);
    box4.setSize(Vector2f(150, 100));

    Text t1, t2, t3, t4, t5, t6, text;

    text.setFont(font);
    text.setFillColor(Color::White);
    text.setString("Do you want to: ");
    text.setCharacterSize(30);
    text.setPosition(770, 350);

    t1.setFont(font);
    t1.setFillColor(Color::White);
    t1.setString("Insert data");
    t1.setCharacterSize(20);
    t1.setPosition(775, 445);

    t2.setFont(font);
    t2.setFillColor(Color::White);
    t2.setString("Delete data");
    t2.setCharacterSize(20);
    t2.setPosition(1005, 445);

    t3.setFont(font);
    t3.setFillColor(Color::White);
    t3.setString("Insert");
    t3.setCharacterSize(20);
    t3.setPosition(805, 625);

    t4.setFont(font);
    t4.setFillColor(Color::White);
    t4.setString("Machine");
    t4.setCharacterSize(20);
    t4.setPosition(795, 645);

    t5.setFont(font);
    t5.setFillColor(Color::White);
    t5.setString("Delete");
    t5.setCharacterSize(20);
    t5.setPosition(1035, 625);

    t6.setFont(font);
    t6.setFillColor(Color::White);
    t6.setString("Machine");
    t6.setCharacterSize(20);
    t6.setPosition(1025, 645);


    int opt = 1;
    bool done = false;

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
            }
            if (event.type == sf::Event::KeyPressed)
            {
                if (event.key.code == sf::Keyboard::Left)
                {
                    if (opt != 1 && opt != 3)
                    {
                        opt--;
                    }
                }
                else if (event.key.code == sf::Keyboard::Right)
                {
                    if (opt != 2 && opt != 4)
                    {
                        opt++;
                    }
                }
                else if (event.key.code == sf::Keyboard::Up)
                {
                    if (opt != 1 && opt != 2)
                    {
                        opt -= 2;
                    }
                }
                else if (event.key.code == sf::Keyboard::Down)
                {
                    if (opt != 3 && opt != 4)
                    {
                        opt += 2;
                    }
                }
                else if (event.key.code == sf::Keyboard::Enter)
                {
                    done = true;
                    break;

                }
            }
            if (opt == 1)
            {
                box1.setOutlineColor(Color::Red);
                box2.setOutlineColor(Color::White);
                box3.setOutlineColor(Color::White);
                box4.setOutlineColor(Color::White);
            }
            else if (opt == 2)
            {
                box1.setOutlineColor(Color::White);
                box2.setOutlineColor(Color::Red);
                box3.setOutlineColor(Color::White);
                box4.setOutlineColor(Color::White);
            }
            else if (opt == 3)
            {
                box1.setOutlineColor(Color::White);
                box2.setOutlineColor(Color::White);
                box3.setOutlineColor(Color::Red);
                box4.setOutlineColor(Color::White);
            }
            else if (opt == 4)
            {
                box1.setOutlineColor(Color::White);
                box2.setOutlineColor(Color::White);
                box3.setOutlineColor(Color::White);
                box4.setOutlineColor(Color::Red);
            }

            window.clear();
            window.draw(background);
            window.draw(rectangle);
            window.draw(text);
            window.draw(box1);
            window.draw(box2);
            window.draw(box3);
            window.draw(box4);
            window.draw(t1);
            window.draw(t2);
            window.draw(t3);
            window.draw(t4);
            window.draw(t5);
            window.draw(t6);
            window.display();
        }
        if (done)
        {
            break;
        }
    }
    if (opt == 1)
    {
        //for insertion of data
        insert = true;
        deletee = false;
        insert_machine = false;
        deletee_machine = false;
    }
    else if (opt == 2)
    {
        //for deletion of data
        insert = false;
        deletee = true;
        insert_machine = false;
        deletee_machine = false;
    }
    else if (opt == 3)
    {
        //for insertion of machine
        insert = false;
        deletee = false;
        insert_machine = true;
        deletee_machine = false;
    }
    else if (opt == 4)
    {
        //for deletion of machine
        insert = false;
        deletee = false;
        insert_machine = false;
        deletee_machine = true;
    }
}
//---------------------------------------------

//======================================================
//-----------------------------------------------------------
void autoAssign(long double array[], long double space, long double machines)
{
    long int id;
    int count = 0;
    bool status = true;
    for (int i = 0; i < machines; i++)
    {
        id = rand() % long int(space);
        for (int j = 0; j < count; j++)
        {
            if (id == array[j])
            {
                status = false;
            }


        }

        if (status)
        {
            array[i] = id;
            count++;
        }
        else
        {
            i--;
            status = true;
        }

    }
    return;
}
//------------------------------------------------------------
void Ring_management(DHT_Ring<long int>* r1,int identifier, long double array[], long double space, long double machines)
{


   // DHT_Ring<long int> r1(identifier);

   long int count = 0;
    bool status = true;
    //long int id;

   /* while (count != machines)
    {
        id = rand() % long int(space);
        r1.addMachine(id, status);
        if (status)
        {
            count++;
        }
        status = true;
    }*/

    for (int i = 0; i < machines; i++)
    {
        r1->addMachine(array[i], status);
    }

    r1->setFTS();


    r1->print();



}
//============================================================
int main()
{
    RenderWindow window(sf::VideoMode(1940, 1100), "Bit Torrent");
    Texture bg_texture;
    Sprite background;
    bg_texture.loadFromFile("images/background.jpeg");
    background.setTexture(bg_texture);
    background.setScale(1.9, 1); 

    int identifier, opt;
    long double  spacerange;
    long int machines;
    DHT_Ring<long int>* r1 = new DHT_Ring<long int>(0);

    Texture bg_texture2;
    Sprite background2;
    bg_texture2.loadFromFile("images/background2.jpg");
    background2.setTexture(bg_texture2);
    background2.setScale(3.5, 3.3);

    Texture bg_texture3;
    Sprite background3;
    bg_texture3.loadFromFile("images/background3.jpg");
    background3.setTexture(bg_texture3);
    background3.setScale(3.5, 3);

    bool c1, c2;
    c1 = c2 = true;
    bool insert = false;
    bool deletee = false;
    bool insert_m = false;
    bool deletee_m = false;
    window.draw(background);
    window.display();
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        if (c1)
        {
            loadingScreen(window, background);
            window.clear();
            takeInput(window, background2, identifier, spacerange, machines, opt);
           // r1 = new DHT_Ring<long int>(identifier);
            c1 = false;
        }

        if (opt == 1 && c2)
        {
            long double* Machine_array = new long double[machines];
            InputIds(window, background3, Machine_array, machines, spacerange);
            
            
            r1 = new DHT_Ring<long int>(identifier);
            Ring_management(r1,identifier, Machine_array, spacerange,machines);

            //insetOrDelete(insert,deletee,window, background3);
            //selectFile(window, background3,r1, spacerange);

            c2 = false;
        }
        else if (opt == 2 && c2)
        {
            long double* Machine_array = new long double[machines];
            autoAssign(Machine_array, spacerange, machines);
            
            
            r1 = new DHT_Ring<long int>(identifier);
            Ring_management(r1,identifier, Machine_array, spacerange, machines);

            //insetOrDelete(insert, deletee,window, background3);
            //selectFile(window, background3,r1, spacerange);

            c2 = false;
        }
        insetOrDelete(insert, deletee, insert_m, deletee_m,window, background3);
        if (insert)
        {
            selectFile(window, background3, r1, spacerange);
            insert = false;
        }
        else if (deletee)
        {
            //cout << "unde construction\n";
            long double key;
            Key(key,window,background3);
            r1->delete_data(key);
            r1->print();
            deletee = false;
        }
        else if (insert_m)
        {
            long double key;
            Mac_Key(key, window, background3);
            r1->insertMachine(key);
            cout << "Ring after inserting new machine :" << key << endl;
            r1->print();
            //long int x;
            //cin >> x;
           // cout<<r1->search_data(x)->id;
            insert_m = false;

        }
        else if (deletee_m)
        {
            long double key;
            Key(key, window, background3);
            r1->delete_machine(key);
            cout << "Ring after deleting machine :" << key << endl;
            r1->print();
            //long int x;
            //cin >> x;
           // cout<<r1->search_data(x)->id;
            deletee_m = false;
        }


        window.draw(background);
        window.display();
    }

    return 0;
}


//255, 255, 255, 100