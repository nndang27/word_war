#include "../include/ModeWindow.h"

using namespace std;

ModeWindow::ModeWindow(sf::Font *font) {
    this->main = new sf::Text();
    this->main->setString("Choosing mode");
    this->main->setFont(*font);
    this->main->setFillColor(sf::Color::White);
    this->main->setCharacterSize(80);
    auto textRect = this->main->getGlobalBounds();
    this->main->setOrigin(textRect.left + textRect.width/2.f, textRect.top + textRect.height/2.f);
    this->main->setPosition(screenWidth/2.f, 150);

    this->easy_mode_btn = new Button(100, 250, 300, 100, font, "Easy", 36, sf::Color::Black, sf::Color::Magenta);
    this->hard_mode_btn = new Button(550, 250, 300, 100, font, "Hard", 36, sf::Color::Black, sf::Color::Magenta);
    this->detroy_mode_btn = new Button(400, 350, 300, 100, font, "Detroy", 36, sf::Color::Black, sf::Color::Magenta);
    this->back_btn = new Button(50, 550, 60, 20, font, "Back", 16, sf::Color::Black, sf::Color::Magenta);
}

ModeWindow::~ModeWindow() {
    delete this->easy_mode_btn;
    delete this->hard_mode_btn;
    delete this->detroy_mode_btn;
    delete this->back_btn;
    delete this->main;

}

// void ModeWindow::refresh() {
//     this->roomname->refresh();
//     this->roomname->setSelected(true);
// }

void ModeWindow::update(sf::Vector2f mousePos) {
    this->easy_mode_btn->update(mousePos);
    this->hard_mode_btn->update(mousePos);
    this->detroy_mode_btn->update(mousePos);
    this->back_btn->update(mousePos);
}

// void ModeWindow::typedOn(sf::Event ev) {
//     this->roomname->typedOn(ev);
// }

void ModeWindow::drawTo(sf::RenderTarget &target) {
    target.draw(*this->main);
    this->easy_mode_btn->drawTo(target);
    this->hard_mode_btn->drawTo(target);
    this->detroy_mode_btn->drawTo(target);
    this->back_btn->drawTo(target);
}

bool ModeWindow::easyModePressed(char *message, int *fail_type) {
    if(this->easy_mode_btn->isPressed()) {

        rq_choose_mode rq;
        rq.mode = "easy";
        this->mode = "easy";
        std::string char_list_msg="";
        struct_to_message(&rq, RQ_CHOOSE_MODE, message, char_list_msg);

        return true;
    }
    return false;
}

bool ModeWindow::hardModePressed(char *message, int *fail_type) {
    if(this->hard_mode_btn->isPressed()) {

        rq_choose_mode rq;
        rq.mode = "hard";
        this->mode = "hard";
        std::string char_list_msg="";
        struct_to_message(&rq, RQ_CHOOSE_MODE, message, char_list_msg);

        return true;
    }
    return false;
}

bool ModeWindow::detroyModePressed(char *message, int *fail_type) {
    if(this->detroy_mode_btn->isPressed()) {

        rq_choose_mode rq;
        rq.mode = "detroy";
        this->mode = "detroy";
        std::string char_list_msg="";
        struct_to_message(&rq, RQ_CHOOSE_MODE, message, char_list_msg);

        return true;
    }
    return false;
}

bool ModeWindow::backPressed() {
    return this->back_btn->isPressed();
}