#include "../include/Client.h"
#include <random>
#include <vector>
#include <algorithm>
using namespace std;

void Client::initNetwork()
{
    this->clientfd = socket(AF_INET, SOCK_STREAM, 0);
    this->listenfd = socket(AF_INET, SOCK_STREAM, 0);

    bzero(&this->servAddr, sizeof(this->servAddr));
    this->servAddr.sin_family = AF_INET;
    this->servAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    // htonl(INADDR_ANY);
    this->servAddr.sin_port = htons(SERVER_PORT);
}

void Client::initVariables()
{
    this->state = INTRO;
}

void Client::initWindow()
{
    this->videoMode.width = screenWidth;
    this->videoMode.height = screenHeight;
    this->window = new sf::RenderWindow(this->videoMode, titleGame, sf::Style::Titlebar | sf::Style::Close);
    this->window->setFramerateLimit(60);
    this->window->setVerticalSyncEnabled(true);
}

void Client::initIntroWindow()
{
    this->introWindow = new IntroWindow(this->font);
}

void Client::initRegisterWindow()
{
    this->registerWindow = new RegisterWindow(this->font);
}

void Client::initLoginWindow()
{
    this->loginWindow = new LoginWindow(this->font);
}

void Client::initLobbyWindow() {
    this->lobbyWindow = new LobbyWindow(this->font);
}

void Client::initNotification()
{
    this->notification = new Notification(this->font);
}

void Client::initCreateRoomWindow() {
    this->createRoomWindow = new CreateRoomWindow(this->font);
}

void Client::initModeWindow() {
    this->createModeWindow = new ModeWindow(this->font);
}

void Client::initJoinWindow() {
    this->joinWindow = new JoinWindow(this->font);
}

void Client::initGameWindow() {
    this->gameWindow = new GameWindow(this->font);
}

void Client::initRoomWindow() {
    this->roomWindow = new RoomWindow(this->font);
}

void Client::initFont()
{
    this->font = new sf::Font();
    this->font->loadFromFile(fontPerfectDosPath);
}

Client::Client()
{
    this->initNetwork();

    this->initFont();
    this->initVariables();
    this->initWindow();
    this->initIntroWindow();
    this->initRegisterWindow();
    this->initLoginWindow();
    this->initLobbyWindow();
    this->initNotification();
    this->initCreateRoomWindow();
    this->initModeWindow();
    this->initJoinWindow();
    this->initRoomWindow();
    this->initGameWindow();
}

Client::~Client()
{
    delete this->window;
    delete this->font;
    delete this->introWindow;
    delete this->registerWindow;
    delete this->loginWindow;
    delete this->lobbyWindow;
    delete this->notification;
    delete this->createRoomWindow;
    delete this->createModeWindow;
}

const bool Client::running() const
{
    return this->window->isOpen();
}

void Client::pollEvents()
{
    char send_msg[BUFF_SIZE + 1];
    char rcv_msg[BUFF_SIZE + 1];

    // Event polling
    while (this->window->pollEvent(this->ev)) {
        if (this->ev.type == sf::Event::Closed) {
            rq_exit temp;
            std::string char_list_msg="";
            struct_to_message(&temp, RQ_EXIT, send_msg, char_list_msg);
            this->sendToServer(this->clientfd, send_msg);
            
            this->sendToServer(this->listenfd, send_msg);
            this->window->close();
            exit(0);
            continue;
        }

        switch (this->state) {
        case INTRO:
            switch (ev.type) {
            case sf::Event::MouseButtonPressed:
                if (introWindow->registerPressed()) {
                    this->state = REGISTER;
                    this->registerWindow->refresh();
                }
                if (introWindow->loginPressed()) {
                    this->state = LOGIN;
                    this->loginWindow->refresh();
                }
                break;

            default:
                break;
            }
            break;
        case REGISTER:
            switch (ev.type) {
            case sf::Event::TextEntered:
                this->registerWindow->typedOn(ev);
                break;

            case sf::Event::MouseButtonPressed:
                if (registerWindow->backPressed()) {
                    this->state = INTRO;
                }
                int fail_type;
                if (registerWindow->submitPressed(send_msg, &fail_type)) {
                    switch (fail_type) {
                    case 0:
                        this->sendToServer(this->clientfd, send_msg);
                        this->rcvFromServer(this->clientfd, rcv_msg);
                        this->rp_register(rcv_msg);
                        break;
                    case 1:
                        this->notification->setText("Register Fail!!", 50, "Empty field", 30);
                        this->next_state = REGISTER;
                        break;
                    case 2:
                        this->notification->setText("Register Fail!!", 50, "Password not match!!", 30);
                        this->next_state = REGISTER;
                        break;
                    default:
                        break;
                    }

                    this->state = NOTIFICATION;
                }
            default:
                break;
            }
            break;
        case LOGIN:
            switch (ev.type) {
            case sf::Event::TextEntered:
                this->loginWindow->typedOn(ev);
                break;

            case sf::Event::MouseButtonPressed:
                if (loginWindow->backPressed()) {
                    this->state = INTRO;
                }
                int fail_type;
                if (loginWindow->submitPressed(send_msg, &fail_type)) {
                    if(fail_type == 0) {
                        this->sendToServer(this->clientfd, send_msg);
                        this->rcvFromServer(this->clientfd, rcv_msg);
                        this->rp_login(rcv_msg);
                    } else if (fail_type == 1) {
                        this->notification->setText("Login Fail!!", 50, "Empty field.", 30);
                        this->next_state = LOGIN;
                    }
                    this->state = NOTIFICATION;
                }
                break;
            default:
                break;
            }
            break;
        case NOTIFICATION:
            if (ev.type == sf::Event::MouseButtonPressed) {
                this->state = this->next_state;
            }
            break;
        case LOBBY:
            switch (ev.type)
            {
            case sf::Event::MouseButtonPressed:
                if (this->lobbyWindow->logoutPressed(send_msg)) {
                    this->sendToServer(this->clientfd, send_msg);
                    this->rcvFromServer(this->clientfd, rcv_msg);
                    this->rp_logout(rcv_msg);
                    this->next_state = INTRO;
                    this->state = NOTIFICATION;
                }
                if (this->lobbyWindow->joinPressed(send_msg)) {
                    this->joinWindow->refresh();
                    this->state = JOIN;
                }
                // if (this->lobbyWindow->createRoomPressed()) {
                //     this->state = CREATEROOM;
                //     this->createRoomWindow->refresh();
                // }
                if (this->lobbyWindow->chooseModePressed()) {
                    this->state = MODE;
                    // this->createModeWindow->refresh();
                }
                break;
            
            default:
                break;
            }
            break;
        case MODE:
            switch (ev.type)
            {
            case sf::Event::MouseButtonPressed:
                if (this->createModeWindow->backPressed()) {
                    this->state = LOBBY;
                }
                int fail_type;
                if (this->createModeWindow->easyModePressed(send_msg, &fail_type)) {
                        // this->sendToServer(this->clientfd, send_msg);
                        // this->rcvFromServer(this->clientfd, rcv_msg);
                        this->state = CREATEROOM;

                }
                if (this->createModeWindow->hardModePressed(send_msg, &fail_type)) {
                        // this->sendToServer(this->clientfd, send_msg);
                        // this->rcvFromServer(this->clientfd, rcv_msg);
                        this->state = CREATEROOM;

                }
                if (this->createModeWindow->detroyModePressed(send_msg, &fail_type)) {
                        // this->sendToServer(this->clientfd, send_msg);
                        // this->rcvFromServer(this->clientfd, rcv_msg);
                        this->state = CREATEROOM;

                }
                break;
            }
            break;
        case CREATEROOM:
            switch (ev.type)
            {
            case sf::Event::TextEntered:
                this->createRoomWindow->typedOn(ev);
                break;
            
            case sf::Event::MouseButtonPressed:
                if (this->createRoomWindow->backPressed()) {
                    this->state = MODE;
                }
                int fail_type;
                if (this->createRoomWindow->submitPressed(send_msg, &fail_type, this->createModeWindow->mode)) {
                    if(fail_type == 0) {
                        this->sendToServer(this->clientfd, send_msg);
                        this->rcvFromServer(this->clientfd, rcv_msg);
                        this->rp_createRoom(rcv_msg);
                    } else if (fail_type == 1) {
                        this->notification->setText("Create Room Fail!!", 50, "Empty field.", 30);
                        this->state = NOTIFICATION;
                        this->next_state = CREATEROOM;
                    }
                }
                break;
            }
            break;
        case JOIN:
            switch (ev.type)
            {
            case sf::Event::TextEntered:
                this->joinWindow->typedOn(ev);
                break;
            
            case sf::Event::MouseButtonPressed:
                if (this->joinWindow->backPressed()) {
                    this->state = LOBBY;
                }
                int fail_type;
                if (joinWindow->submitPressed(send_msg, &fail_type)) {
                    if(fail_type == 0) {
                        this->sendToServer(this->clientfd, send_msg);
                        this->rcvFromServer(this->clientfd, rcv_msg);
                        this->rp_joinRoom(rcv_msg);
                    } else if (fail_type == 1) {
                        this->notification->setText("Join Fail!!", 50, "Empty field.", 30);
                        this->state = NOTIFICATION;
                        this->next_state = CREATEROOM;
                    }
                }
                break;
            }
            break;
        case ROOM:
            switch (ev.type)
            {
            case sf::Event::MouseButtonPressed:
                if (this->roomWindow->backPressed(send_msg)) {
                    this->sendToServer(this->clientfd, send_msg);
                    this->state = LOBBY;
                } else if (this->roomWindow->readyPressed(send_msg)) {
                    this->sendToServer(this->clientfd, send_msg);
                } else if (this->roomWindow->startPressed(send_msg, this->createModeWindow->mode)) {
                    this->sendToServer(this->clientfd, send_msg);
                }

                break;
            }
            break;
        case GAME:
            switch (ev.type)
            {
            case sf::Event::KeyPressed:
                this->gameWindow->keyPressed(ev, send_msg);
                this->sendToServer(this->clientfd, send_msg);
                break;
            
            default:
                break;
            }
            break;
        default:
            break;
        }
    }
}

void Client::updateMousePositions()
{
    this->mousePosWindow = sf::Mouse::getPosition(*this->window);
    this->mousePosView = this->window->mapPixelToCoords(this->mousePosWindow);
}

void Client::update()
{
    this->pollEvents();

    this->updateMousePositions();

    switch (this->state) {
    case INTRO:
        this->introWindow->update(this->mousePosView);
        break;
    case REGISTER:
        this->registerWindow->update(this->mousePosView);
        break;
    case LOGIN:
        this->loginWindow->update(this->mousePosView);
        break;
    case LOBBY:
        this->lobbyWindow->update(this->mousePosView);
        break;
    case CREATEROOM:
        this->createRoomWindow->update(this->mousePosView);
        break;
    case MODE:
        this->createModeWindow->update(this->mousePosView);
        break;
    case JOIN:
        this->joinWindow->update(this->mousePosView);
        break;
    case ROOM:
        this->roomWindow->update(this->mousePosView);
        break;
    default:
        break;
    }
}

void Client::render()
{
    this->window->clear();

    // Draw game
    switch (this->state) {
    case INTRO:
        this->introWindow->drawTo(*this->window);
        break;

    case REGISTER:
        this->registerWindow->drawTo(*this->window);
        break;

    case LOGIN:
        this->loginWindow->drawTo(*this->window);
        break;

    case NOTIFICATION:
        this->notification->drawTo(*this->window);
        break;

    case LOBBY:
        this->lobbyWindow->drawTo(*this->window);
        break;
    case CREATEROOM:
        this->createRoomWindow->drawTo(*this->window);
        break;
    case MODE:
        this->createModeWindow->drawTo(*this->window);
        break;
    case JOIN:
        this->joinWindow->drawTo(*this->window);
        break;
    case ROOM:
        this->roomWindow->drawTo(*this->window);
        break;
    case GAME:
        this->gameWindow->drawTo(*this->window);
        break;
    default:
        break;
    }

    this->window->display();
}

void Client::sendToServer(int fd, char *buff)
{
    int ret;
    ret = send(fd, buff, strlen(buff), 0);
    if (ret < 0) {
        perror("Error: ");
        exit(1);
    }
    cout << "\nSend: " << "\n{\n" << buff << "\n}\n";
}
void Client::rcvFromServer(int fd, char *buff) {
    int ret;
    ret = recv(fd, buff, BUFF_SIZE, 0);
    if (ret < 0) {
        perror("Error: ");
        exit(1);
    }
    buff[ret] = '\0';
    cout << "\nReceiv: " << "\n{\n" << buff << "\n}\n";
}

void Client::rp_register(char *rq_message)
{
    struct rp_register rp = message_to_rp_register(rq_message);
    if (rp.accept) {
        this->notification->setText("Register Success!!", 50, "", 0);
        this->next_state = INTRO;
    } else {
        this->notification->setText("Register Fail!!", 50, rp.notification, 30);
        this->next_state = REGISTER;
    }
}

void Client::rp_login(char *rq_message) {
    struct rp_login rp = message_to_rp_login(rq_message);
    if (rp.accept) {
        this->notification->setText("Login Success!!", 50, "", 0);
        this->next_state = LOBBY;
        User *user = new User(rp.username, "");
        this->userClient = new UserClient(user);
        this->lobbyWindow->setUsername(this->userClient->getUser()->getUsername());
    } else {
        this->notification->setText("Login Fail!!", 50, rp.notification, 30);
        this->next_state = LOGIN;
    }

}

void Client::rp_logout(char *rq_message) {
    struct rp_logout rp = message_to_rp_logout(rq_message);
    if (rp.accept) {
        this->notification->setText("Logout Success!!", 50, "", 0);
        this->next_state = INTRO;
        delete this->userClient;
        this->userClient = nullptr;
    } else {
        this->notification->setText("Logout Fail!!", 50, rp.notification, 30);
        this->next_state = LOBBY;
    }
}

void Client::rp_createRoom(char *rq_message) {
    struct rp_create_room rp = message_to_rp_create_room(rq_message);
    if (rp.accept) {
        this->state = ROOM;
    } else {
        this->notification->setText("Create Room Fail!!", 50, rp.notification, 30);
        this->state = NOTIFICATION;
        this->next_state = LOBBY;
    }
}

void Client::rp_update_lobby(char *message) {
    struct update_lobby rp = message_to_update_lobby(message);
    this->lobbyWindow->updateRoom(rp);
}

void Client::rp_update_room(char *message) {
    struct update_room rp = message_to_update_room(message);
    this->roomWindow->updateRoom(rp, this->userClient);
}

void Client::rp_update_game(char *message) {
    struct update_game rp = message_to_update_game(message);
    this->gameWindow->updateGame(rp);
}

void Client::rp_update_target(char *message) {
    struct update_target rp = message_to_update_target(message);
    this->gameWindow->updateTarget(rp);
}

void Client::rp_resetCharacterBox(char *message) {
    auto splited_line = split(message, "\n");
    std::string values = splited_line.at(2);
    this->gameWindow->clearCharacterBox(this->font, values);
}

void Client::rp_joinRoom(char *message) {
    struct rp_join_room rp = message_to_rp_join_room(message);
    if (rp.accept) {
        this->state = ROOM;
    } else {
        this->notification->setText("Join Room Fail!!", 50, rp.notification, 30);
        this->state = NOTIFICATION;
        this->next_state = LOBBY;
    }
}

void Client::rp_end_game(char *message) {
    struct end_game res = message_to_end_game(message);
    this->state = NOTIFICATION;
    this->next_state = ROOM;

    stringstream notif;
    
    for(int i = 0; i < this->gameWindow->getNbPlayer(); i++) {
        notif << res.username.at(i) << ": " << res.point.at(i) << "\n";
    }

    for(int j = 0; j < this->gameWindow->getNbPlayer(); j++) {
        for (auto i = 0; i < 4; ++i) {
            if(this->roomWindow->getUsernameList().at(i).compare(res.username.at(j)) == 0){
                cout<<"String: "<<std::to_string(res.user_point_dict[res.username.at(j)])<<"\n";
                this->roomWindow->getUserScoreList().at(i)->setString(res.username.at(j)+" "+std::to_string(res.user_point_dict[res.username.at(j)]));
            }
        }
    }

    // this->roomWindow-
    this->notification->setText("End Game!!", 50, notif.str(), 30);
}

void Client::rp_start_game() {
    this->gameWindow->setNumberPlayer(this->roomWindow->getNumberPlayer());
    this->state = GAME;
}

void Client::run() {
    pthread_t thread1, thread2;
    if (connect(this->clientfd, (sockaddr*)&this->servAddr, sizeof(this->servAddr))) {
        perror("Error: ");
        exit(1);
    }
    if (connect(this->listenfd, (sockaddr*)&this->servAddr, sizeof(this->servAddr))) {
        perror("Error: ");
        exit(1);
    }
    if (pthread_create(&thread1, nullptr, Client::routine1, this) < 0) {
        perror("Could not create thread:");
        exit(1);
    }
    if (pthread_create(&thread2, nullptr, Client::routine2, this) < 0) {
        perror("Could not create thread:");
        exit(1);
    }
    pthread_join(thread1, nullptr);
    pthread_join(thread2, nullptr);

    close(this->clientfd);
    close(this->listenfd);

}

void * Client::routine1(void * c) {
    Client* client = (Client *)c;
    
    while(client->running()) {
        client->update();

        client->render();
    }
    return nullptr;
}

void * Client::routine2(void *c) {
    Client* client = (Client *)c;
    int connfd = client->listenfd;

    char rcv_message[BUFF_SIZE + 1], send_message[BUFF_SIZE + 1];
    
    do {
        client->rcvFromServer(connfd, rcv_message);

    } while(client->msg_handle(rcv_message));

    return nullptr;
}

UserClient* Client::getUserClient() {
    return this->userClient;
}

void getmessage(vector<string> *input, MessageType type, char *msg_out) {
    string temp = "";
    for(int i = 0; i < msg_length.at(type); i++) {
        temp += input->at(i) + "\n";
    }
    strcpy(msg_out, temp.c_str());
    input->erase(input->begin(), input->begin() + msg_length.at(type));
}

bool Client::msg_handle(char *message) {
    auto splited_line = split(message, "\n");

    if(splited_line.size() == 1 && splited_line.at(0).length() == 0) {
        return false;
    }

    char c_temp[BUFF_SIZE + 1];

    while(splited_line.size() > 1) {
        switch (stoi(splited_line.at(0)))
        {
            case UPDATE_LOBBY:
                getmessage(&splited_line, UPDATE_LOBBY, c_temp);
                this->rp_update_lobby(c_temp);
                break;
            case UPDATE_ROOM:
                getmessage(&splited_line, UPDATE_ROOM, c_temp);
                this->rp_update_room(c_temp);
                break;
            case START:
                this->rp_start_game();
                splited_line.erase(splited_line.begin(), splited_line.begin() + msg_length.at(START));
                break;
            case UPDATE_GAME:
                getmessage(&splited_line, UPDATE_GAME, c_temp);
                this->rp_update_game(c_temp);
                break;

            case END_GAME:

                getmessage(&splited_line, END_GAME, c_temp);
                this->rp_end_game(c_temp);
                break;

            case RP_JOIN_ROOM:
                getmessage(&splited_line, RP_JOIN_ROOM, c_temp);
                this->rp_joinRoom(c_temp);
                break;
            case UPDATE_TARGET:
                getmessage(&splited_line, UPDATE_TARGET, c_temp);
                // auto splited_line = split(c_temp, "\n");
                this->rp_update_target(c_temp);
                auto splited_line = split(c_temp, "\n");
                std::string values = splited_line.at(3);
                if(values.compare("hard") == 0){
                    this->rp_resetCharacterBox(c_temp);
                }
                if(values.compare("detroy") == 0){
                    this->rp_resetCharacterBox(c_temp);
                }
                
                break;
            // default:
            //     break;
        }
    }
    return true;
}