#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
    ofSetFrameRate(60);
    ofSetVerticalSync(true);

    /*---------------TIMELINE-----------------*/
    ofBackground(0,64,86);
    transportBGColor = ofColor(44,133,141);

    timeline.setup();
    mainOffset = ofVec2f(100,40); //Variable mainOffset para almacenar el vector de corrimiento de la ventana timeline

    timeline.setOffset(mainOffset); //Corre el timeline en el eje x y en el eje y

    //Se cargan las fuentes para el texto de la interfaz grafica
    logoFont.loadFont("GUI/NewMedia Fett.ttf",32);
    transportFont.loadFont("GUI/NewMedia Fett.ttf",18);
    timeline.setSpacebarTogglePlay(true);   //La barra espaciadora funciona como play
    timeline.setDurationInSeconds(120);

    //Se agregan pistas
    timeline.addBloques("PureData-Bloques");
    timeline.addBloques("PureData-Bloques2");
    ofAddListener(timeline.events().bloqued, this, &testApp::receivedBloque);
    //cout << (timeline.getCurrentPage()->getTracks()[0])->getName() << endl; //ANDA

    //Se utiliza una lista aparte de la cantidad de tracks para no saturar de llamadas a la función timeline.getCurrentPage()->getTracks()
    if (timeline.getCurrentPage()->getTracks().size() > 0)
    {
       for (int i = 0; i < timeline.getCurrentPage()->getTracks().size(); i++) {
        pista _pista;
        _pista.trackName = timeline.getCurrentPage()->getTracks()[i]->getName();
        _pista.clientsAssigned = 0;
        pistasList.push_back(_pista);
        }
    }


    /*---------------SERVER--------------------*/
    server.setup(44500);
    ofSetWindowTitle("SERVIDOR");
    mutex = CreateMutexA( NULL, FALSE, NULL );
    font.loadFont("NewMediaFett.ttf",32);
    listFont.loadFont("NewMediaFett.ttf",12);

    // double check where we are ...
	cout << ofFilePath::getCurrentWorkingDirectory() << endl;
    int ticksPerBuffer = 64;	// 8 * 64 = buffer len of 512

	// setup OF sound stream
	ofSoundStreamSetup(2, 0, this, 48000, ofxPd::blockSize()*ticksPerBuffer, 3);


    if(!pd.init(2, 0, 48000, ticksPerBuffer)) {
		OF_EXIT_APP(1);
	}

    pd.addReceiver(*this);
	pd.start();

    patch = pd.openPatch("/pd/server.pd");

}

//--------------------------------------------------------------
void testApp::update(){
    //Se comprueba que el servidor este funcionando antes de comenzar con las demas operaciones
    //cout << server.getLastID() << endl;
    if(server.isConnected())
    {
        char buffer[BUFFER_SIZE];   //Buffer para almacenar los bytes de los mensajes entrantes
        for (int i = 0; i < server.getLastID();i++) //Se hace una pasada por todos los clientes
        {
            if(server.isClientConnected(i)) {   //Se comprueba que el cliente este efectivamente conectado
                if (server.receiveRawBytes(i,buffer,BUFFER_SIZE) > 0) //Si se recibio algo
                {
                    cout << "Mensaje recibido" << endl;

                    osc::ReceivedPacket p(buffer, BUFFER_SIZE);
                    osc::ReceivedMessage m(p);
                    ProcessMessage(m);
                    ofxOscMessage mosc;

                    if(hasWaitingMessages())
                    {
                        getNextMessage(&mosc);
                        if (mosc.getAddress() == "/touchscreen")
                        {
                            for(int j = 0; j < clientList.size();j++)
                            {
                                if (clientList[j].sId == i)
                                {

                                }
                            }
                            cout << "Touchscreen X: " << mosc.getArgAsFloat(0) << " Y: " << mosc.getArgAsFloat(1) << endl;
                        }
                        else if (mosc.getAddress() == "/name")
                        {
                            bool idAlreadyConnected = false;
                            for(int j = 0; j < clientList.size(); j++) {
                                if(clientList[j].sId == i) {
                                    idAlreadyConnected = true;
                                }
                                else{
                                    idAlreadyConnected = false;
                                }
                            }
                            if(!idAlreadyConnected)
                            {
                              cliente tempC;
                                tempC.sIp = server.getClientIP(i);
                                tempC.sName = mosc.getArgAsString(0);
                                tempC.sId = i;

                                //Chequea cual de las pistas tiene menos usuarios conectados y le asigna uno
                                if (pistasList.size() > 0)
                                {
                                    vector<int> cantClientes;
                                    for (int k = 0; k < pistasList.size(); k++) {
                                        cantClientes.push_back(pistasList[k].clientsAssigned);
                                    }
                                    for (int k = 0; k < pistasList.size(); k++) {

                                        if (pistasList[k].clientsAssigned == *min_element(cantClientes.begin(),cantClientes.end()))
                                        {
                                        tempC.sTrackName = pistasList[k].trackName;
                                        pistasList[k].clientsAssigned += 1;
                                        break;
                                        }
                                    }
                                }

                                //Agrega el nombre del cliente al header de la pista asignada
                                ofxTLBloques * bloquePointer;
                                bloquePointer = dynamic_cast<ofxTLBloques*>(timeline.getTrack(tempC.sTrackName));
                                bloquePointer->addToClientHeader(tempC.sName);
                                clientList.push_back(tempC); //Agrega un cliente a la lista

                                cout << "IP: " << server.getClientIP(i) << " ";
                                cout << "Name:" << mosc.getArgAsString(0) << " ";
                                cout << "ClientID: " << i << endl;

                                ofxOscMessage m;
                                m.setAddress("/toast");
                                m.addStringArg("Conectado al servidor.");
                                if(sendTCPMessage(m, clientList[i].sId, &server))
                                {
                                    cout << "asdas8" << endl;
                                    cout << "Confirmacion enviada" << endl;
                                }
                            }

                        }
                        else if (mosc.getAddress() == "/accelerometer")
                        {
                            cout << "Accel.x: " << mosc.getArgAsFloat(0);
                            cout << " Accel.y: " << mosc.getArgAsFloat(1) << endl;
                        }
                        else
                        {
                            cout << "Direccion OSC no valida" << endl;
                        }
                    }
                }
                /*--Usado en pruebas
                if(timeline.bloqueStateChanged("PureData-Switch"))
                {
                    ofxOscMessage m;
                    m.setAddress("/touchscreen");
                    if(sendTCPMessage(m, clientList[i].sId, &server))
                    {
                        cout << "TOUCHSCREEN al cliente " << clientList[i].sName << endl;
                    }
                }
                */
            }
            else {
                cout << "cliente : " << i << endl;
            }
        }

        if(!clientList.empty())
        {
            for (unsigned int i = 0; i < clientList.size(); i++)
            {
                if(!server.isClientConnected(clientList[i].sId))
                {
                    //pd.closePatch(clientList[i].patch);
                    cout << "Cliente nr " << clientList[i].sId << " de nombre " << clientList[i].sName << " se ha desconectado." << endl;
                    ofxTLBloques * bloquePointer;
                    bloquePointer = dynamic_cast<ofxTLBloques*>(timeline.getTrack(clientList[i].sTrackName));
                    bloquePointer->removeFromClientHeader(clientList[i].sName);
                    for (int j=0; j < pistasList.size(); j++) {
                        if ( pistasList[j].trackName == clientList[i].sTrackName ) {
                            pistasList[j].clientsAssigned -= 1;
                            break;
                        }
                    }

                    clientList.erase(clientList.begin()+i);
                }
            }
        }
    }

}

//--------------------------------------------------------------
void testApp::draw(){

    /*--- INICIO BARRA DE TRANSPORTE Y LOGO ------------------------*/
    //Dibujar barra de herramientas y logo superior
    ofSetColor(44,133,141,10);
    ofRectangle logo = logoFont.getStringBoundingBox("APLICACION SERVIDOR",10,36);
    ofRect(logo);
    ofSetColor(255,255,203);
    logoFont.drawString("APLICACION SERVIDOR",10,36);
    logo.x += 80; //Separacion entre logo y transporte

    //Barra de transporte, cambia el color si el mouse esta sobre el texto
    if(hoverPlayButton || timeline.getIsPlaying()){
        transportTextColor = ofColor(0,255,0);
    }
    else {
        transportTextColor = ofColor(timeline.getColors().textColor);
    }
    playButton = dibujarBotonTransporte("PLAY",logo,playButton,transportTextColor);

    if(hoverPauseButton){
        transportTextColor = ofColor(220,220,0);
    }
    else {
        transportTextColor = ofColor(timeline.getColors().textColor);
    }
    pauseButton = dibujarBotonTransporte("PAUSE",playButton,pauseButton,transportTextColor);

    if(hoverStopButton || !timeline.getIsPlaying()){
        transportTextColor = ofColor(220,0,0);
    }
    else {
        transportTextColor = ofColor(timeline.getColors().textColor);
    }
    stopButton = dibujarBotonTransporte("STOP",pauseButton,stopButton,transportTextColor);

    if(hoverLoopButton || timeline.getLoopType()== OF_LOOP_NORMAL){
        transportTextColor = ofColor(0,220,150);
    }
    else {
        transportTextColor = ofColor(timeline.getColors().textColor);
    }
    loopButton = dibujarBotonTransporte("LOOP",stopButton,loopButton,transportTextColor);
    /*--- FIN BARRA DE TRANSPORTE Y LOGO ------------------------*/

    /*--LÍNEA DE TIEMPO--*/
    timeline.draw();
    /*--FIN LÍNEA DE TIEMPO--*/


    /*--INICIO MONITOR DE CLIENTES--*/
    /*
    if (clientList.size() > 0)
    {
        //cout << clientList.size() << endl;
        for (unsigned int i = 0; i < clientList.size(); i++)
        {
            ofPushStyle();
            ofSetColor(244,93,76);
            string tempS = clientList[i].sIp + " " + clientList[i].sName + " " + ofToString(clientList[i].sId);
            listFont.drawString(tempS,
                                20,
                                font.getLineHeight()+(listFont.getLineHeight()*(i+1)));

            //Botonera por cada cliente
            clientList[i].rcTouchSw.x = 20+listFont.stringWidth(tempS)+4;
            clientList[i].rcTouchSw.y = font.getLineHeight()+(listFont.getLineHeight()*(i))+3;

            clientList[i].rcButtonSw.x = 20+listFont.stringWidth(tempS)+clientList[i].rcTouchSw.width+8;
            clientList[i].rcButtonSw.y = font.getLineHeight()+(listFont.getLineHeight()*(i))+3;

            clientList[i].rcFaderSw.x = 20+listFont.stringWidth(tempS)+clientList[i].rcTouchSw.width+clientList[i].rcButtonSw.width+12;
            clientList[i].rcFaderSw.y = font.getLineHeight()+(listFont.getLineHeight()*(i))+3;

            clientList[i].rcKeyboardSw.x = 20+listFont.stringWidth(tempS)+clientList[i].rcTouchSw.width+clientList[i].rcButtonSw.width+clientList[i].rcFaderSw.width+16;
            clientList[i].rcKeyboardSw.y = font.getLineHeight()+(listFont.getLineHeight()*(i))+3;

            clientList[i].rcAccelSw.x = 20+listFont.stringWidth(tempS)+clientList[i].rcTouchSw.width+clientList[i].rcButtonSw.width+clientList[i].rcFaderSw.width+clientList[i].rcKeyboardSw.width+20;
            clientList[i].rcAccelSw.y = font.getLineHeight()+(listFont.getLineHeight()*(i))+3;

            ofSetColor(250,202,102);
            ofRect(clientList[i].rcTouchSw);
            ofRect(clientList[i].rcButtonSw);
            ofRect(clientList[i].rcFaderSw);
            ofRect(clientList[i].rcKeyboardSw);
            ofRect(clientList[i].rcAccelSw);

            ofSetColor(247,165,65);
            listFont.drawString("TOUCH",clientList[i].rcTouchSw.x,font.getLineHeight()+(listFont.getLineHeight()*(i+1)));
            listFont.drawString("BTN",clientList[i].rcButtonSw.x,font.getLineHeight()+(listFont.getLineHeight()*(i+1)));
            listFont.drawString("FADER",clientList[i].rcFaderSw.x,font.getLineHeight()+(listFont.getLineHeight()*(i+1)));
            listFont.drawString("KEY",clientList[i].rcKeyboardSw.x,font.getLineHeight()+(listFont.getLineHeight()*(i+1)));
            listFont.drawString("ACCEL",clientList[i].rcAccelSw.x,font.getLineHeight()+(listFont.getLineHeight()*(i+1)));
            ofPopStyle();
        }
    }
    */
    /*--FIN MONITOR DE CLIENTES--*/
}
//-------------------------------------------------------------
void testApp::exit(){
    server.close();
}
//--------------------------------------------------------------
void testApp::receivedBloque(ofxTLBloqueEventArgs& bloque){
    if(clientList.size() > 0)
    {
       for(int i = 0; i < clientList.size(); i++) {
            if(clientList[i].sTrackName == bloque.track->getName()){
                ofxOscMessage m;
                m.setAddress("/touchscreen");
                if(bloque.on){
                    m.addIntArg(1);
                }
                else {
                    m.addIntArg(0);
                }
                if(sendTCPMessage(m, clientList[i].sId, &server))
                {
                    cout << "TOUCHSCREEN al cliente " << clientList[i].sName << endl;
                }
            }
        }
    }
}

ofRectangle testApp::dibujarBotonTransporte(string name, ofRectangle anterior, ofRectangle actual, ofColor colorTexto) {
    ofSetColor(transportBGColor);
    actual = transportFont.getStringBoundingBox(name,anterior.getX() + anterior.getWidth()+20, mainOffset.y/2 + transportFont.stringHeight(name)/2);
    ofRect(actual);

    ofSetColor(colorTexto);
    transportFont.drawString(name,actual.getX(), mainOffset.y/2 + transportFont.stringHeight(name)/2);

    return actual;
}
//--------------------------------------------------------------
void testApp::keyPressed(int key){

}
//--------------------------------------------------------------
void testApp::keyReleased(int key){
}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){
    //Comprobar que el mouse este sobre los iconos de la barra de transporte
    hoverPlayButton = playButton.inside(x,y);
    hoverPauseButton = pauseButton.inside(x,y);
    hoverStopButton = stopButton.inside(x,y);
    hoverLoopButton = loopButton.inside(x,y);
}
//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){

}
//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){
    if(hoverPlayButton) {
        timeline.play();
    }
    if(hoverPauseButton) {
        timeline.stop();
    }
    if(hoverStopButton) {
        timeline.stop();
        timeline.setPercentComplete(0);
    }
    if(hoverLoopButton) {
            if(timeline.getLoopType() == OF_LOOP_NONE) {
                timeline.setLoopType(OF_LOOP_NORMAL);
            }
            else {
                timeline.setLoopType(OF_LOOP_NONE);
            }
    }

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
void testApp::ProcessMessage( const osc::ReceivedMessage &m)
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
void testApp::grabMutex()
{
#ifdef TARGET_WIN32
	WaitForSingleObject( mutex, INFINITE );
#else
	pthread_mutex_lock( &mutex );
#endif
}
//--------------------------------------------------------------
void testApp::releaseMutex()
{
#ifdef TARGET_WIN32
	ReleaseMutex( mutex );
#else
	pthread_mutex_unlock( &mutex );
#endif
}
//--------------------------------------------------------------
bool testApp::hasWaitingMessages()
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
bool testApp::getNextMessage( ofxOscMessage* message )
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
bool testApp::sendTCPMessage( ofxOscMessage& message, int client, ofxTCPServer * server )
{
	static const int OUTPUT_BUFFER_SIZE = 16384;
	char buffer[OUTPUT_BUFFER_SIZE];
    osc::OutboundPacketStream p( buffer, OUTPUT_BUFFER_SIZE );

	// serialise the message
	//p << osc::BeginBundleImmediate;
	appendMessage( message, p );
	//p << osc::EndBundle;

    //server->sendRawBytes( client, p.Data(), p.Size() );
	return server->sendRawBytes( client, p.Data(), p.Size() );
}
//--------------------------------------------------------------
void testApp::appendMessage( ofxOscMessage& message, osc::OutboundPacketStream& p )
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
void testApp::audioReceived(float * input, int bufferSize, int nChannels) {
	pd.audioIn(input, bufferSize, nChannels);
}
//--------------------------------------------------------------
void testApp::audioRequested(float * output, int bufferSize, int nChannels) {
	pd.audioOut(output, bufferSize, nChannels);
}
//--------------------------------------------------------------
void testApp::print(const std::string& message) {
    cout << message << endl;
}
//--------------------------------------------------------------
void testApp::enlazarCliente(string _cName, int _cId, string _cIP) {
    cliente tempC;
    tempC.sName = _cName;
    tempC.sId = _cId;
    tempC.sIp = _cIP;
    clientList.push_back(tempC);
}
//--------------------------------------------------------------
void testApp::desenlazarCliente(int _cId) {
    if(!clientList.empty())
        {
            for (unsigned int i = 0; i < clientList.size(); i++)
            {
                if(clientList[i].sId == _cId)
                {
                    cout << "Cliente nr " << clientList[i].sId << " de nombre " << clientList[i].sName << " se ha desenlazado de la pista." << endl;
                    clientList.erase(clientList.begin()+i);
                }

            }
        }
}
//--------------------------------------------------------------
bool testApp::tieneCliente() {
    if (clientList.size() > 0) {
        return true;
    }
    else {
        return false;
    }
}
