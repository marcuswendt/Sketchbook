//
//  ShaderSet.h
//  VolumetricLines
//
//  Created by Marcus Wendt on 24/05/2011.
//  Copyright 2011 Marcus Wendt. All rights reserved.
//

#pragma once

#include "cinder/DataSource.h"
#include "cinder/gl/GlslProg.h"

namespace fieldkit { namespace gl {


    ci::gl::GlslProg loadShaderSet(ci::DataSourceRef source,
                                   std::string vertexShaderName,
                                   std::string fragmentShaderName,
                                   std::string geometryShaderName = "",
                                   GLint geometryInputType = GL_POINTS, 
                                   GLint geometryOutputType = GL_TRIANGLES, 
                                   GLint geometryOutputVertices = 0);
    
} }