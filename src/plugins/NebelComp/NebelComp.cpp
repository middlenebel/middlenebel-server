
// #include <chrono>
// #include <thread>

#include <jsoncpp/json/json.h>

#include "../../incPlugins/NebelComp.hpp"
#include "../../inc/Core.hpp"

#define TK_NebelComp "NebelComp"
#define TK_Class "Class"
#define TK_System "System"

using namespace std;

NebelComp* nebelComp = nullptr;

int NebelComp::nebelCompNum = 0;

extern "C" void* loadPlugin(Component* parent){
    cout << "Loading plugin NebelComp...\n";
    // nebelComp = new NebelComp(parent);
    CREATE_NEBEL_PLUGIN( "nebelComp", nebelComp , NebelComp(parent) );
    return (void*)nebelComp;
}

extern "C" void parse(void* instance ){
    ((NebelComp*) instance)->parse();
}

NebelComp::NebelComp(Component* parent):Component(parent){
    loadedComponent = nullptr;

    init();
    // parent->addComponent(this); //Do it in loadPlugin()
}

void NebelComp::init(){
    attributes[ATT_CLASSNAME]="NebelComp";
    attributes[ATT_NAME] = "NebelComp-"+to_string(nebelCompNum++);
    LOG( "NebelComp started!");
}
void NebelComp::parse(){
    LOG( "Parse by NebelComp...");
    bool parsedName = false;
    string key="";
    string value="";
    // Core* core = (Core*) getRootComponent( this );
    while ( readToken() ){
        //DEBUG LOG( "NebelComp TOKEN: " + token );

        if (isBlanc()) { SEG_SHOW_BLANC(); }
        else if (isToken( TK_POINT )){ return; }
        else if (isToken( TK_PORT_FORWARD )){ parsePortForward(); }
        else if (isToken( TK_Executor )){ parseExecutor(); } 
        else if (isToken( TK_Class )){ parseClass(); } 
        else if (isToken( TK_System )){ parseSystem(); } 
        else if ( !parsedName && token.compare( "" ) != 0 ){ // Gets the name of component
            LOG( "NebelComp " + token );
            attributes[ATT_NAME] = token;
            parsedName = true;
        } else PARSE_ATT_KEY_TOKEN(attributes, key, token);
    }
}

void NebelComp::parseExecutor(){
    //DEBUG LOG( "Executor");
    bool parsedName = false;
    bool parsedLibrary = false;
    string name;
    string key="", value="";
    Core* core = (Core*) getRootComponent( this );
    while ( readToken() ){
        //DEBUG LOG( "\NebelComp TOKEN: " + token );

        if (isBlanc()) { SEG_SHOW_BLANC(); }
        else if (isToken( TK_POINT )){ return; }
        // else if (isToken( TK_Executor )){ parseExecutor(); } 
        // else if (isToken( TK_Class )){ parseClass(); } 
        // else if (isToken( TK_System )){ parseSystem(); } 
        else if ( !parsedName && token.compare( "" ) != 0 ){ // Gets the name of action
            //DEBUG
            LOG( "Executor " + token );
            core->executors[token] = this; 
            // this->actions.push_back( name );
            name = token;
            parsedName = true;
        }else if ( !parsedLibrary && token.compare( "" ) != 0 ){ // Gets the name of library
            LOG( "Action executor " + name + " library " + token );
            // core->executors[name] = this; 
            this->actions[ name ] = token;
            parsedLibrary = true;
        } else { 
            lex->lexBack();
            parseSubAttributes( name ); 
            break;
        }; //PARSE_ATTRIBUTES();
    }
}
void NebelComp::parseSubAttributes( string name ){
    Component* subAttributes;
    CREATE_NEBEL( "NebelComp/subAttributes", subAttributes , Component( this ));
    subAttributes->attributes[ATT_NAME] = name;
    addComponent( subAttributes );
    std::map<string, string> &attributes = subAttributes->attributes;

    string key="", value="";
    while ( readToken() ){
        if (isBlanc()) { SEG_SHOW_BLANC(); }
        else if (isToken( TK_POINT )){ return; }
        else PARSE_ATT_KEY_TOKEN(attributes, key, token);
    }
}

// // Use attributes: app, namespace, port
// void NebelComp::startPortForward(){
//     //boost::this_thread::sleep( boost::posix_time::seconds(1) );
//     std::this_thread::sleep_for(3000ms);
//     string nameSpace=getAtt(ATT_NAMESPACE, "default");
//     string command = "kubectl port-forward `kubectl get pod -l app="+getAtt(ATT_APPPP)+" -n "+getAtt(ATT_NAMESPACE)+" -o jsonpath='{.items[0].metadata.name}'` "+ getAtt(ATT_PORT)+" -n "+getAtt(ATT_NAMESPACE)+ " &";
    
//     string resultCommand = systemCommand( command , "portforward.out", "portforward_error.out");
//     while (resultCommand.rfind("error: unable to forward port because pod is not running.") != std::string::npos){
//         LOG("Waiting to server running to port-forward...");
//         std::this_thread::sleep_for(1000ms);
//         resultCommand = systemCommand( command );
//     }
//     LOG( "PortForward running...");
//     portForwardInited = true;
// }

// void NebelComp::stopPortForward(){
//     if (!portForwardInited) return;
//     string command = "kill -9 `ps -ef |grep \"port-forward "+getAtt(ATT_APP)+"\"|awk '!/grep/ {print $2}'`";
//     string resultCommand = systemCommand( command , "portforward-stop.out", "portforward-stop_error.out" );
//     LOG( "PortForward stoped!");
// }

int NebelComp::getObjectNum(){
    int n = objectsNum;
    return n;
}

void NebelComp::doPlay(std::list<std::string> *cmds){
    Component::doDestroy(cmds);
}
void NebelComp::doDestroy(std::list<std::string> *cmds){
    Component::doDestroy(cmds);
}
string NebelComp::doQuit(){
    return Component::doQuit();
}

string NebelComp::execute( string json ){
    //DEBUG 
    LOG(  "NebelComp executing...");

    // if (portForward)
    //     startPortForward();
        
    Json::Value actionJson;
    Json::Reader reader; 
    bool isOk = reader.parse( json, actionJson );
    if (!isOk){
        LOG( "Error parsing actions json! " + json);
        return (string)"{ \"result\" : \"KO\" , \"message\" : \"Error parsing actions json!\" }";
    }

    const Json::Value actionValue = actionJson["action"]; //Allways "NebelComp"
    string actionStr = actionValue.asString();
    string library = actions[ actionStr ];

    //DEBUG 
    LOG(  "NebelComp action " + actionStr + " -> " + library);

    if (actionStr=="DoNothing"){
        LOG( "NebelComp: DoNothing OK!");
        return RETURN_EXECUTE_OK;
    }

    const Json::Value dataValue = actionJson["data"];
    string dataStr = dataValue.asString();

    Json::Value dataJson;
    Json::Reader readerData;
    bool isOkData = readerData.parse( dataStr, dataJson );
    if (!isOkData){
        LOG( "NebelComp: Error parsing data json!");
        return (string)"{ \"result\" : \"KO\" , \"message\": \"Error parsing data json!\" }";
    }

    const Json::Value commandValue = dataJson["command"];
    string commandStr = commandValue.asString();

    if (commandStr=="Load"){
        Component* obj = (Component*) loadPlugin(library);
        objects[library] = obj;
        LOG( "NebelComp: Library loaded!");
        return (string)"{ \"result\" : \"OK\" , \"message\": \"Library loaded!\" }";
    }
    
    if ((commandStr=="Run") && !objects[library] ){
        LOG( "NebelComp: Error Library not loaded!!");
        return (string)"{ \"result\" : \"KO\" , \"message\": \"Error Library not loaded!\" }";
    }
    if (commandStr=="Run"){
        LOG(  "NebelComp command: " + commandStr );
        objects[library]->execute( dataStr );
        LOG(  "NebelComp command: " + commandStr + " executed OK!");
        return (string)"{ \"result\" : \"OK\" , \"message\": \"Executed OK!\" }";
    }

    return RETURN_EXECUTE_KO;
}

string NebelComp::executeSystem(string json){
    return "OK";
}
string NebelComp::executeClass( string json ){
    return "OK";    
}
