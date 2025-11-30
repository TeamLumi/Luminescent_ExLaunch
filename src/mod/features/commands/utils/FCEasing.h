// MIT License - Copyright (c) 2022 Jared Cook

/*
 * Easing Library
 *
 * References:
 * https://easings.net/
 * - no code taken from this one, but the graphs are useful visual reference
 * http://robertpenner.com/easing/
 * https://github.com/Michaelangel007/easing
 * https://github.com/Michaelangel007/easing#the-magic-of-170158
 * https://github.com/photonstorm/phaser/tree/master/src/math/easing
 *
 *
 * LICENSES
 *
 *
 * Original tween equations from Robert Penner:
 *	MIT License
 *
 * 	Copyright © 2001 Robert Penner
 *
 * 	Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files
 * 	(the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify,
 * 	merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is
 *  furnished to do so, subject to the following conditions:
 *
 * 	The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
 *
 * 	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 * 	OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
 * 	LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR
 *  IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * Elastic and Stepped easing equations adapted from Phaser:
 *https://github.com/photonstorm/phaser/blob/master/src/math/easing/elastic/Out.js
 * 	@author       Richard Davey <rich@photonstorm.com>
 * 	@copyright    2022 Photon Storm Ltd.
 * 	@license      {@link https://opensource.org/licenses/MIT|MIT License}
 *
 * Optimized easing functions from https://github.com/Michaelangel007/easing, who wrote this license in their code:
 * 	Optimized Easing Functions by Michael "Code Poet" Pohoreski, aka Michaelangel007
 * 	https://github.com/Michaelangel007/easing
 * 	License: Free as in speech and beer; Attribution is always appreciated!
 * 	Note: Please keep the URL so people can refer back to how these were derived.
 *
 */

#pragma once
#include <cmath>
#include <algorithm>

constexpr float PI = 3.14159265358979323846f;

enum EFCEase
{
    Linear,        //
    Smoothstep,    //
    Stepped,       //
    InSine,        //
    OutSine,       //
    InOutSine,     //
    InQuad,        //
    OutQuad,       //
    InOutQuad,     //
    InCubic,       //
    OutCubic,      //
    InOutCubic,    //
    InQuart,       //
    OutQuart,      //
    InOutQuart,    //
    InQuint,       //
    OutQuint,      //
    InOutQuint,    //
    InExpo,        //
    OutExpo,       //
    InOutExpo,     //
    InCirc,        //
    OutCirc,       //
    InOutCirc,     //
    InElastic,     //
    OutElastic,    //
    InOutElastic,  //
    InBounce,      //
    OutBounce,     //
    InOutBounce,   //
    InBack,        //
    OutBack,       //
    InOutBack      //
};

class FCEasing
{
public:

    static float Ease(float t, EFCEase EaseType);
    static float EaseWithParams(float t, EFCEase EaseType, float Param1, float Param2);

    static float EaseLinear(float t);
    static float EaseSmoothstep(float t, float x0 = 0.0f, float x1 = 1.0f);
    static float EaseStepped(float t, int Steps = 1);

    static float EaseInSine(float t);
    static float EaseOutSine(float t);
    static float EaseInOutSine(float t);

    static float EaseInQuad(float t);
    static float EaseOutQuad(float t);
    static float EaseInOutQuad(float t);

    static float EaseInCubic(float t);
    static float EaseOutCubic(float t);
    static float EaseInOutCubic(float t);

    static float EaseInQuart(float t);
    static float EaseOutQuart(float t);
    static float EaseInOutQuart(float t);

    static float EaseInQuint(float t);
    static float EaseOutQuint(float t);
    static float EaseInOutQuint(float t);

    static float EaseInExpo(float t);
    static float EaseOutExpo(float t);
    static float EaseInOutExpo(float t);

    static float EaseInCirc(float t);
    static float EaseOutCirc(float t);
    static float EaseInOutCirc(float t);

    static float EaseInElastic(float t, float Amp = 1.f, float Period = 0.3f);
    static float EaseOutElastic(float t, float Amp = 1.f, float Period = 0.3f);
    static float EaseInOutElastic(float t, float Amp = 1.f, float Period = 0.45f);

    static float EaseInBounce(float t);
    static float EaseOutBounce(float t);
    static float EaseInOutBounce(float t);

    static float EaseInBack(float t, float Overshoot = 1.70158f);
    static float EaseOutBack(float t, float Overshoot = 1.70158f);
    static float EaseInOutBack(float t, float Overshoot = 1.70158f);
};
