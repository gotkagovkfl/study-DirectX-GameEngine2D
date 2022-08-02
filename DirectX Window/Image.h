#ifndef _IMAGE_H
#define _IMAGE_H
#define WIN32_LEAN_AND_MEAN

#include "textureMAnager.h"
#include "constants.h"

class Image
{
	// Image properties
protected:
	Graphics* graphics;
	TextureManager* textureManager;
	// spriteData contains the data required to draw the image by Graphics::drawSprite()
	SpriteData spriteData;
	COLOR_ARGB colorFilter;
	int cols;
	int startFrame;
	int endFrame;
	int currentFrame;
	double frameDelay;
	double animTimer;
	HRESULT hr;
	bool loop;
	bool visible;
	bool initialized;
	bool animComplete;

public:
	Image();
	virtual ~Image();
	
	////////////////////////////////////////
	//           Get functions            //
	////////////////////////////////////////
	const virtual SpriteData& getSpriteInfo() { return spriteData; }
	virtual bool	getVisible() { return visible; }
	virtual float	getX() { return spriteData.x; }
	virtual float	getY() { return spriteData.y; }
	virtual float	getScale() { return spriteData.scale; }
	virtual int		getWidth() { return spriteData.width; }
	virtual int		getHeight() { return spriteData.height; }
	virtual float	getCenterX() { return spriteData.x + spriteData.width / 2 * getScale(); }
	virtual float	getCenterY() { return spriteData.y + spriteData.height / 2 * getScale(); }
	virtual float	getDegrees() { return spriteData.angle * (180.0f / (float)PI); }
	virtual float	getRadians() { return spriteData.angle; }
	virtual float	getFrameDelay() { return frameDelay; }
	virtual int		getStartFrame() { return startFrame; }
	virtual int		getEndFrame() { return endFrame; }
	virtual int		getCurrentFrame() { return currentFrame; }
	virtual RECT	getSpriteDataRect() { return spriteData.rect; }
	virtual bool	getAnimationComplete() { return animComplete; }
	virtual COLOR_ARGB getColorFilter() { return colorFilter; }

	////////////////////////////////////////
	//           Set functions            //
	////////////////////////////////////////
	virtual void	setX(float newX)		{ spriteData.x = newX; }
	virtual void	setY(float newY)		{ spriteData.y = newY; }
	virtual void	setScale(float s)		{ spriteData.scale = s; }
	virtual void	setDegrees(float deg)	{ spriteData.angle = deg * ((float)PI / 180.0f); }
	virtual void	setRadians(float rad)	{ spriteData.angle = rad; }
	virtual void	setVisible(bool v)		{ visible = v; }
	virtual void	setFrameDelay(float d)	{ frameDelay = d; }
	virtual void	setFrames(int s, int e) { startFrame = s; endFrame = e; }
	virtual void	setCurrentFrame(int c);
	virtual void	setRect();
	virtual void	setSpriteDataRect(RECT r) { spriteData.rect = r; }
	virtual void	setLoop(bool lp)			{ loop = lp; }
	virtual void	setAnimationComplete(bool a) { animComplete = a; }
	virtual void	setColorFilter(COLOR_ARGB color) { colorFilter = color; }
	virtual void	setTextureManager(TextureManager* textureM) {textureManager = textureM; }


	////////////////////////////////////////
	//         Other functions            //
	////////////////////////////////////////
	virtual bool initialize(Graphics* g, int width, int height, int ncols, TextureManager* textureM);
	
	virtual void flipHorizontal(bool flip)	{ spriteData.flipHorizontal = flip; }
	virtual void flipVertical(bool flip)	{ spriteData.flipVertical = flip; }

	// Draw Image using color as filter. Default color is WHITE.
	virtual void draw(COLOR_ARGB color = graphicsNS::WHITE);

	// Draw this image using the specified SpriteData.
	//   The current SpriteData.rect is used to select the texture.
	virtual void draw(SpriteData sd, COLOR_ARGB color = graphicsNS::WHITE); // draw with SpriteData using color as filter

	// Update the animation. frameTime is used to regulate the speed.
	virtual void update(float frameTime);
};

#endif