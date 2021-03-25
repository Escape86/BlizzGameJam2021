#pragma once

#include "SDL.h"
#include "Display.h"
#include <string>


class Texture
{
public:
	Texture(const std::string& path);
	~Texture();

	static Texture* CreateFromText(const std::string& text, SDL_Color textColor, Display::FontSize fontSize);

	bool Load();
	void Draw(int x, int y, bool shiftToCenter, SDL_Rect* clip = nullptr, double angle = 0.0, SDL_Point* center = nullptr, SDL_RendererFlip flip = SDL_FLIP_NONE);

	void SetRenderOffset(int offsetX, int offsetY);

	int GetWidth() const;
	int GetHeight() const;

private:
	Texture();	//for use with CreateFromText()

	bool isLoaded;
	int width;
	int height;
	std::string path;
	bool isForText;
	// rendering x and y pos
	int renderOffsetX;
	int renderOffsetY;

	SDL_Texture* sdl_texture;
};