/*This source code copyrighted by Lazy Foo' Productions (2004-2020)
and may not be redistributed without written permission.*/

// Using SDL, SDL_image, standard math, and strings
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

// Scene sprites
SDL_Rect gSpriteClips[4];
LTexture gSpriteSheetTexture;

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
          printf( "SDL_image could not initialize! SDL_mage Error: %s\n",
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

  // Load sprite sheet texture

  if ( !loadLTextureFromFile( &gSpriteSheetTexture,
                              "11_clip_rendering_and_sprite_sheets/dots.png",
                              gRenderer ) ) {
    printf( "Failed to load sprite sheet texture!\n" );
    success = false;
  } else {
    // Set top left sprite
    gSpriteClips[0].x = 0;
    gSpriteClips[0].y = 0;
    gSpriteClips[0].w = 100;
    gSpriteClips[0].h = 100;

    // Set top right sprite
    gSpriteClips[1].x = 100;
    gSpriteClips[1].y = 0;
    gSpriteClips[1].w = 100;
    gSpriteClips[1].h = 100;

    // Set bottom left sprite
    gSpriteClips[2].x = 0;
    gSpriteClips[2].y = 100;
    gSpriteClips[2].w = 100;
    gSpriteClips[2].h = 100;

    // Set bottom right sprite
    gSpriteClips[3].x = 100;
    gSpriteClips[3].y = 100;
    gSpriteClips[3].w = 100;
    gSpriteClips[3].h = 100;
  }

  return success;
}

void close() {
  // Free loaded images
  freeLTexture( &gSpriteSheetTexture );

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

      // While application is running
      while ( !quit ) {
        // Handle events on queue
        while ( SDL_PollEvent( &e ) != 0 ) {
          // User requests quit
          if ( e.type == SDL_QUIT ) {
            quit = true;
          }
        }

        // Clear screen
        SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
        SDL_RenderClear( gRenderer );

        // Render top left sprite
        renderLTexture( &gSpriteSheetTexture, 0, 0, &gSpriteClips[0],
                        gRenderer );

        // Render top right sprite
        renderLTexture( &gSpriteSheetTexture, SCREEN_WIDTH - gSpriteClips[1].w,
                        0, &gSpriteClips[1], gRenderer );

        // Render bottom left sprite
        renderLTexture( &gSpriteSheetTexture, 0,
                        SCREEN_HEIGHT - gSpriteClips[2].h, &gSpriteClips[2],
                        gRenderer );

        // Render bottom right sprite
        renderLTexture( &gSpriteSheetTexture, SCREEN_WIDTH - gSpriteClips[3].w,
                        SCREEN_HEIGHT - gSpriteClips[3].h, &gSpriteClips[0],
                        gRenderer );

        // Update screen
        SDL_RenderPresent( gRenderer );
      }
    }
  }

  // Free resources and close SDL
  close();

  return 0;
}