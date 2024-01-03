#ifndef MODEWINDOW_H
#define MODEWINDOW_H

#include "LoginWindow.h"
#include <string>

class ModeWindow {
protected:
    Button  *back_btn, *easy_mode_btn, *hard_mode_btn, *detroy_mode_btn;
    sf::Text *main;
    
public:
    std::string mode;
    ModeWindow(sf::Font *font);
    ~ModeWindow();

    void refresh();
    void update(sf::Vector2f mousePos);
    void typedOn(sf::Event ev);
    void drawTo(sf::RenderTarget &target);
    bool easyModePressed(char *message, int *fail_type);
    bool hardModePressed(char *message, int *fail_type);
    bool detroyModePressed(char *message, int *fail_type);
    bool backPressed();
    
};

#endif