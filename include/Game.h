#ifndef GAME_H
#define GAME_H

#include <ctime>
#include <iostream>
#include <vector>

#include "UserClient.h"
#include "GameConfig.h"
#include "Game.h"
#include "Message.h"

class Game {
private:
    // Game logic
    std::vector<UserClient *> listUser;
    std::vector<int> x; // 0 - 5
    std::vector<int> y; // 0 - 4
    std::vector<int> point;
    std::vector<int> nb_word_done;
    std::string mode;
    int time_left; //60
    std::string target;
    bool endGame;
public:
    Game(std::vector<UserClient *>, std::string mode);
    virtual ~Game();

    const bool getEndGame() const;

    void setListUser(std::vector<UserClient *> input);

    std::vector<int> getX();
    std::vector<int> getY();
    std::vector<int> getPoint();
    std::vector<int> getNbWordDone();
    std::string getTarget();
    int getTimeLeft(); 

    // Functions
    void updateRandomTarget();
    void secondPass();
    //return true if need update target
    bool receivAction(struct rq_action, UserClient *&userClient, std::string char_list_msg, std::string mode);

    bool isEndGame();
};

#endif