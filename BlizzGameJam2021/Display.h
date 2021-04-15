#pragma once

#include <vector>
#include <functional>
#include <map>
#include <string>
#include "SDL_events.h"
#include "SDL_ttf.h"

#pragma region Forward Declarations
struct SDL_Window;
struct SDL_Renderer;
class Texture;
#pragma endregion

enum RenderLayers
{
	GROUND = 0,
	OBJECTS,
	SPAWNS,
	PLAYER,
	UI,

	NUM_LAYERS
};

enum FontSize
{
	TWELVE = 12,
	SIXTEEN = 16,
	TWENTY = 20,
	THIRTYFOUR = 34,

	//remember to add corresponding load functionality when adding new font
};

class Display
{
public:
	Display() = delete;

	static bool Initialize();
	static bool ShutDown();
	static void InjectFrame();
	static void SetEventCallback(std::function<void(SDL_Event e)> eventCallback);

	static SDL_Renderer* const GetRenderer();
	static void QueueTextureForRendering(const Texture* texture, int x, int y, int width, int height, bool shiftToCenterPoint, RenderLayers layer,  bool isSpriteSheet = false, int spriteSheetOffsetX = 0, int spriteSheetOffsetY = 0);
	static void QueueRectangleForRendering(int x, int y, int width, int height, unsigned char r, unsigned char g, unsigned char b, RenderLayers layer);

	static TTF_Font* const GetFont(FontSize size);

	struct QueuedText
	{
		int x;
		int y;
		std::string text;
		SDL_Color textColor;
		FontSize fontsize;
		bool isVisible;
		int id;
		bool useChatBox;
	};

	static int CreateText(const std::string& text, int x, int y, FontSize fontSize, bool useChatBox, SDL_Color textColor = { 0, 0, 0 });
	static bool UpdateText(int id, const std::string& text, SDL_Color textColor = { 0, 0, 0 });
	static bool MoveText(int id, int x, int y);
	static bool SetTextIsVisible(int id, bool isVisible);
	static bool RemoveText(int id);

	static Uint8 GetRenderLayerOpacity(RenderLayers layer);
	static void SetRenderLayerOpacity(RenderLayers layer, Uint8 opacity);

private:
	static bool loadFonts();

	static SDL_Window* window;
	static SDL_Renderer* renderer;
	static std::map<FontSize, TTF_Font*> fonts;
	static std::function<void(SDL_Event e)> eventCallback;
	static SDL_Joystick* gameController;
	static int textControlIdCounter;

	static std::map<RenderLayers, Uint8> layerOpacity;

	struct QueuedTexture
	{
		const Texture* texture;
		int x;
		int y;
		int width;
		int height;
		bool shiftToCenterPoint;
		bool isSpriteSheet;
		int spriteSheetOffsetX;
		int spriteSheetOffsetY;
		RenderLayers layer;
	};
	static std::vector<QueuedTexture> textureQueue;

	struct QueuedRectangle
	{
		int x;
		int y;
		int width;
		int height;
		SDL_Color color;
		RenderLayers layer;
	};
	static std::vector<QueuedRectangle> rectangleQueue;

	static std::vector<QueuedText> textQueue;
};