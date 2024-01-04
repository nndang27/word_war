#include "../include/RankedWindow.h"

using namespace std;

RankedWindow::RankedWindow(sf::Font *font) {
    for (int i = 0; i < 10; i++) {
        sf::Text *a = new sf::Text();
        a->setString("----------") ;
        a->setFont(*font);
        a->setFillColor(sf::Color::White);
        a->setCharacterSize(20);
        a->setPosition(500, 20*(i+10));
        this->score_ranked_list.push_back(a);

    }

    this->main = new sf::Text();
    this->main->setString("Ranked table");
    this->main->setFont(*font);
    this->main->setFillColor(sf::Color::White);
    this->main->setCharacterSize(50);
    auto textRect = this->main->getGlobalBounds();
    this->main->setOrigin(textRect.left + textRect.width/2.f, textRect.top + textRect.height/2.f);
    this->main->setPosition(screenWidth/2.f, 150);
    this->back_btn = new Button(50, 550, 60, 20, font, "Back", 16, sf::Color::Black, sf::Color::Magenta);
}

RankedWindow::~RankedWindow() {
    delete this->back_btn;
    delete this->main;

}

// void RankedWindow::refresh() {
//     this->roomname->refresh();
//     this->roomname->setSelected(true);
// }

void RankedWindow::update(sf::Vector2f mousePos) {
    this->back_btn->update(mousePos);
}

// void RankedWindow::typedOn(sf::Event ev) {
//     this->roomname->typedOn(ev);
// }
vector<sf::Text *> RankedWindow::getUserScoreList()
{
    return this->score_ranked_list;
}

void RankedWindow::drawTo(sf::RenderTarget &target) {
    target.draw(*this->main);
    this->back_btn->drawTo(target);
    for (int i = 0; i < 10; i++) {
        target.draw(*this->score_ranked_list.at(i));
    }
}

bool RankedWindow::backPressed() {
    return this->back_btn->isPressed();
}