#ifndef MESSAGE_H
#define MESSAGE_H

#include <string>
#include <string.h>
#include <sstream>
#include <iostream>
#include <vector>
#include "GameConfig.h"

enum MessageType {
RQ_EXIT = 0, 
RQ_REGISTER, RP_REGISTER, 
RQ_LOGIN, RP_LOGIN, 
RQ_LOGOUT, RP_LOGOUT, 
RQ_CREATE_ROOM, RP_CREATE_ROOM, 
RQ_JOIN_ROOM, RP_JOIN_ROOM,
UPDATE_LOBBY, UPDATE_ROOM,
RQ_READY, RQ_START,
START,
RQ_ACTION,
UPDATE_GAME, UPDATE_TARGET, 
END_GAME,
RQ_EXIT_ROOM};

enum Action {UP, DOWN, LEFT, RIGHT, SPACE};

extern std::vector<int> msg_length;

struct rq_exit {
    MessageType type = RQ_EXIT;
};

struct rq_register {
    MessageType type = RQ_REGISTER;
    std::string username;
    std::string password;
};

struct rp_register {
    MessageType type = RP_REGISTER;
    bool accept;
    std::string notification = " ";
};

struct rq_login {
    MessageType type = RQ_LOGIN;
    std::string username;
    std::string password;
};

struct rp_login {
    MessageType type = RP_LOGIN;
    bool accept;
    std::string notification = " ";
    std::string username;
};

struct rq_logout {
    MessageType type = RQ_LOGOUT;
};

struct rp_logout {
    MessageType type = RP_LOGOUT;
    bool accept;
    std::string notification = " ";
};
struct rq_create_room {
    MessageType type = RQ_CREATE_ROOM;
    std::string name;
};

struct rp_create_room {
    MessageType type = RP_CREATE_ROOM;
    bool accept;
    std::string notification = " ";
    std::string roomname = " ";
};

struct rq_join_room {
    MessageType type = RQ_JOIN_ROOM;
    std::string room_name;
};

struct rp_join_room {
    MessageType type = RP_JOIN_ROOM;
    bool accept;
    std::string notification = " ";
};

struct update_lobby{
    MessageType type = UPDATE_LOBBY;
    std::vector<std::string> name = {};
    std::vector<bool> ingame = {};
    std::vector<int> number_user = {};

};

struct update_room{
    MessageType type = UPDATE_ROOM;
    std::string room_name = "";
    std::vector<std::string> username = {};
    std::vector<bool> ready = {};
};

struct rq_ready{
    MessageType type = RQ_READY;
};

struct rq_start{
    MessageType type = RQ_START;
};

struct start {
    MessageType type = START;
};

struct rq_action {
    MessageType type = RQ_ACTION;
    Action action;
};

struct update_game {
    MessageType type = UPDATE_GAME;
    int time_left;
    std::vector<int> x;
    std::vector<int> y;
    std::vector<int> nb_word_done;
    std::vector<int> point;
};

struct update_target {
    MessageType type = UPDATE_TARGET;
    std::string target;
};

struct end_game {
    MessageType type = END_GAME;
    std::vector<int> point;
    std::vector<std::string> username;
};

struct rq_exit_room {
    MessageType type = RQ_EXIT_ROOM;
};

void struct_to_message(void *p, MessageType type, char *output,std::string& char_list_msg);

rq_register message_to_rq_register(char *message);
rp_register message_to_rp_register(char *message);

rq_login message_to_rq_login(char *message);
rp_login message_to_rp_login(char *message);

rq_logout message_to_rq_logout(char *message);
rp_logout message_to_rp_logout(char *message);

rq_create_room message_to_rq_create_room(char *message);
rp_create_room message_to_rp_create_room(char *message);

rq_join_room message_to_rq_join_room(char *message);
rp_join_room message_to_rp_join_room(char *message);

update_lobby message_to_update_lobby(char *message);
update_room message_to_update_room(char *message);

update_game message_to_update_game(char *message);
update_target message_to_update_target(char *message);
rq_action message_to_rq_action(char *message);
end_game message_to_end_game(char *message);

//Phân tách input bằng delimiter và trả về vector 
std::vector<std::string> split(char *input, std::string delimiter);

int getCode(char *input);



#endif