/*This source code copyrighted by Lazy Foo' Productions (2004-2020)
and may not be redistributed without written permission.*/

// Using SDL, standard IO, and strings
#include <SDL2/SDL.h>
#include <stdbool.h>
#include <stdio.h>

// Screen dimension constants
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

// Key press surfaces constants
enum KeyPressSurfaces {
  KEY_PRESS_SURFACE_DEFAULT,
  KEY_PRESS_SURFACE_UP,
  KEY_PRESS_SURFACE_DOWN,
  KEY_PRESS_SURFACE_LEFT,
  KEY_PRESS_SURFACE_RIGHT,
  KEY_PRESS_SURFACE_TOTAL
};

// Starts up SDL and creates window
bool init( void );

// Load image for one keypress
bool loadImage( char* filename, int idx );

// Loads media
bool loadMedia( void );

// Frees media and shuts down SDL
void close( void );

// Loads individual image
SDL_Surface* loadSurface( char* path );

// The window we'll be rendering to
SDL_Window* gWindow = NULL;

// The surface contained by the window
SDL_Surface* gScreenSurface = NULL;

// The images that correspond to a keypress
SDL_Surface* gKeyPressSurfaces[KEY_PRESS_SURFACE_TOTAL];

// Current displayed image
SDL_Surface* gCurrentSurface = NULL;

bool init() {
  // Initialization flag
  bool success = true;

  // Initialize SDL
  if ( SDL_Init( SDL_INIT_VIDEO ) < 0 ) {
    printf( "SDL could not initialize! SDL Error: %s\n", SDL_GetError() );
    success = false;
  } else {
    // Create window
    gWindow = SDL_CreateWindow( "SDL Tutorial", SDL_WINDOWPOS_UNDEFINED,
                                SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH,
                                SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
    if ( gWindow == NULL ) {
      printf( "Window could not be created! SDL Error: %s\n", SDL_GetError() );
      success = false;
    } else {
      // Get window surface
      gScreenSurface = SDL_GetWindowSurface( gWindow );
    }
  }

  return success;
}

bool loadImage( char* filename, int idx ) {
  gKeyPressSurfaces[idx] = loadSurface( filename );
  if ( gKeyPressSurfaces[idx] == NULL ) {
    printf( "Failed to load %s!\n", filename );
    return false;
  }
  return true;
}

bool loadMedia() {
  // Loading success flag
  bool success = true;

  // Load default surface
  success |= loadImage( "04_key_presses/press.bmp", KEY_PRESS_SURFACE_DEFAULT );

  // Load up surface
  success |= loadImage( "04_key_presses/up.bmp", KEY_PRESS_SURFACE_UP );

  // Load down surface
  success |= loadImage( "04_key_presses/down.bmp", KEY_PRESS_SURFACE_DOWN );

  // Load left surface
  success |= loadImage( "04_key_presses/left.bmp", KEY_PRESS_SURFACE_LEFT );

  // Load right surface
  success |= loadImage( "04_key_presses/right.bmp", KEY_PRESS_SURFACE_RIGHT );

  return success;
}

void close() {
  // Deallocate surfaces
  for ( int i = 0; i < KEY_PRESS_SURFACE_TOTAL; ++i ) {
    SDL_FreeSurface( gKeyPressSurfaces[i] );
    gKeyPressSurfaces[i] = NULL;
  }

  // Destroy window
  SDL_DestroyWindow( gWindow );
  gWindow = NULL;

  // Quit SDL subsystems
  SDL_Quit();
}

SDL_Surface* loadSurface( char* path ) {
  // Load image at specified path
  SDL_Surface* loadedSurface = SDL_LoadBMP( path );
  if ( loadedSurface == NULL ) {
    printf( "Unable to load image %s! SDL Error: %s\n", path, SDL_GetError() );
  }

  return loadedSurface;
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

      // Set default current surface
      gCurrentSurface = gKeyPressSurfaces[KEY_PRESS_SURFACE_DEFAULT];

      // While application is running
      while ( !quit ) {
        // Handle events on queue
        while ( SDL_PollEvent( &e ) != 0 ) {
          // User requests quit
          if ( e.type == SDL_QUIT ) {
            quit = true;
          }
          // User presses a key
          else if ( e.type == SDL_KEYDOWN ) {
            // Select surfaces based on key press
            switch ( e.key.keysym.sym ) {
            case SDLK_UP:
              gCurrentSurface = gKeyPressSurfaces[KEY_PRESS_SURFACE_UP];
              break;

            case SDLK_DOWN:
              gCurrentSurface = gKeyPressSurfaces[KEY_PRESS_SURFACE_DOWN];
              break;

            case SDLK_LEFT:
              gCurrentSurface = gKeyPressSurfaces[KEY_PRESS_SURFACE_LEFT];
              break;

            case SDLK_RIGHT:
              gCurrentSurface = gKeyPressSurfaces[KEY_PRESS_SURFACE_RIGHT];
              break;

            default:
              gCurrentSurface = gKeyPressSurfaces[KEY_PRESS_SURFACE_DEFAULT];
              break;
            }
          }
        }

        // Apply the current image
        SDL_BlitSurface( gCurrentSurface, NULL, gScreenSurface, NULL );

        // Update the surface
        SDL_UpdateWindowSurface( gWindow );
      }
    }
  }

  // Free resources and close SDL
  close();

  return 0;
}