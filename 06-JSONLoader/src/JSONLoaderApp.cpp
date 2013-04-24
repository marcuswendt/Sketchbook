#include <stdio.h>

#include "cinder/app/AppNative.h"
#include "cinder/gl/gl.h"

#include "cinder/Json.h"

#include "rapidjson/document.h"	
#include "rapidjson/reader.h"
#include "rapidjson/filestream.h"

using namespace ci;
using namespace ci::app;
using namespace std;
using namespace rapidjson;


class JSONLoaderApp : public AppNative {
  public:
	void setup();
	void mouseDown( MouseEvent event );	
	void update();
	void draw();
};

void JSONLoaderApp::setup()
{
    ci::Timer timer;
    std::string file;
    
//    file = "/Users/marcus/Documents/Development/102-chorus/Composer/export/animation/chorus-0.3.6-breaking-propaganda.json";
    
    file = "/Users/marcus/Documents/Development/102-chorus/Composer/export/animation/chorus-0.3.6-london-olympics-2012.json";

//    timer.start();
//    JsonTree data = JsonTree(ci::loadFile(file));
//    timer.stop();
//    console() << "cinder JsonTree parsed file in " << timer.getSeconds() << "s" << std::endl;
    //    for(JsonTree::Iter it=data.begin(); it != data.end(); it++) {
    //        console() << *it << std::endl;
    //    }
    
    
    Document document;
    FileStream is( std::fopen(file.c_str(), "r"));

    timer.start();
    document.ParseStream<0>(is);
    timer.stop();
    console() << "rapidjson parsed file in " << timer.getSeconds() << "s" << std::endl;
    
    static const char* kTypeNames[] = { "Null", "False", "True", "Object", "Array", "String", "Number" };

    for (Value::ConstMemberIterator itr = document.MemberBegin(); itr != document.MemberEnd(); ++itr) {
        printf("Type of member %s is %s\n", itr->name.GetString(), kTypeNames[itr->value.GetType()]);
    }
    
    // iterate over config
    const Value& config = document["config"];
    for (Value::ConstMemberIterator itr = config.MemberBegin(); itr != config.MemberEnd(); ++itr) {
        printf("Type of member %s is %s\n", itr->name.GetString(), kTypeNames[itr->value.GetType()]);
    }
}

void JSONLoaderApp::mouseDown( MouseEvent event )
{
}

void JSONLoaderApp::update()
{
}

void JSONLoaderApp::draw()
{
	// clear out the window with black
	gl::clear( Color( 0, 0, 0 ) ); 
}

CINDER_APP_NATIVE( JSONLoaderApp, RendererGl )
