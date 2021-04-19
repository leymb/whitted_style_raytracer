#include "precomp.h"

int main(int, char** )
{
	const bool exit = 0;
    renderer rMainRenderer;

    SDL_Event event;


    while (!exit)
    {
        while (SDL_PollEvent(&event) != 0);
    	
        rMainRenderer.Update();

    }
    return 0;
}




