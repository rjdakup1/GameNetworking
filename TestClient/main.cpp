
/******************************************************************************
 * This is the client side of the Space Shooter project for CISS465
 *****************************************************************************/


/******************************************************************************
 * BEGIN Includes.
 *****************************************************************************/

// Standard includes

#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

// SDL wrapper from Dr. Liow

#include "Includes.h"
#include "Event.h"
#include "compgeom.h"
#include "Constants.h"
#include "Surface.h"
#include "play.h"


// SDL net
#include "SDL_net.h"


/******************************************************************************
 * END Includes.
 *****************************************************************************/


/******************************************************************************
 * Class definitions.
 *****************************************************************************/
class Player 
{
public:
	Player(int x1, int y1, int num = -1, int state = 0)
		: x(x1), y(y1), size(10), number(num), status(state)
	{
        switch(num)
        {
           case 0:
               color[0] = 255;
               color[1] = 0;
               color[2] = 0;
               break;

            case 1:
                color[0] = 0;
                color[1] = 255;
                color[2] = 0;
                break;

            case 2:
                color[0] = 0;
                color[1] = 0;
                color[2] = 255;
                break;
                
            default:
                color[0] = 255 / 10;
                color[1] = 255 / 5;
                color[2] = 255 / 3;
        }
	}
        
    int x, y, size;
	int number, status;
    int color[3];
    Rect rect;
};


/******************************************************************************
 * Global Constants.
 *****************************************************************************/
const int MAXLEN = 1024;


/******************************************************************************
 * Global Variables.
 *****************************************************************************/
std::vector<Player> players;
std::vector <Rect> alien;

// std::vector<Player> rect;
SDL_Thread *net_thread = NULL, *local_thread = NULL;
int player_number = -1;


/******************************************************************************
 * Functions
 *****************************************************************************/
// Receive a string over TCP/IP
std::string recv_message(TCPsocket sock)
{
    char buff[MAXLEN] = {' '};
    SDLNet_TCP_Recv(sock, buff, MAXLEN);

    if (buff == NULL)
    {
        std::string ret = "";
        return ret;
    }
    
    std::string ret(buff, strlen(buff));
    return ret;
}


// Send a string over TCP/IP
int send_message(std::string msg, TCPsocket sock)
{
    char * buff = (char *)msg.c_str();      
    SDLNet_TCP_Send(sock, buff, MAXLEN);

    return 1;
}


void parse_player_data(std::string message)
{
    std::stringstream message_stream(message);

    int num_players = 0, _status = 0;
    int _x, _y;
    
    message_stream >> num_players;
    
	for (int i = 0; i < num_players; i++)
    {
        if (i < players.size())
        {
            message_stream >> players[i].x >> players[i].y >> players[i].status;
        }
        
        else
        {
            message_stream >> _x >> _y >> _status;

            Player player(_x, _y, i, _status);
            players.push_back(player);
        }
    }
    for(int i = 0; i < 15; ++i)
    {
        if(i < alien.size())
        {
            message_stream >> alien[i].x >> alien[i].y;
        }
	}		
}


void recv_player_number(std::string message)
{
	int i = 0;
	std::string temp_num = "";

	if (message[0] == 'N')
    {
		i++;
		while (message[i] != ';')
        {
			temp_num += message[i];
			i++;
		}
	}

	player_number = atoi(temp_num.c_str());
}


int main(int argc, char **argv)
{
	IPaddress ip;
	TCPsocket sock;
  
	int numready;
	Uint16 port;
	SDLNet_SocketSet set;	

	std::string name;
	std::string to_server;
	std::string from_server;

    game();
    
    return 0;
}
