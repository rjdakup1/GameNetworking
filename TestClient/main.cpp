
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
	Player(int x1, int y1, int num = -1, int state = 0, int scores = 0)
		: x(x1), y(y1), size(10), number(num), status(state), score(scores) 
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
    int score;
};


/******************************************************************************
 * Global Constants.
 *****************************************************************************/
const int MAXLEN = 1024;


/******************************************************************************
 * Global Variables.
 *****************************************************************************/
std::vector<Player> players;

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
    int _score, _y;
    
    message_stream >> num_players;
    
	for (int i = 0; i < num_players; i++)
    {
        if (i < players.size())
        {
            message_stream >> players[i].score >> players[i].y >> players[i].status;
        }
        
        else
        {
            message_stream >> _score >> _y >> _status ;

            Player player(_score, _y, i, _status);
            players.push_back(player);
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


//----------------------------------------------------------------
// this is bull shit
//----------------------------------------------------------------

    Surface surface(W, H);
    Event event;
    
    Sound explosion("sounds/explosion.wav");
	Sound sound("sounds/laser.wav");
    Music music("sounds/GameLoop.ogg");		// create music object from file
   
    music.play();
    
    Image exp_1("images/galaxian/explosion_sprites_1.png");	// loads alien image 1
    Rect Exp_1 = exp_1.getRect();					    // creates alien rect

    Image exp_2("images/galaxian/explosion_sprites_2.png");	// loads alien image 2
    Rect Exp_2 = exp_2.getRect();					    // creates alien rect

    Image exp_3("images/galaxian/explosion_sprites_3.png");	// loads explosion image 3
    Rect Exp_3 = exp_3.getRect();					    // creates alien rect
    
    Image red("images/galaxian/GalaxianRedAlien.gif");	// loads alien image
    Image purple("images/galaxian/GalaxianPurpleAlien.gif");	// loads alien image	
    Image blue("images/galaxian/GalaxianAquaAlien.gif");	// loads alien image
    Image flagship("images/galaxian/GalaxianFlagship.gif");	// loads alien image
    	 	
    
	Image ship("images/galaxian/GalaxianGalaxip.gif");	// loads ships image
    Rect ships = ship.getRect();					    // creates ships rect
    Rect Alive = ship.getRect();
    Rect Live = ship.getRect();
   
	Font font("fonts/FreeMonoBold.ttf", 25); // Loads words
	Color c = {255, 255, 0};

    Image image(font.render("Lives:", c)); // Loads the text lives to screen
    Rect lives = image.getRect();

    Image lose(font.render("You Lose!!!", c)); // prints You Lose!!!
    Rect lost = lose.getRect();

    const int N = 500;
    
    Star star[N];
    
    int alien_count = 0; // Checking when missle needs to fire
    int num_aliens = 9; // Number of Aliens
    int aliens_postion = 0; // where the aliens are located on he screen
    int alien_rows = 5; // tells home me rows per level
    
    int num_missiles = 100; // Number of missles
    int w = 2, h = 6; // Missle size   
    int count = 0; // Animation for the explosion 
    int ship_lives = 3; // How many lives a player has
    int ships_x; // Allows you to move
    int n = 0;  // sends the a random alien to attack
    int a = 0; // tells when the alien to move down on screen
    int f = 0;  // Picks the random alien to move down 
    int r0 = 0;  // Moves the alien back to its orignal spot
    int score = 0; // The score of the player
    int lvl = 0; // the level the player is on
    int missilecd = 0; // cool down on the missles
    int reset = 19; // resets the aliens
    int fire = 0; // this fires and alien missle
    int timer= 0; // keeps time
       
	lives.x = 0;
	lives.y = H - 35;

    Alive.x = lives.x + 80;
    Alive.y = H - 35;
   
    Live.x = 105; // This needs to change
    Live.y = H - 35; 
  
    lost.x = 250;
    lost.y = H - 35; // all the way to here
    
    ships.x = 300;
	ships.y = H - 70; 
 
    Object Stats;
    bool game_over = false; // end the game
    bool ship_alive = true; // If ship is alive
    bool laser_alive = false; // If Lasers are alive
    bool alien_alive = false; // sets aliens to alive
    bool move = true; // tells the alien which way to travel 
    bool missile_cooldown = true; // missile cool down
    bool loaded = false; // missiles are loaded and ready to fire
    bool Extinguish = false; // missile gets destoryed
    bool attack = false; //
    
    //-------------------------------------------------------------------
    // This makes a vector for each of the following aliens missils
    // and the alien_missiles
    //-------------------------------------------------------------------
    std::vector <Rect> alien;
    std::vector <Rect> missile;
    std::vector <Rect> alien_missile;

    // setting the amount of missles the player gets to fire in  a vector
    for (int i = 0; i < num_missiles; i++)
    { 
        Rect l(0, 0, w, h);
        missile.push_back(l);     
    }
           
	while (1)
	{

        if (event.poll() && event.type() == QUIT) break; // exits game 

        Uint32 time = SDL_GetTicks(); // helps with frames per second
        
		KeyPressed keypressed = get_keypressed(); // checks keypressed

        if (timer == 200)
        {
            break;
        }
        if (ship_lives < 1)
        {
            game_over = true;
        }
        if (alien.size() == 0)
        {
            alien.clear();
            reset++;
        }

        //---------------------------------------------------------------------
        // creates new aliens for the level they are on
        //---------------------------------------------------------------------
        if(reset == 20)
        {
            if(alien.size() == 0)
            {
                for (int i = 0; i < alien_rows; ++ i)
                {
                    for (int j = 0; j < num_aliens; j++)
                    {
                        Rect a(40 + (i * 30)+ (j * 30), 10 +
                               (i * 20), 38, 38, i);    
                        alien.push_back(a);
                        
                    }
                    num_aliens -= 2;
                }
             
                for (int i = 0; i < alien.size(); ++i)
                {
                    alien[i].move = false;
                
                }
                
                ++lvl;
                
                for (int i = 0; i < 1 + lvl; i++)
                { 
                    Rect m(0, 0, w, h);
                    alien_missile.push_back(m);     
                }
                
                for(int i = 0; i < alien_missile.size(); ++i)
                {
                    alien_missile[i].move = false;
                }
                
                for (int i = 0; i < N; i++)
                {
                    star[i].pos() = vec2i(rand() % W, rand() % H);
                    star[i].vel() = vec2i(0, rand() % 3 + 1);
                }
                
            }

            
            num_aliens = 9 + (lvl * 2);           
            alien_rows++;
            reset = 0;
        }
        
        // Begin aliens attacking / moving
        if (a == 50 - lvl)
        {
            
            for (int i = 0; i < alien_missile.size(); ++i)
            {
                if (alien.size() > 0)
                {
                    n = rand() % alien.size();
                }
                for (int j = 0; j < alien.size(); ++j)
                {
                    alien_missile[i].x = alien[n].x + 14;
                    alien_missile[i].y = alien[n].y + 25;
                    alien_missile[i].move = true;
                }
            }
            alien_count = 0;
        }

       
         
        for (int i = 0; i < alien_missile.size(); ++i)
        {
            if(alien_missile[i].move == true)
            {
                if(alien_missile[i].y > H - 35)
                {
                    alien_missile[i].move = false;
                }
                else
                {
                    alien_missile[i].y += 5;
                }
            }
        }

        if(move) // moves aliens right
        {
            
            for (int j = 0; j < alien.size(); j++)
            {
                alien[j].x += 2;
            }
            for (int i = 0; i < alien.size(); i++)
            {
                if(alien[i].x == W - 30)
                {
                    move = false;
                }
            }
            
        }
            
        if (!move) // moves aliens left
        {
            
            for (int i = 0; i < alien.size(); i++)
            {
                alien[i].x -= 2;
            }

            for (int i = 0; i < alien.size(); i++)
            {
                if(alien[i].x == 0)
                {
                    move = true;
                }
            }
                
            
        }
        
        if (attack)
        {
            for (int i = 0; i < alien.size(); ++i)
            {
                if (alien[i].move)
                {
                    
                    if (a == 200)
                    {
                        alien_missile[fire].x = alien[i].x + 14;
                        alien_missile[fire].y = alien[i].y + 25;
                        alien_missile[fire].move = true;
                    }
                    alien[i].y += 1 + lvl % 2;
                    
                    if (alien[i].y > H - 35)
                    {
                        alien[i].y = 0;  
                    } 
                    if (alien[i].y == r0)
                    {
                        attack = false;
                        a = 0;
                        alien[i].move = false;
                    }
                }
                
            }
        }
        if (a == 140)
        {
            if (!attack)
            {
                f = rand() % alien.size();
                r0 = alien[f].y;
                fire = rand() % alien_missile.size();
                
            }
            attack = true;
            alien[f].move = true;
           
        }
        
    
        // End of alien attacking/moving
         
        // Start of work on the ship 
        if (count == 80 && ship_lives > 0) // checks if you have more lives  
        {
            count = 0;
            ship_alive = true;
        }
        
        if (ship_alive)
        {
            if (keypressed[LEFTARROW]) // Moves SpaceShip left
            {
                if (ships.x < 0)
                {
                    ships_x = 0;
                }
                else
                {
                    ships_x = -3;
                }
                ships.x += ships_x;
                
            }
        }
        
        if (ship_alive == true)
        {
            if (keypressed[RIGHTARROW]) // Moves SpaceShip Right
            {
                if (ships.x > W - 32)
                {
                    ships_x = 0;
                }
                else
                {
                    ships_x = 3;
                }
                ships.x += ships_x;
            }
        }
        
        if (!ship_alive) // resets the ship
        {
            count++;
        }

        // end of work on the ship

        // Start work on missle
        if (missilecd < 0)
        {
            if (keypressed[SPACE]) // Sets the x, y for the missle and sound for missle
            {
                if (ship_alive && missile_cooldown)
                {
                    
                    sound.play();
                    delay(5);
                    
                    
                  
                    missile[f].x = ships.x + 15;
                    missile[f].y = ships.y + 2 - (1 * 10);

                    if (f < missile.size())
                    {   
                        f++;
                    }
                    else
                    {
                        f = 0;
                    }
                    missile_cooldown = false;
                    loaded = false;
                    
                    missilecd = 20;
                }
                
            }
        }
        // Destroys the missle when its < 0
        for (int i = 0; i < missile.size(); ++i)
        {
            if(missile[i].y < 0)
            {
                laser_alive = false;
                missile_cooldown = true;
            }
        }
        // end or work on missiles
        
        // Checks if something collided!!
        for(int j = 0; j < missile.size(); ++j) 
        {
            for (int i = 0; i < alien.size(); ++i)
            { 
                if ( missile[j].collide(alien[i]))
                {
                    if (alien[i].move == true)
                    {
                        score += 10;
                        attack = false;
                        a = 0;
                    }
                    if (alien.size() <= alien_missile.size())
                    {
                        alien_missile.erase(alien_missile.begin());
                    }
                    if(alien[i].color < 1) 
                    {
                        score += 20;
                         for (int i = 0; i < players.size(); i++)
                         {
                             players[i].score = score;
                         }
                    }
                    
                    if(alien[i].color >= 1 && alien[i].color < 3) 
                    {
                         score += 15;
                    }
                    
                    if(alien[i].color >= 3 && alien[i].color < 7) 
                    {
                        score += 10;
                    }
                    
                    if(alien[i].color >= 7) 
                    {
                        score += 5;
                    }
               
             
                    alien.erase(alien.begin() + i);
                    missile[j].x = 0;
                    missile[j].y = 0;
                    missile_cooldown = true;
                   
                }
            }
        }

        
        // Checks if Alien Missile has collided with ship
        for(int j = 0; j < alien_missile.size(); ++j) 
        {
                if (alien_missile[j].collide(ships))
                {
                    explosion.play();
                    delay(10);
                    ship_alive = false;
                    alien_missile[j].x = 0;
                    alien_missile[j].y = 0;
                    alien_missile[j] = false;
                    ship_lives--;
                }
            
        } 
        for(int j = 0; j < alien.size(); ++j) 
        {
                if (alien[j].collide(ships))
                {
                    score += 10; 
                    explosion.play();
                    delay(10);
                    alien.erase(alien.begin() + j);
                    ship_alive = false;
                    alien_missile[j].x = 0;
                    alien_missile[j].y = 0;
                    attack = false;
                    a = 0;
                    ship_lives--;
                }
            
        } 
        

        // drawing stars


        for (int i = 0; i < N; i++)
        {
            star[i].update();
            
            if (star[i].y() >= H)
            {
                star[i].pos() = vec2i(rand() % W, 0);
            }
        }
        
        //******************************************************
        // starts drawing on screen
        //******************************************************
        surface.lock();
        surface.fill(BLACK);
        
        if (ship_alive)
        {
            surface.put_image(ship, ships); // puts Ships image at rect on surface
        }
       
        // ships explosion chain starts
        
        if (!ship_alive && ship_lives >= 0 && count < 20) // First Phase
        {
            Exp_1.x = ships.x;
            Exp_1.y = ships.y;
       
            surface.put_image(exp_1, Exp_1);
        }
        
        if (!ship_alive && ship_lives >= 0 && count < 40) // Second Phase
        {
            Exp_2.x = Exp_1.x;
            Exp_2.y = Exp_1.y;
       
            surface.put_image(exp_2, Exp_2);
        }

        if (!ship_alive && ship_lives >= 0 && count < 60) // Third Phase
        {
            Exp_3.x = Exp_2.x;
            Exp_3.y = Exp_2.y;
       
            surface.put_image(exp_3, Exp_3);
        }
        
        // explosion chain ends
     
            
        for (int i = 0; i < alien_missile.size(); ++i)
        {
            if (alien_missile[i].move && !game_over)
            {
                surface.put_rect(alien_missile[i].x, alien_missile[i].y, w, h, 255, 0 , 0); // draws lasers
            }
        }
        
        
        if (ship_alive) // Draws laser as long as its in permators
        {
            for (int i = 0; i < missile.size(); ++i)
            {
                surface.put_rect(missile[i].x, missile[i].y -= 10, w, h, 150, 150 , 150); // draws lasers
            }
        }
        
        // How many lives the player has left
        
        if (ship_lives == 3)
        {   
            surface.put_image(ship, Alive);
            surface.put_image(ship, Live);
        } 
        
        if (ship_lives == 2)
        {
            surface.put_image(ship, Alive);
        }
        
        for (int i = 0; i < alien.size(); ++i)
        {
            if(alien[i].color < 1 && !game_over) 
            {
                surface.put_image(flagship, alien[i]);
            }
         
            if(alien[i].color >= 1 && alien[i].color < 3 && !game_over) 
            {
                surface.put_image(red, alien[i]);
            }
         
            if(alien[i].color >= 3 && alien[i].color < 7 && !game_over) 
            {
                surface.put_image(purple, alien[i]);
            }
                     
            if(alien[i].color >= 7 && !game_over) 
            {
                surface.put_image(blue, alien[i]);
            }
               
            
        }
        for (int i = 0; i < N; i++)
        {
            surface.put_pixel(star[i].x(), star[i].y(), 255, 255, 0);
        }

        if(game_over)
        {
            surface.put_image(lose, lost);
            ++timer;
        }
        surface.put_image(image, lives);
        for (int i = 0; i < players.size(); i++)
        {
            Stats.score(surface, players[i].score);
        }
        Stats.level(surface, lvl);
        surface.unlock();
        surface.flip();
        
        if(20 > (SDL_GetTicks() -time))
        {
            SDL_Delay (20-(SDL_GetTicks()-time));
        }
        ++a;
        
        --missilecd;
    }
    

    


    
    
    return 0;
}
