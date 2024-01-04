#ifndef SERVER_H
#define SERVER_H

#include "User.h"
#include "UserClient.h"
#include "Room.h"
#include "GameConfig.h"
#include "Message.h"
#include <stdio.h>
#include <iostream>
#include <vector>
#include <string.h>
#include <fstream>
#include <unordered_map>

#include <arpa/inet.h> // in_addr
#include <sys/socket.h> // socklen_t
#include <netdb.h> //dns
#include <unistd.h> //close, fork
#include <sys/wait.h> //waitpid
#include <pthread.h>

class Server {
private:
    int listenfd;
    struct sockaddr_in servAddr;

    std::vector<pthread_t> threads;

public:
    static std::vector<User *> listUser;
    static std::vector<Room *> listRoom;
    static std::vector<UserClient *> listClient;
    static std::unordered_map<std::string, std::string> listScore;
    Server();
    ~Server();

    static std::vector<std::string> listTarget;

    void run();

    static void rq_register(char *rq_register, char *rp_register);
    static void rq_login(char *rq_login, int connfd, UserClient *&userClient);
    static void rq_logout(char *rq_logout, char *rp_logout, UserClient *&userClient);
    static void rq_createRoom(char *rq_createRoom, char *rp_createRoom, UserClient *&userClient);
    static void rq_exitRoom(UserClient *&userClient);
    static void rq_joinRoom(char *rq_joinRoom, char *rp_joinRoom, UserClient *&userClient);
    static void rq_watch_ranked(char *rq_watchRanked, char *rp_watchRanked, UserClient *&userClient);
    static void rq_ready(UserClient *&userClient);
    static void rq_start(Room *room, char *rq_start_room);
    static void rq_action(char *rq_action, UserClient *&userClient);
    static void disconnect(UserClient *&userClient);
    static void reconnect(UserClient *&userClient, User *user);

    static struct update_lobby to_struct_update_lobby();
    static struct update_room to_struct_update_room(Room *&room);
    static struct update_game to_struct_update_game(Room *&room);
    static struct update_target to_struct_update_target(Room *&room);
    static struct end_game to_struct_end_game(Room *&room);
    static void updateLobby();
    static void updateLobby(UserClient *&userClient);
    static void updateRoom(Room *room);
    static void updateGame(Room *room);
    static void updateTarget(Room *room);
    static void deleteEmptyRoom();

    static void rcvFromClient(int connfd, char *rcv_message);
    static void sendToClient(int connfd, char *send_message);
    static void update_list_ranked(const end_game& res);
    void loadUserData(std::string path);
    void loadTarget(std::string path);
    void loadScore(std::string path);

    static void* routine1(void *);
    static void* routine2(void *);
    static void* time_routine(void *);
};


#endif