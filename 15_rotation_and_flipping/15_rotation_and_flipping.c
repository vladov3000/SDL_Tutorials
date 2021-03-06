/*This source code copyrighted by Lazy Foo' Productions (2004-2020)
and may not be redistributed without written permission.*/

// Using SDL, SDL_image, standard IO, math, and strings
#include "LTexture.h"

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
LTexture gArrowTexture;

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
    gWindow = SDL_CreateWindow( "SDL Tutorial", SDL_WINDOWPOS_UNDEFINED,
                                SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH,
                                SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
    if ( gWindow == NULL ) {
      printf( "Window could not be created! SDL Error: %s\n", SDL_GetError() );
      success = false;
    } else {
      // Create vsynced renderer for window
      gRenderer = SDL_CreateRenderer(
          gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC );
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

  // Load arrow
  if ( !LTextureLoadFromFile( &gArrowTexture, gRenderer,
                              "15_rotation_and_flipping/arrow.png" ) ) {
    printf( "Failed to load arrow texture!\n" );
    success = false;
  }

  return success;
}

void close() {
  // Free loaded images
  LTextureFree( &gArrowTexture );

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

      // Angle of rotation
      double degrees = 0;

      // Flip type
      SDL_RendererFlip flipType = SDL_FLIP_NONE;

      // While application is running
      while ( !quit ) {
        // Handle events on queue
        while ( SDL_PollEvent( &e ) != 0 ) {
          // User requests quit
          if ( e.type == SDL_QUIT ) {
            quit = true;
          } else if ( e.type == SDL_KEYDOWN ) {
            switch ( e.key.keysym.sym ) {
            case SDLK_a:
              degrees -= 60;
              break;

            case SDLK_d:
              degrees += 60;
              break;

            case SDLK_q:
              flipType = SDL_FLIP_HORIZONTAL;
              break;

            case SDLK_w:
              flipType = SDL_FLIP_NONE;
              break;

            case SDLK_e:
              flipType = SDL_FLIP_VERTICAL;
              break;
            }
          }
        }

        // Clear screen
        SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
        SDL_RenderClear( gRenderer );

        // Render arrow
        LTextureRender( &gArrowTexture, gRenderer,
                        ( SCREEN_WIDTH - gArrowTexture.mWidth ) / 2,
                        ( SCREEN_HEIGHT - gArrowTexture.mHeight ) / 2, NULL,
                        degrees, NULL, flipType );

        // Update screen
        SDL_RenderPresent( gRenderer );
      }
    }
  }

  // Free resources and close SDL
  close();

  return 0;
}