#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	// initialize the accelerometer
	ofxAccelerometer.setup();
	ofxRegisterAccelEvents(this);
	font.loadFont("bin/data/verdana.ttf",24);
	headerFont.loadFont("bin/data/Neris-SemiBold.otf",16);
	bodyFont.loadFont("bin/data/NexaLight.otf",45);
	infoFont.loadFont("bin/data/NexaLight.otf",30);
	butFadFont.loadFont("bin/data/NexaLight.otf",25);


	settings.loadFile("bin/data/GUI/defaultColors.xml");

	backgroundColor = ofColor(settings.getValue("colors:background:r", 41),
	                              settings.getValue("colors:background:g", 42),
	                              settings.getValue("colors:background:b", 53),
	                              settings.getValue("colors:background:a", 255));

	keyColor = ofColor(settings.getValue("colors:key:r", 52),
	                       settings.getValue("colors:key:g", 175),
	                       settings.getValue("colors:key:b", 195),
	                       settings.getValue("colors:key:a", 255));

	lightBackgroundColor = ofColor(settings.getValue("colors:lightBackground:r", 51),
	                       settings.getValue("colors:lightBackground:g", 51),
	                       settings.getValue("colors:lightBackground:b", 51),
	                       settings.getValue("colors:lightBackground:a", 255));

	lightBlueColor = ofColor(settings.getValue("colors:bloque:r", 44),
		                       settings.getValue("colors:bloque:g", 151),
		                       settings.getValue("colors:bloque:b", 195),
		                       settings.getValue("colors:bloque:a", 206));
	buttons = 2;
	int sep = 20;
	for (int i=0; i < buttons; i++) {
				ofRectangle _rect;
				butRects.push_back(_rect);
				butRects.at(i).set(sep+i*(ofGetWidth()-(sep*2)-((buttons-1)*sep))/buttons+i*sep,
					bodyFont.stringHeight("SERVIDOR")+sep,
					(ofGetWidth()-(sep*2)-((buttons-1)*sep))/buttons,
					ofGetHeight()-sep*2-bodyFont.stringHeight("SERVIDOR"));
	}
	infoBF.resize(4);
	for(int i = 0; i < infoBF.size(); i++){
		infoBF[i] = "";
	}

}
//--------------------------------------------------------------
void ofApp::accelerationChanged(ofPoint &accelval){
	if(swAccelerometer)
		{
			//Envia datos del acelerometro
			ofxOscMessage m;
			m.setAddress("/accelerometer");
			m.addFloatArg(accelval.x);
			m.addFloatArg(accelval.y);
			sendTCPMessage(m,&client);
		}
}
//--------------------------------------------------------------
void ofApp::update(){
	if(client.isConnected())
	{
		char buffer[BUFFER_SIZE];
		if (client.receiveRawBytes(buffer,BUFFER_SIZE) > 0)
		{
			osc::ReceivedPacket p(buffer, BUFFER_SIZE);
			osc::ReceivedMessage m(p);
			ProcessMessage(m);

			ofxOscMessage mosc;

			if(hasWaitingMessages())
			{
				getNextMessage(&mosc);
				if (mosc.getAddress() == "/accelerometer")
				{
					swKeyboard = false;
					swGPS = false;
					swTouchscreen = false;
					swButtons = false;
					swFaders = false;
					if (mosc.getArgAsInt32(0) == 1){
						swAccelerometer = true;
						infox = mosc.getArgAsString(1);
						infoy = mosc.getArgAsString(2);
						infoText = mosc.getArgAsString(3);
						ofxAndroidToast("Enviando datos del acelerometro");
					}
					else {
						swAccelerometer = false;
						infoText = "";
						ofxAndroidToast("Se detuvo el env�o de datos del acelerometro");
					}
				}
				else if (mosc.getAddress() == "/keyboard")
				{
					swAccelerometer = false;
					swGPS = false;
					swTouchscreen = false;
					swButtons = false;
					swFaders = false;
					if (mosc.getArgAsInt32(0) == 1){
						swKeyboard = true;
						ofxAndroidToast("Ingrese texto");
					}
					else {
						swKeyboard = false;
						infoText = "";
						ofxAndroidToast("Se detuvo el ingreso de texto");
					}
				}
				else if (mosc.getAddress() == "/touchscreen" )
				{
					swAccelerometer = false;
					swGPS = false;
					swKeyboard = false;
					swButtons = false;
					swFaders = false;
					if (mosc.getArgAsInt32(0) == 1){
						swTouchscreen = true;
						infox = mosc.getArgAsString(1);
						infoy = mosc.getArgAsString(2);
						infoText = mosc.getArgAsString(3);
						ofxAndroidToast("Enviando datos de la pantalla");
					}
					else {
						swTouchscreen = false;
						infoText = "";
						ofxAndroidToast("Se detuvo el env�o de datos de la pantalla");
					}
				}
				else if (mosc.getAddress() == "/gps")
				{
					swGPS = !swGPS;
					swAccelerometer = false;
					swKeyboard = false;
					swTouchscreen = false;
					swButtons = false;
					swFaders = false;
					ofxAndroidToast("Enviando datos del GPS");
				}
				else if (mosc.getAddress() == "/buttons")
				{
					swAccelerometer = false;
					swKeyboard = false;
					swTouchscreen = false;
					swGPS = false;
					swFaders = false;
					if (mosc.getArgAsInt32(0) == 1){
						swButtons = true;
						ofxAndroidToast("Utilice los botones en pantalla");

						if(mosc.getArgAsInt32(1) == 0 || mosc.getArgAsInt32(1) > 4) {
							buttons = 1;
						}
						else {
							buttons = mosc.getArgAsInt32(1);
						}
						infoBF.at(0) = mosc.getArgAsString(2);
						infoBF.at(1) = mosc.getArgAsString(3);
						infoBF.at(2) = mosc.getArgAsString(4);
						infoBF.at(3) = mosc.getArgAsString(5);
						infoText = mosc.getArgAsString(6);
						butRects.clear();
						for (int i=0; i < buttons; i++) {
							ofRectangle _rect;
							butRects.push_back(_rect);

						}

						updateButtonsRect();
					}
					else {
						swButtons = false;
						infoText = "";
						ofxAndroidToast("Se detuvo el uso de botones en pantalla");
					}
				}
				else if (mosc.getAddress() == "/faders")
				{
					swButtons = false;
					swAccelerometer = false;
					swKeyboard = false;
					swTouchscreen = false;
					swGPS = false;
					if (mosc.getArgAsInt32(0) == 1){
						swFaders = true;
						ofxAndroidToast("Utilice los faders en pantalla");
						if(mosc.getArgAsInt32(1) == 0 || mosc.getArgAsInt32(1) > 4) {
							faders = 1;
						}
						else {
							faders = mosc.getArgAsInt32(1);
						}
						infoBF.at(0) = mosc.getArgAsString(2);
						infoBF.at(1) = mosc.getArgAsString(3);
						infoBF.at(2) = mosc.getArgAsString(4);
						infoBF.at(3) = mosc.getArgAsString(5);
						infoText = mosc.getArgAsString(6);

						faderRect.clear();
						fillFaderRect.clear();

						for (int i=0; i < faders; i++) {
							ofRectangle _rect;
							faderRect.push_back(_rect);
							fillFaderRect.push_back(_rect);
						}

						updateFadersRect();
					}
					else {
						swFaders = false;
						infoText = "";
						ofxAndroidToast("Se detuvo el uso de faders en pantalla");
					}
				}
				else if (mosc.getAddress() == "/toast")
				{
					//Muestra el texto recibido como un toast de Android
					string str = mosc.getArgAsString(0);
					ofxAndroidToast(str);
				}
				else if (mosc.getAddress() == "/allsensorsoff")
				{
					if (mosc.getArgAsInt32(0) == 1) {
						infoText = "";
						swButtons = false;
						swAccelerometer = false;
						swKeyboard = false;
						swTouchscreen = false;
						swGPS = false;
						swFaders = false;
					}
				}
				else if (mosc.getAddress() == "/info")
				{
					infoText = mosc.getArgAsString(0);
				}
				else if (mosc.getAddress() == "/infox")
				{
					infox = mosc.getArgAsString(0);
				}
				else if (mosc.getAddress() == "/infoy")
				{
					infoy = mosc.getArgAsString(0);
				}
				else if (mosc.getAddress() == "/info1")
				{
					infoBF.at(0) = mosc.getArgAsString(0);
				}
				else if (mosc.getAddress() == "/info2")
				{
					infoBF.at(1) = mosc.getArgAsString(0);
				}
				else if (mosc.getAddress() == "/info3")
				{
					infoBF.at(2) = mosc.getArgAsString(0);
				}
				else if (mosc.getAddress() == "/info4")
				{
					infoBF.at(3)= mosc.getArgAsString(0);
				}
			}
		}
	}

	if (!client.isConnected() && (ofGetFrameNum() % 60) == 0) {
		infoText = "";
		swButtons = false;
		swAccelerometer = false;
		swKeyboard = false;
		swTouchscreen = false;
		swGPS = false;
		swFaders = false;
		bNameSent = false;
		client.setup("192.168.0.10",44500);
		if (client.isConnected())
		{

			JNIEnv * env = ofGetJNIEnv();
			ofxAndroidToast("Conectado");

			//Llama a m�todos en la clase de Java
			jobject activity = ofGetOFActivityObject();
			jclass activityClass = env->FindClass("cc/openframeworks/Howell/OFActivity");
			jmethodID method = env->GetMethodID(activityClass,"showSendNameDialog","()V");
			env->CallVoidMethod(activity,method);
		}

	}

	if(swKeyboard)
	{
		 //Envia datos del teclado
	}

}

//--------------------------------------------------------------
void ofApp::draw(){
	ofBackground(backgroundColor);
	ofSetColor(lightBackgroundColor);
	ofFill();
	ofRect(0,0,ofGetWidth(),ofGetHeight()*0.12);

	ofSetColor(lightBlueColor);
	bodyFont.drawString("SERVIDOR",
			18,
			7+bodyFont.stringHeight("SERVIDOR"));
	ofLine(bodyFont.stringWidth("SERVIDOR")+20+20,
			6,
			bodyFont.stringWidth("SERVIDOR")+20+20,
			ofGetHeight()*0.12-6);
	ofNoFill();
	ofRect(bodyFont.stringWidth("SERVIDOR")+20+20+15,
			7,
			ofGetHeight()*0.12-14,
			ofGetHeight()*0.12-14);
	ofFill();
	ofRect(bodyFont.stringWidth("SERVIDOR")+20+20+15+(ofGetHeight()*0.12-14)/2-3,
			12,
			6,
			ofGetHeight()*0.12-14-21);
	ofRect(bodyFont.stringWidth("SERVIDOR")+20+20+15+(ofGetHeight()*0.12-14)/2-3,
			12+ofGetHeight()*0.12-14-21+5,
			6,
			6);

	/*
	ofSetLineWidth(10);
	ofBackground(0,60,0);
	ofSetColor(255,255,0);
	font.drawString(accelmessages[0],10,font.stringHeight(accelmessages[0])+20);
	ofSetColor(255,0,255);
	font.drawString(accelmessages[1],10,(font.stringHeight(accelmessages[0])+20)*2);
	ofSetColor(0,255,255);
	font.drawString(accelmessages[2],10,(font.stringHeight(accelmessages[0])+20)*3);

	ofPushMatrix();
	ofTranslate(ofGetWidth()/2, ofGetHeight()/2);

	ofSetColor(255,255,0);
	ofLine(0,0,normAccel.x*ofGetWidth()/2,0);
	ofSetColor(255,0,255);
	ofLine(0,0,0,-normAccel.y*ofGetHeight()/2);
	// we don't draw z as the perspective might be confusing
	// but it's approximately one when the device is still and parallel
	// to the ground
	ofPopMatrix();
	*/

	/*---TOUCHSCREEN / ACELEROMETRO
	infoText = "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Donec a diam lectus.";

	ofSetColor(keyColor);
	infoFont.drawString(infoText, (ofGetWindowSize().x-bodyFont.getStringBoundingBox(infoText,0,0).width)/2, ofGetHeight()/2);

	bodyFont.drawString("Valor X", ofGetWidth()/2-bodyFont.stringWidth("Valor X")/2, ofGetHeight()-10);
	bodyFont.drawString("SERVIDOR", ofGetWidth()/2-bodyFont.stringWidth("SERVIDOR")/2,bodyFont.stringHeight("SERVIDOR")+10);

	ofPushMatrix();
	ofTranslate(10,ofGetHeight()/2-bodyFont.stringWidth("Valor Y")/2);
	ofRotate(90);
	bodyFont.drawString("Valor Y", 0, 0);
	ofPopMatrix();
	*/

	if(swButtons)
	{
		//Dibuja botones y envia cual fue presionado al servidor
		ofSetColor(keyColor);
		infoFont.drawString(infoText,
						440,
						infoFont.stringHeight("INFO")+20);

		//Dibuja todos los rect que conforman los botones
		for (int i=0; i < butRects.size(); i++) {
			ofSetColor(keyColor);
			ofRect(butRects.at(i));
			ofSetColor(255);
			butFadFont.drawString(infoBF.at(i),butRects.at(i).x, butRects.at(i).getBottom());
		}

	}

	if(swTouchscreen)
	{
		ofSetColor(keyColor);
		infoFont.drawString(infoText,
				440,
				infoFont.stringHeight("INFO")+20);

		bodyFont.drawString(infox, ofGetWidth()/2-bodyFont.stringWidth(infox)/2, ofGetHeight()-10);

		ofPushMatrix();
		ofTranslate(10,ofGetHeight()/2-bodyFont.stringWidth(infoy)/2);
		ofRotate(90);
		bodyFont.drawString(infoy, 0, 0);
		ofPopMatrix();
	}

	if(swAccelerometer)
	{
		ofSetColor(keyColor);
		infoFont.drawString(infoText,
				440,
				infoFont.stringHeight("INFO")+20);

		bodyFont.drawString(infox, ofGetWidth()/2-bodyFont.stringWidth(infox)/2, ofGetHeight()-10);

		ofPushMatrix();
		ofTranslate(10,ofGetHeight()/2-bodyFont.stringWidth(infoy)/2);
		ofRotate(90);
		bodyFont.drawString(infoy, 0, 0);
		ofPopMatrix();
	}

	if(swFaders)
	{
		ofSetColor(keyColor);
		infoFont.drawString(infoText,
						440,
						infoFont.stringHeight("INFO")+20);

		//Dibuja todos los rect que conforman los faders
		ofFill();
		for (int i=0; i < faderRect.size(); i++) {
			ofRect(faderRect.at(i));
		}

		ofSetColor(lightBackgroundColor);
		for (int i=0; i < fillFaderRect.size(); i++) {
			ofRect(fillFaderRect.at(i));
		}

		ofSetColor(255);
		for (int i=0; i < faderRect.size(); i++) {
			butFadFont.drawString(infoBF.at(i),faderRect.at(i).x, faderRect.at(i).getBottom());
		}
	}

	if(swKeyboard)
	{
		ofSetColor(keyColor);
		infoFont.drawString(infoText,
						440,
						infoFont.stringHeight("INFO")+20);

	}
}

//--------------------------------------------------------------
void ofApp::keyPressed  (int key){ 
	
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){ 
	
}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::touchDown(int x, int y, int id) {

	if((ofGetFrameNum()-prevFrame)>60){
		prevFrame = ofGetFrameNum();
		if (swButtons) {
			for(int i=0; i < butRects.size(); i++){
				if( butRects.at(i).inside(x,y) ) {
					ofxOscMessage m;
					m.setAddress("/buttons");
					m.addIntArg(i+1);
					sendTCPMessage(m,&client);
				}
			}
		}
	}

	if (swFaders) {
		for(int i=0; i < faderRect.size(); i++){
			if( faderRect.at(i).inside(x,y) ) {

				fillFaderRect.at(i).height = y-faderRect.at(i).y;

				ofxOscMessage m;
				m.setAddress("/faders");
				m.addIntArg(i+1);
				m.addFloatArg(fillFaderRect.at(i).height/faderRect.at(i).height);
				sendTCPMessage(m,&client);
			}
		}
	}
}
//--------------------------------------------------------------
void ofApp::touchMoved(int x, int y, int id) {
	if(swTouchscreen)
	{
		//Envia datos de la pantalla
		ofxOscMessage m;
		float normx = (float)x / (float)ofGetWidth();
		float normy = (float)y / (float)ofGetHeight();
		m.setAddress("/touchscreen");
		m.addFloatArg(normx);
		m.addFloatArg(normy);
		sendTCPMessage(m,&client);

		ofSetColor(255,0,0);
		ofFill();
		ofCircle(x,y,12);

	}

	if (swFaders) {
		for(int i=0; i < faderRect.size(); i++){
			if( faderRect.at(i).inside(x,y) ) {

				fillFaderRect.at(i).height = y-faderRect.at(i).y;

				ofxOscMessage m;
				m.setAddress("/faders");
				m.addIntArg(i+1);
				m.addFloatArg(fillFaderRect.at(i).height/faderRect.at(i).height);
				sendTCPMessage(m,&client);
			}
		}
	}
}
//--------------------------------------------------------------
void ofApp::touchUp(int x, int y, int id){

}

//--------------------------------------------------------------
void ofApp::touchDoubleTap(int x, int y, int id){

}

//--------------------------------------------------------------
void ofApp::touchCancelled(int x, int y, int id){

}

//--------------------------------------------------------------
void ofApp::swipe(ofxAndroidSwipeDir swipeDir, int id){

}

//--------------------------------------------------------------
void ofApp::pause(){

}

//--------------------------------------------------------------
void ofApp::stop(){

}

//--------------------------------------------------------------
void ofApp::resume(){

}

//--------------------------------------------------------------
void ofApp::reloadTextures(){

}

//--------------------------------------------------------------
bool ofApp::backPressed(){
	return false;
}

//--------------------------------------------------------------
void ofApp::okPressed(){

}

//--------------------------------------------------------------
void ofApp::cancelPressed(){

}
//--------------------------------------------------------------
void ofApp::conectar(){
	//Lo siguiente es el codigo que corresponde a la tesis, comentado para debug

    if(!client.isConnected())
    {
        bNameSent = false;
        client.setup(serverIP,44500);
        if (client.isConnected())
        {
        	//Llama a m�todos en la clase de Java
			jobject activity = ofGetOFActivityObject();
			jclass activityClass = ofGetJNIEnv()->FindClass("cc/openframeworks/Howell/OFActivity");
			jmethodID method = ofGetJNIEnv()->GetMethodID(activityClass,"showSendNameDialog","()V");
			ofGetJNIEnv()->CallVoidMethod(activity,method);
        }
    }
    else
    {
		if(!bNameSent)
		{
			//grabMutex();		//Probando

			if(clientName.size() > 0)
			{
				ofxOscMessage m;
				m.setAddress("/name");
				m.addStringArg(clientName);
				if(sendTCPMessage(m,&client))
				{
					bNameSent = true;
				}
			//releaseMutex(); 	//Probando
			}
			else
			{
				//Si el nombre es muy corto llama de vuelta al di�logo para ingresar nuevamente el nombre
				jobject activity = ofGetOFActivityObject();
				jclass activityClass = ofGetJNIEnv()->FindClass("cc/openframeworks/Howell/OFActivity");
				jmethodID method = ofGetJNIEnv()->GetMethodID(activityClass,"showSendNameDialog","()V");
				ofGetJNIEnv()->CallVoidMethod(activity,method);
				ofxAndroidToast("Nombre de cliente muy corto");
			}
		}

	}


    //Lo siguiente es para etapa de debug, eliminar o comentar para tesis
	/*
    if(!client.isConnected())
        {
            bNameSent = false;
            client.setup("192.168.0.10",44500);
            */
            /*
            if (client.isConnected())
            {
            	//Llama a m�todos en la clase de Java
    			jobject activity = ofGetOFActivityObject();
    			jclass activityClass = ofGetJNIEnv()->FindClass("cc/openframeworks/androidClient/OFActivity");
    			jmethodID method = ofGetJNIEnv()->GetMethodID(activityClass,"showSendNameDialog","()V");
    			ofGetJNIEnv()->CallVoidMethod(activity,method);
            }
            */
	/*
            if(!bNameSent)
			{
				//grabMutex();		//Probando
					ofxOscMessage m;
					m.setAddress("/name");
					m.addStringArg("ClientTest");
					if(sendTCPMessage(m,&client))
					{
						bNameSent = true;
					}
				//releaseMutex(); 	//Probando

			}
        }
        else
        {
    		if(!bNameSent)
    		{
    			//grabMutex();		//Probando


    				ofxOscMessage m;
    				m.setAddress("/name");
    				m.addStringArg("ClientTest");
    				if(sendTCPMessage(m,&client))
    				{
    					bNameSent = true;
    				}
    			//releaseMutex(); 	//Probando

    		}

    	}
    */
}
//--------------------------------------------------------------
void ofApp::ProcessMessage( const osc::ReceivedMessage &m)
{
	// convert the message to an ofxOscMessage
	ofxOscMessage* ofMessage = new ofxOscMessage();

	// set the address
	ofMessage->setAddress( m.AddressPattern() );


	// transfer the arguments
	for ( osc::ReceivedMessage::const_iterator arg = m.ArgumentsBegin();
		  arg != m.ArgumentsEnd();
		  ++arg )
	{
		if ( arg->IsInt32() )
			ofMessage->addIntArg( arg->AsInt32Unchecked() );
		else if ( arg->IsInt64() )
			ofMessage->addInt64Arg( arg->AsInt64Unchecked() );
		else if ( arg->IsFloat() )
			ofMessage->addFloatArg( arg->AsFloatUnchecked() );
		else if ( arg->IsString() )
			ofMessage->addStringArg( arg->AsStringUnchecked() );
		else
		{
			cout << "message argument is not int, float, or string" << endl;
		}
	}

	// now add to the queue

	// at this point we are running inside the thread created by startThread,
	// so anyone who calls hasWaitingMessages() or getNextMessage() is coming
	// from a different thread

	// so we have to practise shared memory management

	// grab a lock on the queue
	grabMutex();

	// add incoming message on to the queue
	messages.push_back( ofMessage );

	// release the lock
	releaseMutex();
}
//--------------------------------------------------------------
void ofApp::grabMutex()
{
#ifdef TARGET_WIN32
	WaitForSingleObject( mutex, INFINITE );
#else
	pthread_mutex_lock( &mutex );
#endif
}
//--------------------------------------------------------------
void ofApp::releaseMutex()
{
#ifdef TARGET_WIN32
	ReleaseMutex( mutex );
#else
	pthread_mutex_unlock( &mutex );
#endif
}
//--------------------------------------------------------------
bool ofApp::hasWaitingMessages()
{
	// grab a lock on the queue
	grabMutex();

	// check the length of the queue
	int queue_length = (int)messages.size();

	// release the lock
	releaseMutex();

	// return whether we have any messages
	return queue_length > 0;
}
//--------------------------------------------------------------
bool ofApp::getNextMessage( ofxOscMessage* message )
{
	// grab a lock on the queue
	grabMutex();

	// check if there are any to be got
	if ( messages.size() == 0 )
	{
		// no: release the mutex
		releaseMutex();
		return false;
	}

	// copy the message from the queue to message
	ofxOscMessage* src_message = messages.front();
	message->copy( *src_message );

	// now delete the src message
	delete src_message;
	// and remove it from the queue
	messages.pop_front();

	// release the lock on the queue
	releaseMutex();

	// return success
	return true;
}
//--------------------------------------------------------------
bool ofApp::sendTCPMessage( ofxOscMessage& message, ofxTCPClient * client )
{
	static const int OUTPUT_BUFFER_SIZE = 16384;
	char buffer[OUTPUT_BUFFER_SIZE];
    osc::OutboundPacketStream p( buffer, OUTPUT_BUFFER_SIZE );

	// serialise the message
	//p << osc::BeginBundleImmediate;
	appendMessage( message, p );
	//p << osc::EndBundle;

    client->sendRawBytes( p.Data(), p.Size() );
	return client->sendRawBytes( p.Data(), p.Size() );
}
//--------------------------------------------------------------
void ofApp::appendMessage( ofxOscMessage& message, osc::OutboundPacketStream& p )
{
    p << osc::BeginMessage( message.getAddress().c_str() );
	for ( int i=0; i< message.getNumArgs(); ++i )
	{
		if ( message.getArgType(i) == OFXOSC_TYPE_INT32 )
			p << message.getArgAsInt32( i );
		else if ( message.getArgType(i) == OFXOSC_TYPE_INT64 )
			p << (osc::int64)message.getArgAsInt64( i );
		else if ( message.getArgType( i ) == OFXOSC_TYPE_FLOAT )
			p << message.getArgAsFloat( i );
		else if ( message.getArgType( i ) == OFXOSC_TYPE_STRING )
			p << message.getArgAsString( i ).c_str();
		else
		{
			ofLogError() << "bad argument type" + ofToString(message.getArgType( i ));
			assert( false );
		}
	}
	p << osc::EndMessage;
}
//--------------------------------------------------------------
void ofApp::updateButtonsRect() {
	int sep = 20;
	if(butRects.size() == 4) {
		for (int i=0; i < butRects.size(); i++) {
			if(i < 2) {
				butRects.at(i).set(sep+i*(ofGetWidth()-(sep*3))/2+i*sep,
					bodyFont.stringHeight("SERVIDOR")+sep,
					(ofGetWidth()-(sep*3))/2,
					(ofGetHeight()-sep*3-bodyFont.stringHeight("SERVIDOR"))/2);
			} else {
				butRects.at(i).set(sep+(i-2)*(ofGetWidth()-(sep*3))/2+(i-2)*sep,
					bodyFont.stringHeight("SERVIDOR")+sep*2+(ofGetHeight()-sep*3-bodyFont.stringHeight("SERVIDOR"))/2,
					(ofGetWidth()-(sep*3))/2,
					(ofGetHeight()-sep*3-bodyFont.stringHeight("SERVIDOR"))/2);
			}
		}
	} else {
		for (int i=0; i < butRects.size(); i++) {
			butRects.at(i).set(sep+i*(ofGetWidth()-(sep*2)-((buttons-1)*sep))/buttons+i*sep,
				bodyFont.stringHeight("SERVIDOR")+sep,
				(ofGetWidth()-(sep*2)-((buttons-1)*sep))/buttons,
				ofGetHeight()-sep*2-bodyFont.stringHeight("SERVIDOR"));
		}
	}
}
//-------
void ofApp::updateFadersRect(){

	int sep = 20;

	for (int i=0; i < faderRect.size(); i++) {
		faderRect.at(i).set(sep+i*(ofGetWidth()-(sep*2)-((faders-1)*sep))/faders+i*sep,
			bodyFont.stringHeight("SERVIDOR")+sep,
			(ofGetWidth()-(sep*2)-((faders-1)*sep))/faders,
			ofGetHeight()-sep*2-bodyFont.stringHeight("SERVIDOR"));
	}

	for (int i=0; i < fillFaderRect.size(); i++) {
		fillFaderRect.at(i).set(sep+i*(ofGetWidth()-(sep*2)-((faders-1)*sep))/faders+i*sep,
			bodyFont.stringHeight("SERVIDOR")+sep,
			(ofGetWidth()-(sep*2)-((faders-1)*sep))/faders,
			ofGetHeight()-sep*2-bodyFont.stringHeight("SERVIDOR"));
	}
}
