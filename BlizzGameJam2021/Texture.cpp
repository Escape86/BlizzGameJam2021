#include "Texture.h"
#include "SDL_image.h"
#include "SDL_ttf.h"

#ifdef _DEBUG
#include <assert.h>
#endif

const Uint8 opacityR = 0x00;
const Uint8 opacityG = 0x00;
const Uint8 opacityB = 0x00;

#pragma region Constructor

Texture::Texture(const std::string& path)
{
	this->path = path;
	this->isLoaded = false;
	this->isForText = false;
	this->renderOffsetX = this->width;
	this->renderOffsetY = this->height;
}

Texture::Texture()
{

}

#pragma endregion

#pragma region Public Methods

Texture::~Texture()
{
	//Free texture if it exists
	if (this->isLoaded)
	{
		SDL_DestroyTexture(this->sdl_texture);
		this->sdl_texture = nullptr;
		this->width = 0;
		this->height = 0;
		this->isLoaded = false;
	}
}

Texture* Texture::CreateFromText(const std::string& text, SDL_Color textColor, Display::FontSize fontSize)
{
	Texture* t = new Texture();

	//Render text surface
	SDL_Surface* textSurface = TTF_RenderText_Solid(Display::GetFont(fontSize), text.c_str(), textColor);
	if (textSurface == nullptr)
	{
		printf("Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError());
		return nullptr;
	}

	//Create texture from surface pixels
	t->sdl_texture = SDL_CreateTextureFromSurface(Display::GetRenderer(), textSurface);
	if (t == nullptr)
	{
		printf("Unable to create texture from rendered text! SDL Error: %s\n", SDL_GetError());
		return nullptr;
	}

	//Get image dimensions
	t->width = textSurface->w;
	t->height = textSurface->h;

	//Get rid of old surface
	SDL_FreeSurface(textSurface);

	t->isLoaded = true;
	t->isForText = true;

	return t;
}

bool Texture::Load()
{
	if (this->isLoaded)
		return true;

	//clear any texture we already have loaded
	if (this->sdl_texture)
	{
		SDL_DestroyTexture(this->sdl_texture);
		this->sdl_texture = nullptr;
		this->width = 0;
		this->height = 0;
	}

	//Load image at specified path
	SDL_Surface* loadedSurface = IMG_Load(path.c_str());
	if (loadedSurface == nullptr)
	{
		printf("Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError());
		return false;
	}

	//Color key image
	SDL_SetColorKey(loadedSurface, SDL_TRUE, SDL_MapRGB(loadedSurface->format, opacityR, opacityG, opacityB));

	//Create texture from surface pixels
	this->sdl_texture = SDL_CreateTextureFromSurface(Display::GetRenderer(), loadedSurface);
	if (this->sdl_texture == nullptr)
	{
		printf("Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError());
		return false;
	}

	//Get image dimensions
	this->width = loadedSurface->w;
	this->height = loadedSurface->h;

	//Get rid of old loaded surface
	SDL_FreeSurface(loadedSurface);

	this->isLoaded = true;

	return true;
}

void Texture::Draw(int x, int y, bool shiftToCenter, SDL_Rect* clip /*= nullptr*/, double angle /*= 0.0*/, SDL_Point* center /*= nullptr*/, SDL_RendererFlip flip /*= SDL_FLIP_NONE*/)
{
#ifdef _DEBUG
	assert(this->isLoaded);
#endif

	//Set rendering space and render to screen
	int renderX = this->isForText ? x : shiftToCenter ? x - this->renderOffsetX : x;
	int renderY = this->isForText ? y : shiftToCenter ? y - this->renderOffsetY : y;
	SDL_Rect renderQuad = { renderX, renderY, this->width, this->height };

	//Set clip rendering dimensions
	if (clip != nullptr)
	{
		renderQuad.w = clip->w;
		renderQuad.h = clip->h;
	}

	//Render to screen
	SDL_RenderCopyEx(Display::GetRenderer(), this->sdl_texture, clip, &renderQuad, angle, center, flip);
}

void Texture::SetRenderOffset(int offsetX, int offsetY)
{
	this->renderOffsetX = offsetX;
	this->renderOffsetY = offsetY;
}

int Texture::GetWidth() const
{
	return this->width;
}

int Texture::GetHeight() const
{
	return this->height;
}

#pragma endregion