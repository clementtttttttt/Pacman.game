#include <exception>
#include <string>
#include <SDL.h>
#include <SDL_mixer.h>

class InitError : public std::exception
{
    std::string msg;
public:
    InitError();
    InitError( const std::string & );
    virtual ~InitError() throw();
    virtual const char * what() const throw();
};



class SDL
{
    SDL_Window * m_window;
    SDL_Renderer * m_renderer;
public:
    SDL( Uint32 flags = 0 );
    void drawmouth(int x,int y, double radius, int ang1, int ang2);
    virtual ~SDL();
    void draw();
    void draw_gui();
};
