#ifndef _NETWORK_H_
#define _NETWORK_H_

#include "utils_v1.h"
#include "structures.h"

/**
 * PRE: serverIP : a valid IP address
 *      serverPort: a valid port number
 * POST: on success, connects a client socket to serverIP:serverPort ;
 *       on failure, displays error cause and quits the program
 * RES: return socket file descriptor
 */
int initSocketClient(char *serverIP, int serverPort);

/**
 * PRE: 
 * 
 * POST: disconnect  all the players in the table given as parameter
 */
void disconnect_players(Player *tabPlayers, int nbPlayers);

/**
 * PRE:  serverPort: a valid port number
 * POST: on success, binds a socket to 0.0.0.0:serverPort and listens to it ;
 *       on failure, displays error cause and quits the program
 * RES:  return socket file descriptor
 */
int initSocketServer(int port);

#endif