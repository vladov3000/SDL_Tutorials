#include <SDL2/SDL.h>
#include <SDL2_Image/SDL_image.h>
#include <stdbool.h>
#include <stdio.h>

// Texture wrapper struct
typedef struct LTexture LTexture;

// creates LTexture with default values
LTexture newLTexture( void );

// Deallocates LTexture
void freeLTexture( LTexture* lTexture );

// Loads image at specified path for LTexture
bool loadLTextureFromFile( LTexture* lTexture, char* path,
                           SDL_Renderer* gRenderer );

// Renders texture at given point
void renderLTexture( LTexture* lTexture, int x, int y, SDL_Rect* clip,
                     SDL_Renderer* gRenderer );

typedef struct LTexture {
  // The actual hardware texture
  SDL_Texture* mTexture;

  // Image dimensions
  int mWidth;
  int mHeight;
} LTexture;

LTexture newLTexture() {
  LTexture lTexture = { NULL, 0, 0 };
  return lTexture;
}

void freeLTexture( LTexture* lTexture ) {
  if ( lTexture->mTexture != NULL ) {
    SDL_DestroyTexture( lTexture->mTexture );
    lTexture->mTexture = NULL;
    lTexture->mWidth = 0;
    lTexture->mHeight = 0;
  }
}

bool loadLTextureFromFile( LTexture* lTexture, char* path,
                           SDL_Renderer* gRenderer ) {
  // Get rid of preexisting texture
  freeLTexture( lTexture );

  // The final texture
  SDL_Texture* newTexture = NULL;

  // Load image at specified path
  SDL_Surface* loadedSurface = IMG_Load( path );
  if ( loadedSurface == NULL ) {
    printf( "Unable to load image %s! SDL_image Error: %s\n", path,
            IMG_GetError() );
    return false;
  }

  // Color key image
  if ( SDL_SetColorKey( loadedSurface, SDL_TRUE,
                        SDL_MapRGB( loadedSurface->format, 0, 0xFF, 0xFF ) ) !=
       0 ) {
    printf( "Unable to load image %s! SDL_image Error: %s\n", path,
            SDL_GetError() );
    return false;
  }

  // Create texture from surface pixels
  newTexture = SDL_CreateTextureFromSurface( gRenderer, loadedSurface );
  if ( newTexture == NULL ) {
    printf( "Unable to create texture from %s! SDL Error: %s\n", path,
            SDL_GetError() );
    return false;
  }

  // Get image dimensions & texture
  lTexture->mTexture = newTexture;
  lTexture->mWidth = loadedSurface->w;
  lTexture->mHeight = loadedSurface->h;

  // Get rid of old loaded surface
  SDL_FreeSurface( loadedSurface );

  return true;
}

void renderLTexture( LTexture* lTexture, int x, int y, SDL_Rect* clip,
                     SDL_Renderer* gRenderer ) {
  // Set rendering space and render to screen
  SDL_Rect renderQuad = { x, y, lTexture->mWidth, lTexture->mHeight };

  // Set clip rendering dimensions
  if ( clip != NULL ) {
    renderQuad.w = clip->w;
    renderQuad.h = clip->h;
  }

  // Render to screen
  if ( SDL_RenderCopy( gRenderer, lTexture->mTexture, clip, &renderQuad ) !=
       0 ) {
    printf( "Failed to render texture! SDL Error: %s\n", SDL_GetError() );
  }
}