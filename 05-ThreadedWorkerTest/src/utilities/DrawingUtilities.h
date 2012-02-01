/*                                                                           
 *      _____  __  _____  __     ____                                   
 *     / ___/ / / /____/ / /    /    \   WorkerTest
 *    / ___/ /_/ /____/ / /__  /  /  /   (c) 2012, FIELD. All rights reserved.              
 *   /_/        /____/ /____/ /_____/    http://www.field.io           
 *   
 *	 Created by Marcus Wendt on 22/02/2011.
 */

#pragma once

#include "cinder/Rect.h"
#include "cinder/Color.h"

namespace field {
	
    //! draws a single rectangle
    void drawRect(const ci::Rectf &rect);

    void drawText(std::string const& text, float x, float y, ci::Color const& color = ci::Color(1,1,1));
    
    //! draws the given fft data using rects
    void drawFFT(float* fftLeft, float* fftRight);
}