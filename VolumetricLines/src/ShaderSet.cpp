//
//  ShaderSet.cpp
//  VolumetricLines
//
//  Created by Marcus Wendt on 24/05/2011.
//  Copyright 2011 Marcus Wendt. All rights reserved.
//

#include "ShaderSet.h"

#include <cstdio>
#include <iostream>
#include <vector>
#include <map>
#include <boost/algorithm/string.hpp>
#include <boost/foreach.hpp>

#include "cinder/Buffer.h"

namespace fieldkit { namespace gl {
    
    template<typename _OutputIterator>
    void split(std::string str, std::string delim, _OutputIterator result) {
        typedef boost::algorithm::split_iterator<std::string::iterator> string_split_iterator;
        for(string_split_iterator iter=boost::algorithm::make_split_iterator(str,boost::algorithm::first_finder(delim, boost::algorithm::is_equal()));
            iter!=string_split_iterator();
            ++iter) {
                std::string component = boost::copy_range<std::string>(*iter);
                boost::trim(component); // trim whitespace
                *result = component;
                ++result;
        }
    }

    std::map<std::string, std::string> splitShaderSource(ci::DataSourceRef source)
    {
        using namespace std;
        
        ci::Buffer buffer = source->getBuffer();
        string str(static_cast<char*>(buffer.getData()), buffer.getDataSize());
        
        string del("---");
        vector<string> splitted;
        
        split(str, del, insert_iterator<vector<string> >(splitted, splitted.begin()));
        
        vector<string>::iterator it = splitted.begin();
        ++it; // skip header
        
        map<string, string> sources;
        for(; it != splitted.end(); ++it) {    
            string key(*it);
            ++it;
            string value(*it);
            sources.insert( pair<string, string>(key, value) );
//            printf("key '%s' '%s'", key.c_str(), value.c_str());
        }
        return sources;
    }
    
    
    ci::gl::GlslProg loadShaderSet(ci::DataSourceRef source,  GLint geometryInputType, GLint geometryOutputType, GLint geometryOutputVertices)
    {
        using namespace std;
        using ci::gl::GlslProg;
        
        map<string, string> sources = splitShaderSource(source);
        map<string, string>::iterator it = sources.begin();
        
        int numShaders = sources.size();

        if(numShaders == 1) {
            const char* vs = (*it).second.c_str();
            return GlslProg(vs);

        } else if(numShaders == 2) {
            const char* fs = (*it).second.c_str(); ++it;
            const char* vs = (*it).second.c_str();
            return GlslProg(vs, fs);

        } else if(numShaders == 3) {
            const char* fs = (*it).second.c_str(); ++it;
            const char* gs = (*it).second.c_str(); ++it;
            const char* vs = (*it).second.c_str();
            
//            printf("VS = '%s'\n", vs);
//            printf("GS = '%s'\n", gs);
//            printf("FS = '%s'\n", fs);
            
            return GlslProg(vs, fs, gs, geometryInputType, geometryOutputType, geometryOutputVertices);
        }
        
        return GlslProg();
    }
    
} }