#ifndef ROOM_H
#define ROOM_H

#include "User.h"
#include <vector>
#include <string>

class UserClient;
class Game;

class Room {
private:
    std::string name;
    std::vector<UserClient *> listUser;
    std::vector<UserClient *> disconnectedClient;
    std::vector<bool> ready;
    bool ingame = false;
    void setup(std::string name, std::vector<UserClient *> listUser, std::vector<bool> ready);
    
    Game *game;
public:
    Room();
    Room(std::string name, std::vector<UserClient *> listUser, std::vector<bool> ready);
    ~Room();

    int getNumberUser();
    UserClient* getHost();
    std::string getName();
    std::string char_list_msg;
    std::vector<UserClient *> getListUser();
    std::vector<UserClient *> getOnlineClient();
    std::vector<bool> getReady();
    Game* getGame();
    void setReady(UserClient *);
    void setReady(UserClient *, bool ready);
    void resetReady();
    
    void addUser(UserClient *);
    void removeUser(UserClient *);

    bool isIngame();
    void userDisconnectWhileInGame(UserClient *);
    void userReconnectWhileInGame(UserClient *);

    void startGame();
    void endGame();
};

#endif