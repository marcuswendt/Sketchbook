/*                                                                           
 *      _____  __  _____  __     ____                                   
 *     / ___/ / / /____/ / /    /    \   WorkerTest
 *    / ___/ /_/ /____/ / /__  /  /  /   (c) 2012, FIELD. All rights reserved.              
 *   /_/        /____/ /____/ /_____/    http://www.field.io           
 *   
 *	 Created by Marcus Wendt on 22/02/2011.
 */

#include "DrawingUtilities.h"

#include "cinder/app/App.h"
#include "cinder/Camera.h"
#include "cinder/gl/gl.h"
#include "cinder/gl/Fbo.h"

using namespace ci;
using namespace ci::app;

namespace field {

    void drawRect(const Rectf &rect)
    {
//        glEnableClientState( GL_VERTEX_ARRAY );
//        
//        GLfloat verts[8];
//        glVertexPointer(2, GL_FLOAT, 0, verts);
//        verts[0] = rect.getX1();
//        verts[1] = rect.getY1();
//        
//        verts[2] = rect.getX2();
//        verts[3] = rect.getY1();
//        
//        verts[4] = rect.getX2();
//        verts[5] = rect.getY2();
//        
//        verts[6] = rect.getX1();
//        verts[7] = rect.getY2();
//        
//        glDrawArrays(GL_QUADS, 0, 4 );
//        glDisableClientState( GL_VERTEX_ARRAY );
        
        glRectf(rect.getX1(), rect.getY1(), rect.getX2(), rect.getY2());
    }

    static ci::Font defaultFont;
    
    void drawText(std::string const& text, float x, float y, ci::Color const& color)
    {
        if(!defaultFont) 
            defaultFont = ci::Font("Monaco", 12);
        
        ci::gl::drawString(text, Vec2f(x,y), color, defaultFont);
    }
    
    void drawFFT(float* fftLeft, float* fftRight)
    {       
        float w = getWindowWidth() / (float)kAudioFFTNumBins;
        float h = getWindowHeight();
     
//        gl::enableAlphaBlending();
        gl::enableAdditiveBlending();
        
        float alpha = 0.75;
        float scaleY = 2.0;
        
        // left
        gl::color(ColorA(1.0,0.0,0.0,alpha));
        for(int i=0; i<kAudioFFTNumBins; i++) {
            float value = fftLeft[i] * scaleY;
            drawRect(Rectf(i * w, h, i * w + w, h - value));
        }    

        // right
        gl::color(ColorA(0.0,1.0,0.0,alpha));
        for(int i=0; i<kAudioFFTNumBins; i++) {
            float value = fftRight[i] * scaleY;
            drawRect(Rectf(i * w, h, i * w + w, h - value));
        }    
        
//        gl::disableAlphaBlending();
    }
}