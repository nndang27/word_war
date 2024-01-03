#ifndef LOBBYWINDOW_H
#define LOBBYWINDOW_H

#include <iostream>
#include <sstream>
#include <list>
#include "GameConfig.h"
#include "Button.h"
#include "Textbox.h"
#include "Message.h"

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

class RoomBox;

class LobbyWindow {
private:
    Button *logout_btn;
    // Button *createRoom_btn;
    Button *joinRoom_btn;
    Button *mode_btn;
    Button *rankng_btn;
    
    std::vector<RoomBox *> listRoom;

    sf::Text *main;
    sf::Text *userName;
public:
    LobbyWindow(sf::Font *font);
    ~LobbyWindow();

    void setUsername(std::string username);

    void refresh();

    void update(sf::Vector2f mousePos);
    void updateRoom(struct update_lobby);
    
    void drawTo(sf::RenderTarget &target);

    // bool submitPressed(char *message, int *fail_type);
    bool logoutPressed(char *message);
    bool chooseModePressed(char *message);

    bool joinPressed(char *message);
    bool chooseModePressed();
    bool watchingRankedunPressed();
    // bool createRoomPressed();
};

#endif