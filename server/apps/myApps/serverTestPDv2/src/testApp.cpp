#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
    int ticksPerBuffer = 64;	// 8 * 64 = buffer len of 512
    //ofSoundStream dastream;
    //dastream.setDeviceID(2);
    //dastream.setup(2, 1, this, 48000, ofxPd::blockSize()*ticksPerBuffer, 3)
   // dastream.setup(this,2,1,48000, ofxPd::blockSize()*ticksPerBuffer, 3);
     //Tomado de ofxPd/examplePitchShifter
    // the number if libpd ticks per buffer,
	// used to compute the audio buffer len: tpb * blocksize (always 64)

	// setup OF sound stream
	ofSoundStreamSetup(2, 0, this, 48000, ofxPd::blockSize()*ticksPerBuffer, 3);

	// setup the app core
	core.setup(2, 0, 48000, ticksPerBuffer);
}

//--------------------------------------------------------------
void testApp::update(){
    core.update();  //Actualiza el objeto AppCore
}

//--------------------------------------------------------------
void testApp::draw(){
    core.draw();
}
//--------------------------------------------------------------
void testApp::exit(){
    core.exit();
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
    core.keyPressed(key);
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){
    core.mousePressed(x,y,button);
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){

}

//--------------------------------------------------------------
void testApp::audioReceived(float * input, int bufferSize, int nChannels) {
	core.audioReceived(input, bufferSize, nChannels);
}

//--------------------------------------------------------------
void testApp::audioRequested(float * output, int bufferSize, int nChannels) {
	core.audioRequested(output, bufferSize, nChannels);
}
