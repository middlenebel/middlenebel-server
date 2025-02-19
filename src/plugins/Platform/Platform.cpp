
#include "../../inc/utils/Util.hpp"
#include "../../incPlugins/Platform.hpp"
#include "../../incPlugins/DockerDesktop.hpp"
#include "../../incPlugins/K8S.hpp"
#include "../../inc/Core.hpp"

Platform* platform = nullptr;

int Platform::platformNum = 0;

extern "C" void* loadPlugin(Component* parent){
    cout << "Loading plugin Platform...\n";
    CREATE_NEBEL_PLUGIN( "platform", platform , Platform(parent) );

    return (void*)platform;
}

extern "C" void parse(void* instance ){
    Platform* platform = (Platform*) instance;
    platform->parse();
}

Platform::Platform(Component* parent):Component(parent){
    init();
    // parent->addComponent(this); //Do it in loadPlugin()
}

void Platform::init(){
    attributes[ATT_CLASSNAME]="Platform";
    attributes[ATT_NAME] = "Platform-"+to_string(platformNum++);
    LOG( "Platform started!");
}

void Platform::parse(){
    LOG( "Parse by Platform...");
    bool parsedName = false;
    string key="", value="";
    while ( readToken() ){
        //DEBUG LOG( "\nPlatform TOKEN: " << token );

        if (isBlanc()) { SEG_SHOW_BLANC(); }
        else if (isToken( TK_POINT )){ return; }
        else if (isToken( TK_DockerDesktop )){ parseDockerDesktop(); }
        else if (isToken( TK_K8S)){ loadPlugin(token); } //parseK8S(); }
        else if (isToken( TK_Server )){ parseServer(); }
        else if (isToken( TK_DBase )){ parseDBase(); }
        else if (isToken( TK_Kafka )){ loadPlugin(token); }
        else if (isToken( TK_MySQL )){ loadPlugin(token); }
        else if (isToken( TK_NebelComp )){ loadPlugin(token); }
        else if ( !parsedName && token.compare( "" ) != 0 ){ // Gets the name of platform
                LOG( "Platform " + token );
                attributes[ATT_NAME] = token;
                parsedName = true;
            }
        else PARSE_ATT_KEY_TOKEN(attributes, key, token);
        //TODO else detect errors, now go to attribs
        // else {
        //     LOG( "Platform UNKNOWN " + token );
        //     lex->lexBack();
        //     parent->parse();
        // }
    }
}

void Platform::parseDockerDesktop(){
    LOG( "DockerDesktop" );
    // DockerDesktop* dk = new DockerDesktop(this);
    DockerDesktop* dk;
    CREATE_NEBEL( "platform/dockerdesktop", dk , DockerDesktop(this) );
    dk->parse();
}

// void Platform::parseK8S(){
//     LOG( "K8S");
//     // K8S* k8s = new K8S(this);
//     K8S* k8s;
//     CREATE_NEBEL( "platform/k8s", k8s , K8S(this) );
//     k8s->parse();
// }

void Platform::parseServer(){
    //DEBUG LOG( "Server" );

    while ( readToken() ){
        //DEBUG LOG( "\nTOKEN: " << parser->token );

        if (isBlanc()) { LOG( "Skip blank" ); }
        // else if (isToken( TK_Server )){ Server(); }
        // else if (isToken( TK_DBase )){ DBase(); }
        else if (token.compare( "" ) != 0 ){
                attributes[ATT_NAME] = token;
            }
        else return;
    }
    LOG( "End!" );
}

void Platform::parseDBase(){
}

