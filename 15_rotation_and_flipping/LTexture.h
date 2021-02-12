#include <SDL2/SDL.h>
#include <SDL2_Image/SDL_image.h>
#include <stdbool.h>
#include <stdio.h>

// Texture wrapper struct
typedef struct LTexture LTexture;

// creates LTexture with default values
LTexture LTextureNew( void );

// Deallocates LTexture
void LTextureFree( LTexture* lTexture );

// Loads image at specified path for LTexture
bool LTextureLoadFromFile( LTexture* lTexture, SDL_Renderer* gRenderer,
                           char* path );

// Renders texture at given point
void LTextureRender( LTexture* lTexture, SDL_Renderer* gRenderer, int x, int y,
                     SDL_Rect* clip, double angle, SDL_Point* center,
                     SDL_RendererFlip flip );

// Set color modulation
bool LTextureSetColor( LTexture* lTexture, Uint8 red, Uint8 green, Uint8 blue );

// Set blending
void LTextureSetBlendMode( LTexture* lTexture, SDL_BlendMode blending );

// Set alpha modulation
void LTextureSetAlpha( LTexture* lTexture, Uint8 alpha );

typedef struct LTexture {
  // The actual hardware texture
  SDL_Texture* mTexture;

  // Image dimensions
  int mWidth;
  int mHeight;
} LTexture;

LTexture LTextureNew() {
  LTexture lTexture = { NULL, 0, 0 };
  return lTexture;
}

void LTextureFree( LTexture* lTexture ) {
  if ( lTexture->mTexture != NULL ) {
    SDL_DestroyTexture( lTexture->mTexture );
    lTexture->mTexture = NULL;
    lTexture->mWidth = 0;
    lTexture->mHeight = 0;
  }
}

bool LTextureLoadFromFile( LTexture* lTexture, SDL_Renderer* gRenderer,
                           char* path ) {
  // make pixel art not blurry
  SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "0" );

  // Get rid of preexisting texture
  LTextureFree( lTexture );

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

void LTextureRender( LTexture* lTexture, SDL_Renderer* gRenderer, int x, int y,
                     SDL_Rect* clip, double angle, SDL_Point* center,
                     SDL_RendererFlip flip ) {
  // Set rendering space and render to screen
  SDL_Rect renderQuad = { x, y, lTexture->mWidth, lTexture->mHeight };

  // Set clip rendering dimensions
  if ( clip != NULL ) {
    renderQuad.w = clip->w;
    renderQuad.h = clip->h;
  }

  // Render to screen
  if ( SDL_RenderCopyEx( gRenderer, lTexture->mTexture, clip, &renderQuad,
                         angle, center, flip ) != 0 ) {
    printf( "Failed to render texture! SDL Error: %s\n", SDL_GetError() );
  }
}

bool LTextureSetColor( LTexture* lTexture, Uint8 red, Uint8 green,
                       Uint8 blue ) {
  // Modulate texture
  if ( SDL_SetTextureColorMod( lTexture->mTexture, red, green, blue ) != 0 ) {
    printf( "Failed to set color modulation! SDL Error: %s\n", SDL_GetError() );
    return false;
  }
  return true;
}

void LTextureSetBlendMode( LTexture* lTexture, SDL_BlendMode blending ) {
  SDL_SetTextureBlendMode( lTexture->mTexture, blending );
}

void LTextureSetAlpha( LTexture* lTexture, Uint8 alpha ) {
  SDL_SetTextureAlphaMod( lTexture->mTexture, alpha );
}