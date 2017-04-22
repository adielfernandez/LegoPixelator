#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){

    ofSetVerticalSync(false);
    ofSetFrameRate(200);
    
    cam.setup(w, h);
    
    
    guiName = "settings";
    
    gui.setup(guiName, guiName + ".xml", 0, 0);
    gui.add(pixelSize.setup("Pixel Size", 20, 10, 64));
    gui.add(use8BitColor.setup("Use 8 bit color", false));
    gui.add(saturationMult.setup("Saturation Mult.", 1.2, 0.0, 2.0));
    gui.add(brightnessMult.setup("Brightness Mult.", 1.15, 0.0, 2.0));
    gui.add(useLegoPixels.setup("Use Lego Pixels", true));
    
    gui.loadFromFile(guiName + ".xml");
    
    //common factorsof 640 & 480
    //1, 2, 4, 5, 8, 10, 16, 20, 32, 40, 80, 160
    
    fbo.allocate(w * 2, h * 2);
    
    fbo.begin();
    ofClear(255, 255);
    fbo.end();
    
    ofImage lego;
    lego.load("legoPixel.png");
    legoTex = lego.getTexture();
    
    
}

//--------------------------------------------------------------
void ofApp::update(){

    cam.update();
    
    if(cam.isFrameNew()) {

        
        //store some framerate data
        float thisFrameRate = 1.0/( (ofGetElapsedTimef() - lastFrameTime) );
        
        //log frame rates for each camera and average with the
        //last recorded frame rate to smooth a little
        camFrameRate = (thisFrameRate + lastFrameRate)/2;
        lastFrameRate = thisFrameRate;
        lastFrameTime = ofGetElapsedTimef();
        
        fbo.begin();
        
        //draw squares at the pixel size
        for( int y = 0; y < h; y += pixelSize ){
            for( int x = 0; x < w; x += pixelSize ){
            
                //get the color from the image at
                //the center of the pixel
                ofColor c;
                c = cam.getPixels().getColor(x + pixelSize/2, y + pixelSize/2);
                
                
                
                    c.setHsb(c.getHue(), c.getSaturation() * saturationMult, c.getBrightness() * brightnessMult);
                
                if( use8BitColor ){
                    //R, G and B need to get dropped to different bit depths
                    //8 bit color = 3 bit red, 3 bit green, 2 bit blue
                    // => 8 levels of red, 8 of green 4 of blue
                    //Then scale up to a 255 scale to fake it on screen
                    c.r = floor( (c.r/256.0f) * 8 ) * (int)(255/8);
                    c.g = floor( (c.g/256.0f) * 8 ) * (int)(255/8);
                    c.b = floor( (c.b/256.0f) * 4 ) * (int)(255/4);
                    
                }
                
                ofSetColor(c);
                
                //FBO is twice the size for better resolution output
                //scale everything by 2
                if ( !useLegoPixels ){
                    ofDrawRectangle(x * 2, y * 2, pixelSize * 2, pixelSize * 2);
                } else {
                    legoTex.draw(x * 2, y * 2, pixelSize * 2, pixelSize * 2);
                }
                
            }
        }

        fbo.end();
        
        
        

        

        
        
        
    }
    
}

//--------------------------------------------------------------
void ofApp::draw(){

    ofSetBackgroundColor(30);
    
    ofSetColor(255);
    ofDrawBitmapString("Framerate: " + ofToString(ofGetFrameRate()), 10, 15);
    ofDrawBitmapString("Cam Framerate: " + ofToString(camFrameRate), 10, 30);
    
    
    
    int leftMargin = 240;
    int topMargin = 50;

    
    ofDrawBitmapString("Press SPACEBAR to save your image. Find it in the data folder.", leftMargin, 30);
    
    cam.draw(leftMargin, topMargin);
    
    ofSetColor(255);
    fbo.draw(leftMargin + w, topMargin, w, h);
    

    
    gui.setPosition(10, topMargin);
    gui.draw();
    
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

    if(key == ' '){
        
        ofPixels imgPix;
        fbo.readToPixels(imgPix);
        
        string ts = ofGetTimestampString();
        
        ofSaveImage(imgPix, "saved_images/" + ts + ".png");
        
    }
    
    
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
