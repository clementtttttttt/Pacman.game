#include "gsdl.hpp"

extern int sfreq;

SDL::SDL( Uint32 flags )
{
    if ( SDL_Init( flags ) != 0 )
        throw InitError();
    SDL_SetHint(SDL_HINT_EMSCRIPTEN_KEYBOARD_ELEMENT,"#canvas");
    SDL_SetHint(SDL_HINT_RENDER_VSYNC,"1");
    if ( SDL_CreateWindowAndRenderer( 720, 576, SDL_WINDOW_SHOWN,
                                      &m_window, &m_renderer ) != 0 )
        throw InitError();

    SDL_SetWindowTitle(m_window, "Pacman.");

    Uint16 sformat;
    int schannels;

    Mix_OpenAudio(44100,AUDIO_S16LSB,2,512);

    Mix_QuerySpec(&sfreq,&sformat,&schannels);

    SDL_RenderSetLogicalSize(m_renderer,720,576);
}

SDL::~SDL()
{
    SDL_DestroyWindow( m_window );
    SDL_DestroyRenderer( m_renderer );
    SDL_Quit();
}
InitError::InitError() :
    exception(),
    msg( SDL_GetError() )
{
}

InitError::InitError( const std::string & m ) :
    exception(),
    msg( m )
{
}

InitError::~InitError() throw()
{
}
const char * InitError::what() const throw()
{
    return msg.c_str();
}
