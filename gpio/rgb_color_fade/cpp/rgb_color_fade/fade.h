#include "curry.h"
enum colors
{
    RED,
    GREEN,
    BLUE
};
auto Fade = [](int _color, bool _inc, int _r, int _g, int _b, int _delay) {
    int _col, _pin;
    int start, end;
    // Test Color
    if (_color == RED)
    {
        _pin = _r;
    }
    else if (_color == BLUE)
    {
        _pin = _b;
    }
    else if (_color == GREEN)
    {
        _pin = _g;
    }
    // Test ASC/DESC
    if (_inc)
    {
        start = 0;
        end = 255;
    }
    else
    {
        start = 255;
        end = 0;
    }
    // Test for loop
    if (end > start)
    {
        for (_col = start; _col <= end; _col++)
        {
            analogWrite(_pin, _col);
            if (_col < 180 || _col > 60)
            {
                delay(_delay);
            }
            else
            {
                delay(_delay * 10);
            }
        }
    }
    else
    {
        for (_col = start; _col >= end; _col--)
        {
            analogWrite(_pin, _col);
            if (_col > 60 || _col < 180)
            {
                delay(_delay);
            }
            else
            {
                delay(_delay * 10);
            }
        }
    }

};
auto FadeWithFlash = [](int _color, bool _inc, int _r, int _g, int _b, int _delay) {
    auto black = [_delay, _r, _g, _b]() {
        analogWrite(_r, 0);
        analogWrite(_g, 0);
        analogWrite(_b, 0);
        delay(_delay);

    };
    int _col, _pin;
    int start, end;
    // Test Color
    if (_color == RED)
    {
        _pin = _r;
    }
    else if (_color == BLUE)
    {
        _pin = _b;
    }
    else if (_color == GREEN)
    {
        _pin = _g;
    }
    // Test ASC/DESC
    if (_inc)
    {
        start = 0;
        end = 255;
    }
    else
    {
        start = 255;
        end = 0;
    }
    // Test for loop
    if (end > start)
    {
        for (_col = start; _col <= end; _col++)
        {
            int rand = random(0, 9);
            if (rand == 0 || rand == 1 || rand == 3)
                black();
            analogWrite(_pin, _col);
            if (_col < 180 || _col > 60)
            {
                delay(_delay);
            }
            else
            {
                delay(_delay * 10);
            }
        }
    }
    else
    {
        for (_col = start; _col >= end; _col--)
        {
            int rand = random(0, 9);
            if (rand == 0 || rand == 1 || rand == 3)
                black();
            analogWrite(_pin, _col);
            if (_col < 60 || _col > 180)
            {
                delay(_delay);
            }
            else
            {
                delay(_delay * 10);
            }
        }
    }
};
auto fadeRedInc = curry(Fade, RED, true);
auto fadeRedDec = curry(Fade, RED, false);
auto fadeBlueInc = curry(Fade, BLUE, true);
auto fadeBlueDec = curry(Fade, BLUE, false);
auto fadeGreenInc = curry(Fade, GREEN, true);
auto fadeGreenDec = curry(Fade, GREEN, false);
auto fadeRedIncFlash = curry(FadeWithFlash, RED, true);
auto fadeRedDecFlash = curry(FadeWithFlash, RED, false);
auto fadeBlueIncFlash = curry(FadeWithFlash, BLUE, true);
auto fadeBlueDecFlash = curry(FadeWithFlash, BLUE, false);
auto fadeGreenIncFlash = curry(FadeWithFlash, GREEN, true);
auto fadeGreenDecFlash = curry(FadeWithFlash, GREEN, false);