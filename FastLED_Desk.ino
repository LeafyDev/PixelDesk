#include "FastLED.h"
#include "Enums.h"

#define STRIP_PIN 6
#define RING_PIN 5

#define STRIP_LEDCOUNT 113
#define STRIP_ACTIVECOUNT 24

#define RING_LEDCOUNT 16

#define ANIMATION_DELAY 20

int sBrightness = 200;
int rBrightness = 5;

int pos = 0;

CRGB stripLEDs[STRIP_LEDCOUNT];
CRGB ringLEDs[RING_LEDCOUNT];

void clear_leds(target = both);
void set_color(target, int, CRGB, bool = true);

void sweep(target, CRGB&);
void rainbow_anim(byte);
CRGB rainbow_order(byte);

void setup()
{
	FastLED.addLeds<NEOPIXEL, 6>(stripLEDs, STRIP_LEDCOUNT);
	FastLED.addLeds<NEOPIXEL, 5>(ringLEDs, RING_LEDCOUNT);

	clear_leds();

	sweep(strip, CRGB::Red);
	sweep(strip, CRGB::Green);
	sweep(strip, CRGB::Blue);

	for (int i = 0; i < STRIP_ACTIVECOUNT; i++)
	{
		set_color(strip, i, CRGB::White);
		FastLED.delay(ANIMATION_DELAY);
	}

	sweep(ring, CRGB::Red);
	sweep(ring, CRGB::Green);
	sweep(ring, CRGB::Blue);

	for (int i = 0; i < RING_LEDCOUNT; i++)
	{
		set_color(ring, i, CRGB::White);
		FastLED.delay(ANIMATION_DELAY);
	}
}

void loop()
{
	rainbow_anim(pos);
	pos--;
	FastLED.delay(ANIMATION_DELAY * 4);
}

void clear_leds(const target board = both)
{
	switch (board)
	{
	case strip:
		fill_solid(stripLEDs, STRIP_ACTIVECOUNT, CRGB::Black);
		FastLED[strip].showLeds();
		break;
	case ring:
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

void set_color(const target board, const int pixel, const CRGB color, const bool show = true)
{
	switch (board)
	{
		case strip:
			stripLEDs[pixel].r = map(color.r, 0, 255, 0, sBrightness);
			stripLEDs[pixel].g = map(color.g, 0, 255, 0, sBrightness);
			stripLEDs[pixel].b = map(color.b, 0, 255, 0, sBrightness);
			break;
		case ring:
			ringLEDs[pixel].r = map(color.r, 0, 255, 0, rBrightness);
			ringLEDs[pixel].g = map(color.g, 0, 255, 0, rBrightness);
			ringLEDs[pixel].b = map(color.b, 0, 255, 0, rBrightness);
			break;
		default:
			break;
	}

	if (show)
		FastLED[board].showLeds();
}

void sweep(const target board, const CRGB& color)
{
	switch (board)
	{
		case strip:
			for (auto i = 0; i < STRIP_ACTIVECOUNT; i++)
			{
				set_color(strip, i, color);
				FastLED.delay(ANIMATION_DELAY);
				set_color(strip, i, CRGB::Black);
			}
			break;
		case ring:
			for (auto i = 0; i < RING_LEDCOUNT; i++)
			{
				set_color(ring, i, color);
				FastLED.delay(ANIMATION_DELAY);
				set_color(ring, i, CRGB::Black);
			}
			break;
		case none:
		case both:
		default:
			break;
	}
}

void rainbow_anim(const byte start_position)
{
	const auto rainbow_scale = 192 / RING_LEDCOUNT;

	for (auto i = 0; i < RING_LEDCOUNT; i++)
	{
		set_color(ring, i, rainbow_order(rainbow_scale * (i + start_position) % 192), false);
	}
	FastLED[ring].showLeds();
}

CRGB rainbow_order(byte position)
{
	CRGB ret;

	if (position < 31)
	{
		ret.r = 0xFF;
		ret.g = position * 8;
		ret.b = 0x00;
	}
	else if (position < 63)
	{
		position -= 31;

		ret.r = 0xFF - position * 8;
		ret.g = 0xFF;
		ret.b = 0x00;
	}
	else if (position < 95)
	{
		position -= 63;

		ret.r = 0x00;
		ret.g = 0xFF;
		ret.b = position * 8;
	}
	else if (position < 127)
	{
		position -= 95;

		ret.r = 0x00;
		ret.g = 0xFF - position * 8;
		ret.b = 0xFF;
	}
	else if (position < 159)
	{
		position -= 127;

		ret.r = position * 8;
		ret.g = 0x00;
		ret.b = 0xFF;
	}
	else
	{
		position -= 159;

		ret.r = 0xFF;
		ret.g = 0x00;
		ret.b = 0xFF - position * 8;
	}

	return ret;
}
