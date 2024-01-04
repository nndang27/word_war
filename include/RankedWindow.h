#ifndef RankedWindow_H
#define RankedWindow_H

#include "LoginWindow.h"
#include <string>

class RankedWindow {
protected:
    Button  *back_btn;
    sf::Text *main;
    std::vector <sf::Text *> score_ranked_list;
public:
    std::string mode;

    std::unordered_map<std::string, std::string> listScore;
    RankedWindow(sf::Font *font);
    ~RankedWindow();

    void refresh();
    void update(sf::Vector2f mousePos);
    void typedOn(sf::Event ev);
    void drawTo(sf::RenderTarget &target);
    std::vector<sf::Text *> getUserScoreList();
    bool backPressed();
    
};

#endif