#include "AppCore.h"

void AppCore::setup(const int numOutChannels, const int numInChannels,
				    const int sampleRate, const int ticksPerBuffer) {

    ofSetFrameRate(60);
    ofSetVerticalSync(true);

    // double check where we are ...
	cout << ofFilePath::getCurrentWorkingDirectory() << endl;

    if(!pd.init(numOutChannels, numInChannels, sampleRate, ticksPerBuffer)) {
		OF_EXIT_APP(1);
	}

    pd.addReceiver(*this);
	pd.start();

	patch = pd.openPatch("pd/test.pd");
	cout << patch << endl;

	pd.sendBang("fromOF");

}
//--------------------------------------------------------------
void AppCore::update() {
    //if (bTouch)
    //{
        pd.startMessage();
        pd.addSymbol("x");
        pd.addFloat(x);
        pd.finishList("fromOF");

        pd.startMessage();
        pd.addSymbol("y");
        pd.addFloat(y);
        pd.finishList("fromOF");
    //}
}
//--------------------------------------------------------------
void AppCore::draw() {

}
//--------------------------------------------------------------
void AppCore::exit() {

}
//--------------------------------------------------------------
void AppCore::keyPressed(int key) {}
//--------------------------------------------------------------
void AppCore::audioReceived(float * input, int bufferSize, int nChannels) {
	pd.audioIn(input, bufferSize, nChannels);
}

//--------------------------------------------------------------
void AppCore::audioRequested(float * output, int bufferSize, int nChannels) {
	pd.audioOut(output, bufferSize, nChannels);
}
//--------------------------------------------------------------
void AppCore::print(const std::string& message) {
    cout << message << endl;
}
