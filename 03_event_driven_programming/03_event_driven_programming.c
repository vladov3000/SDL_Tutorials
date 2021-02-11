/*This source code copyrighted by Lazy Foo' Productions (2004-2020)
and may not be redistributed without written permission.*/

// Using SDL and standard IO
#include <SDL2/SDL.h>
#include <stdbool.h>
#include <stdio.h>

// Screen dimension constants
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

// Starts up SDL and creates window
bool init( void );

// Loads media
bool loadMedia( void );

// Frees media and shuts down SDL
void close( void );

// The window we'll be rendering to
SDL_Window* gWindow = NULL;

// The surface contained by the window
SDL_Surface* gScreenSurface = NULL;

// The image we will load and show on the screen
SDL_Surface* gXOut = NULL;

bool init() {
  // Initialization flag
  bool success = true;

  // Initialize SDL
  if ( SDL_Init( SDL_INIT_VIDEO ) < 0 ) {
    printf( "SDL could not initialize! SDL_Error: %s\n", SDL_GetError() );
    success = false;
  } else {
    // Create window
    gWindow = SDL_CreateWindow( "SDL Tutorial", SDL_WINDOWPOS_UNDEFINED,
                                SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH,
                                SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
    if ( gWindow == NULL ) {
      printf( "Window could not be created! SDL_Error: %s\n", SDL_GetError() );
      success = false;
    } else {
      // Get window surface
      gScreenSurface = SDL_GetWindowSurface( gWindow );
    }
  }

  return success;
}

bool loadMedia() {
  // Loading success flag
  bool success = true;

  // Load splash image
  gXOut = SDL_LoadBMP( "03_event_driven_programming/x.bmp" );
  if ( gXOut == NULL ) {
    printf( "Unable to load image %s! SDL Error: %s\n",
            "03_event_driven_programming/x.bmp", SDL_GetError() );
    success = false;
  }

  return success;
}

void close() {
  // Deallocate surface
  SDL_FreeSurface( gXOut );
  gXOut = NULL;

  // Destroy window
  SDL_DestroyWindow( gWindow );
  gWindow = NULL;

  // Quit SDL subsystems
  SDL_Quit();
}

int main() {
  // Start up SDL and create window
  if ( !init() ) {
    printf( "Failed to initialize!\n" );
  } else {
    // Load media
    if ( !loadMedia() ) {
      printf( "Failed to load media!\n" );
    } else {
      // Main loop flag
      bool quit = false;

      // Event handler
      SDL_Event e;

      // While application is running
      while ( !quit ) {
        // Handle events on queue
        while ( SDL_PollEvent( &e ) ) {
          // User requests quit
          if ( e.type == SDL_QUIT ) {
            quit = true;
          }
          if ( e.type == SDL_WINDOWEVENT ) {
            // User minimizes window
            switch ( e.window.event ) {
            case SDL_WINDOWEVENT_MINIMIZED:
              SDL_Log( "Window %d minimzed", e.window.windowID );
              break;
            default:
              SDL_Log( "Window %d got unknown event %d", e.window.windowID,
                       e.window.event );
              break;
            }
          }
        }

        // Apply the image
        SDL_BlitSurface( gXOut, NULL, gScreenSurface, NULL );

        // Update the surface
        SDL_UpdateWindowSurface( gWindow );
      }
    }
  }

  // Free resources and close SDL
  close();

  return 0;
}