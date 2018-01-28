#include <FastLED.h>
#include "Enums.h"

#define STRIP_PIN 6
#define RING_PIN 5

#define STRIP_LEDCOUNT 113
#define STRIP_ACTIVECOUNT 24

#define RING_LEDCOUNT 16

#define ANIMATION_DELAY 20

int sBrightness = 230;
int rBrightness = 5;

int pos = 0;

CRGB stripLEDs[STRIP_LEDCOUNT];
CRGB ringLEDs[RING_LEDCOUNT];

void ClearLEDs(int = 0);
void sSetColor(int, CRGB, bool = true);
void rSetColor(int, CRGB, bool = true);

void Sweep(Mode, CRGB);

void rainbow(byte);
CRGB rainbowOrder(byte);


void setup()
{
	FastLED.addLeds<NEOPIXEL, 6>(stripLEDs, STRIP_LEDCOUNT);
	FastLED.addLeds<NEOPIXEL, 5>(ringLEDs, RING_LEDCOUNT);

	ClearLEDs();

	Sweep(strip, CRGB::Red);
	Sweep(strip, CRGB::Green);
	Sweep(strip, CRGB::Blue);

	/*for (int i = 0; i < STRIP_ACTIVECOUNT; i++)
	{
		sSetColor(i, CRGB::White);
		FastLED.delay(ANIMATION_DELAY);
	}*/

	Sweep(ring, CRGB::Red);
	Sweep(ring, CRGB::Green);
	Sweep(ring, CRGB::Blue);

	for (int i = 0; i < RING_LEDCOUNT; i++)
	{
		rSetColor(i, CRGB::White);
		FastLED.delay(ANIMATION_DELAY);
	}
}

uint8_t thishue = 0;
uint8_t deltahue = 15;

void loop()
{
	rainbow(pos);
	pos++;
	FastLED.delay(ANIMATION_DELAY * 5);
}

void rainbow(byte startPosition)
{
	int rainbowScale = 192 / RING_LEDCOUNT;

	for (int i = 0; i < RING_LEDCOUNT; i++)
	{
		rSetColor(i, rainbowOrder((rainbowScale * (i + startPosition)) % 192), false);
	}
	FastLED[ring].showLeds();
}

CRGB rainbowOrder(byte position)
{
	CRGB ret;

	if (position < 31)
	{
		ret.r = 0xFF;
		ret.g = position * 8;
		ret.b = 0x00;

		return ret;
	}
	else if (position < 63)
	{
		position -= 31;

		ret.r = 0xFF - position * 8;
		ret.g = 0xFF;
		ret.b = 0x00;

		return ret;
	}
	else if (position < 95)
	{
		position -= 63;

		ret.r = 0x00;
		ret.g = 0xFF;
		ret.b = position * 8;

		return ret;
	}
	else if (position < 127)
	{
		position -= 95;

		ret.r = 0x00;
		ret.g = 0xFF - position * 8;
		ret.b = 0xFF;

		return ret;
	}
	else if (position < 159)
	{
		position -= 127;

		ret.r = position * 8;
		ret.g = 0x00;
		ret.b = 0xFF;

		return ret;
	}
	else
	{
		position -= 159;

		ret.r = 0xFF;
		ret.g = 0x00;
		ret.b = 0xFF - position * 8;

		return ret;
	}
}

/// <summary>Clears either 1-strip, 2-ring, else all</summary>
void ClearLEDs(int type = 0)
{
	switch (type)
	{
		case 1:
			fill_solid(stripLEDs, STRIP_ACTIVECOUNT, CRGB::Black);
			FastLED[strip].showLeds();
			break;
		case 2:
			fill_solid(ringLEDs, RING_LEDCOUNT, CRGB::Black);
			FastLED[ring].showLeds();
			break;
		default:
			fill_solid(stripLEDs, STRIP_ACTIVECOUNT, CRGB::Black);
			fill_solid(ringLEDs, RING_LEDCOUNT, CRGB::Black);
			FastLED[strip].showLeds();
			FastLED[ring].showLeds();
			break;
	}
}

void sSetColor(int pixel, CRGB color, bool show = true)
{
	stripLEDs[pixel].r = map(color.r, 0, 255, 0, sBrightness);
	stripLEDs[pixel].g = map(color.g, 0, 255, 0, sBrightness);
	stripLEDs[pixel].b = map(color.b, 0, 255, 0, sBrightness);

	if(show)
		FastLED[strip].showLeds();
}

void rSetColor(int pixel, CRGB color, bool show = true)
{
	ringLEDs[pixel].r = map(color.r, 0, 255, 0, rBrightness);
	ringLEDs[pixel].g = map(color.g, 0, 255, 0, rBrightness);
	ringLEDs[pixel].b = map(color.b, 0, 255, 0, rBrightness);

	if (show)
		FastLED[ring].showLeds();
}

void Sweep(Mode mode, CRGB color)
{
	switch (mode)
	{
		case strip:
			for (int i = 0; i < STRIP_ACTIVECOUNT; i++)
			{
				sSetColor(i, color);
				FastLED.delay(ANIMATION_DELAY);
				sSetColor(i, CRGB::Black);
			}
			break;
		case ring:
			for (int i = 0; i < RING_LEDCOUNT; i++)
			{
				rSetColor(i, color);
				FastLED.delay(ANIMATION_DELAY);
				rSetColor(i, CRGB::Black);
			}
			break;
	}
}