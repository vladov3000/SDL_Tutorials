/*This source code copyrighted by Lazy Foo' Productions (2004-2020)
and may not be redistributed without written permission.*/

// Using SDL, SDL_image, standard math, and strings
#include "LTexture.h"
#include <time.h>

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

// The window renderer
SDL_Renderer* gRenderer = NULL;

// Scene texture
LTexture gModulatedTexture;

bool init() {
  // Initialization flag
  bool success = true;

  // Initialize SDL
  if ( SDL_Init( SDL_INIT_VIDEO ) < 0 ) {
    printf( "SDL could not initialize! SDL Error: %s\n", SDL_GetError() );
    success = false;
  } else {
    // Set texture filtering to linear
    if ( !SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "1" ) ) {
      printf( "Warning: Linear texture filtering not enabled!" );
    }

    // Create window
    gWindow = SDL_CreateWindow( "My Color Modulation", SDL_WINDOWPOS_UNDEFINED,
                                SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH,
                                SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
    if ( gWindow == NULL ) {
      printf( "Window could not be created! %s\n", SDL_GetError() );
      success = false;
    } else {
      // Create renderer for window
      gRenderer = SDL_CreateRenderer( gWindow, -1, SDL_RENDERER_ACCELERATED );
      if ( gRenderer == NULL ) {
        printf( "Renderer could not be created! SDL Error: %s\n",
                SDL_GetError() );
        success = false;
      } else {
        // Initialize renderer color
        SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );

        // Initialize PNG loading
        int imgFlags = IMG_INIT_PNG;
        if ( !( IMG_Init( imgFlags ) & imgFlags ) ) {
          printf( "SDL_image could not initialize! SDL_image Error: %s\n",
                  IMG_GetError() );
          success = false;
        }
      }
    }
  }

  return success;
}

bool loadMedia() {
  // Loading success flag
  bool success = true;

  // Load texture
  if ( !loadLTextureFromFile( &gModulatedTexture,
                              "my_color_modulation/spritesheet.png",
                              gRenderer ) ) {
    printf( "Failed to load colors texture!\n" );
    success = false;
  }

  return success;
}

void close() {
  // Free loaded images
  freeLTexture( &gModulatedTexture );

  // Destroy window
  SDL_DestroyRenderer( gRenderer );
  SDL_DestroyWindow( gWindow );
  gWindow = NULL;
  gRenderer = NULL;

  // Quit SDL subsystems
  IMG_Quit();
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

      // Modulation components
      Uint8 r = 255;
      Uint8 g = 255;
      Uint8 b = 255;

      // precompute color scheme

      int ticks = 0;
      int tickBy = 1;
      int ticksPerSecond = 10000;
      clock_t timer = clock();

      // While application is running
      while ( !quit ) {
        // Handle events on queue
        while ( SDL_PollEvent( &e ) != 0 ) {
          // User requests quit
          if ( e.type == SDL_QUIT ) {
            quit = true;
          }
        }

        double deltaTime = (double)( clock() - timer ) / CLOCKS_PER_SEC;
        if ( deltaTime > 1 / (double)ticksPerSecond ) {
          ticks += tickBy;
          ticks %= ( 6 * 0xFF );
          timer = clock();
          // printf( "Delta time: %f Target: %f\n", deltaTime,
          //         1 / (double)ticksPerSecond );
        }

        if ( ticks < 0xFF ) {
          r = 0xFF;
          g = ticks % 0xFF;
          b = 0;
        } else if ( ticks < 2 * 0xFF ) {
          r = 0xFF - ticks % 0xFF;
          g = 0xFF;
          b = 0;
        } else if ( ticks < 3 * 0xFF ) {
          r = 0;
          g = 0xFF;
          b = ticks % 0xFF;
        } else if ( ticks < 4 * 0xFF ) {
          r = 0;
          g = 0xFF - ticks % 0xFF;
          b = 0xFF;
        } else if ( ticks < 5 * 0xFF ) {
          r = ticks % 0xFF;
          g = 0;
          b = 0xFF;
        } else {
          r = 0xFF;
          g = 0;
          b = 0xFF - ticks % 0xFF;
        }

        // Clear screen
        SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
        SDL_RenderClear( gRenderer );

        // Modulate and render texture
        setColorLTexture( &gModulatedTexture, r, g, b );
        renderLTexture( &gModulatedTexture,
                        ( SCREEN_WIDTH - gModulatedTexture.mWidth * 4 ) / 2,
                        ( SCREEN_HEIGHT - gModulatedTexture.mHeight * 4 ) / 2,
                        NULL, gRenderer );

        // Update screen
        SDL_RenderPresent( gRenderer );
      }
    }
  }

  // Free resources and close SDL
  close();

  return 0;
}