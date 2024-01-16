
#include <stdlib.h>
// #include <chrono>
// #include <thread>
#include <jsoncpp/json/json.h>

#include "/usr/include/mysql_connection.h"
#include "/usr/include/mysql_driver.h"
#include "/usr/include/mysql_error.h"

// https://stackoverflow.com/questions/20583026/mysql-connector-c-preparedstatement-forward-declaration-of-class-sqlprepar
#include "/usr/include/cppconn/prepared_statement.h"

#include "../../inc/Core.hpp"
#include "../../incPlugins/MySQL.hpp"

using namespace std;

int MySQL::mySQLNum = 0;

MySQL* mysql = nullptr;

extern "C" void* loadPlugin(Component* parent){
    cout << "Loading plugin MySQL...\n";
    mysql = new MySQL(parent);
    return (void*)mysql;
}

extern "C" void parse(void* instance ){
    cout << "MySQL loaded!\n";
    ((MySQL*) instance)->parse();
}

MySQL::MySQL(Component* parent):Component(parent){ 
  LOG("MySQL made by " << parent->attributes["name"]);

  init();
  parent->addComponent(this);
  parent->objectsNum++; //On deleting objects the parent delete this as his child.
}
void MySQL::init(){
    attributes["className"]="K8S-MySQL";
    attributes["name"] = "MySQL-"+to_string(mySQLNum++);
    LOG( "MySQL started!");
}
void MySQL::parse(){
    LOG( "Parse by K8S-MySQL...");
    bool parsedName = false;
    string key="";
    string value="";
    // Core* core = (Core*) getRootComponent( this );
    while ( readToken() ){
        //DEBUG LOG( "NebelComp TOKEN: " << token );

        if (isBlanc()) { SEG_SHOW_BLANC(); }
        else if (isToken( TK_POINT )){ return; }
        else if (isToken( TK_PORT_FORWARD )){ parsePortForward(); }
        else if (isToken( TK_INITIALIZE )){ parseInitialize(); }
        else if ( !parsedName && token.compare( "" ) != 0 ){ // Gets the name of component
            LOG( "MySQL " << token );
            attributes["name"] = token;
            parsedName = true;

            string actionPF = (string) getAtt("className")+"-"+TK_INITIALIZE;
       
            Core* core = (Core*) getRootComponent( this );
            core->executors[actionPF] = this; 
            this->actions[ actionPF ] = actionPF;
        } else PARSE_ATTRIBUTES();
    }
}
void MySQL::parseInitialize(){
    LOG( "Initialize...");
    bool parsedName = false;
    bool parsedScript = false;
    string key="";
    string value="";
    // Core* core = (Core*) getRootComponent( this );
    while ( readToken() ){
        //DEBUG LOG( "NebelComp TOKEN: " << token );

        if (isBlanc()) { SEG_SHOW_BLANC(); }
        else if (isToken( TK_POINT )){ return; }
        else if ( !parsedName && token.compare( "" ) != 0 ){ // Gets the name of component
            LOG( "MySQL Initialize" << token );
            attributes[ ATTRIB_INITIALIZE ] = token;
            parsedName = true;
        }else if ( !parsedScript && token.compare( "" ) != 0 ){ // Gets the name of component
            LOG( "MySQL Script: " << token );
            attributes[ ATTRIB_INITIALIZE_SCRIPT ] = token;
            parsedScript = true;
        }else PARSE_ATTRIBUTES();
    }
}

string MySQL::doPlay(){
     string result = "OK";
     try{
        LOG("K8S-MySQL Deploying...");
        string nameSpace=getAtt("namespace", "default");
        
        string fileName, command, resultCommand;

        LOG("K8S-MySQL Deploying app...");
        fileName="./cfgPlugins/deploy-mysql-nebel.tmp.yaml";
        command = "kubectl apply -f "+fileName+" -n "+nameSpace;
        resultCommand = systemCommand( command );

        LOG("K8S-MySQL Deploying services...");
        fileName="./cfgPlugins/service-mysql-nebel.tmp.yaml";
        command = "kubectl apply -f "+fileName+" -n "+nameSpace;
        resultCommand = systemCommand( command );

        if (portForward)
            startPortForward();
    }catch(...){
          result = "ERROR";
     }
     return result;
}
string MySQL::doDestroy(){
     string result = "OK";
     try{
        if (portForward)
            stopPortForward();

        string name=attributes["name"];
        string nameSpace=getAtt("namespace", "default");

        string command, resultCommand;

        command = "kubectl delete deployment mysql-nebel -n "+nameSpace;
        resultCommand += systemCommand( command );

        command = "kubectl delete service mysql-nebel -n "+nameSpace;
        resultCommand += systemCommand( command );

        LOG( "Destroyed K8S-MySQL "+attributes["name"]); // Can have " in the messages  + ": " + resultCommand);
     }catch(...){
          result = "ERROR";
     }
     return result;
}
string MySQL::doQuit(){
    if (portForward)
        stopPortForward();
    return Component::doQuit();
}
void MySQL::onLoad(){
    LOG( "Hello World! v.a0.1.Tooling MySQL Library loaded" );
}

string MySQL::execute( string json ){
    LOG( "K8S-MySQL Executing..." );

    // if (portForward)
    //     startPortForward();
    
    Json::Value actionJson;
    Json::Reader reader; 
    bool isOk = reader.parse( json, actionJson );
    if (!isOk){
        LOG( "Error parsing actions json! " + json);
        return (string)"{ \"result\" : \"KO\" , \"message\" : \"Error parsing actions json!\" }";
    }

    const Json::Value actionValue = actionJson["action"]; //Allways "K8S-MySQL"
    string actionStr = actionValue.asString();

    string library = actions[ actionStr ];

    // if ( actionStr == TK_KAFKA_PRODUCER){
    //     const Json::Value dataValue = actionJson["data"];
    //     string dataStr = dataValue.asString();
    //     return executeProducer( dataStr );
    // }else 
    if ( actionStr == (attributes["className"] +"-"+ TK_TESTCONN) ){ //TODO
        // const Json::Value dataValue = actionJson["data"];
        // string dataStr = dataValue.asString();
        return executeTestConn( );
    }else if ( actionStr == (attributes["className"] +"-"+ TK_INITIALIZE) ){ //TODO
        // const Json::Value dataValue = actionJson["data"];
        // string dataStr = dataValue.asString();
        return executeInitialize( );
    }else 
    if ( actionStr == (attributes["className"] +"-"+ TK_PORT_FORWARD)){
        if (portForward){
            startPortForward();
            LOG( "NebelComp: StartPortForward!");
        }else{
            LOG( "NebelComp: portForward is FALSE!!!");
        }
        return RETURN_EXECUTE_OK;
    }

    //DEBUG 
    LOG(  "NebelComp actionStr " << actionStr << " -> " << library);

    // sql::mysql::MySQL_Driver *driver;
    // sql::Connection *con;
    // sql::Statement *stmt;

    // driver = sql::mysql::get_mysql_driver_instance();
    // con = driver->connect("tcp://localhost:"+getAtt("port"), "root", getAtt("pass"));

    // LOG( "Connection isValid? " << (con->isValid()?"true":"false") );

    // con->setSchema("NEBEL_DB"); //Throw exception if not exists.
    // bool result = false;
    // stmt = con->createStatement();
    // stmt->execute("CREATE DATABASE IF NOT EXISTS NEBEL_DB"); //CREATE DATABASE IF NOT EXISTS NEBEL_DB
    // stmt->execute("USE NEBEL_DB");
    // result = stmt->execute("DROP TABLE IF EXISTS nebel");
    // LOG("DROP TABLE Result: " << (result?"true":"false"));

    // stmt->execute("CREATE TABLE nebel(id INT, label CHAR(1))");
    // stmt->execute("INSERT INTO nebel(id, label) VALUES (1, 'a')");

    // delete stmt;
    // delete con;

    return RETURN_EXECUTE_OK;
}

string MySQL::executeTestConn(){
    sql::mysql::MySQL_Driver *driver;
    sql::Connection *con;

    driver = sql::mysql::get_mysql_driver_instance();
    con = driver->connect("tcp://localhost:"+getAtt("port"), "root", getAtt("pass"));

    LOG( "Connection isValid? " << (con->isValid()?"true":"false") );

    delete con;
    return RETURN_EXECUTE_OK;
}

string MySQL::executeInitialize(){
    sql::mysql::MySQL_Driver *driver;
    sql::Connection *con;
    // sql::Statement *stmt;

    driver = sql::mysql::get_mysql_driver_instance();
    con = driver->connect("tcp://localhost:"+getAtt("port"), "root", getAtt("pass"));

    //LOG( "Connection isValid? " << (con->isValid()?"true":"false") );

    bool result = executeScript(con, "./scripts/"+getAtt( ATTRIB_INITIALIZE_SCRIPT ));

    // bool result = false;
    // stmt = con->createStatement();
    // stmt->execute("CREATE DATABASE IF NOT EXISTS "+getAtt("name")); //CREATE DATABASE IF NOT EXISTS NEBEL_DB
    // stmt->execute("USE "+getAtt("name"));
    // result = stmt->execute("DROP TABLE IF EXISTS nebel");
    // LOG("DROP TABLE Result: " << (result?"true":"false"));

    // stmt->execute("CREATE TABLE nebel(id INT, label CHAR(1))");
    // stmt->execute("INSERT INTO nebel(id, label) VALUES (1, 'a')");
    // delete stmt;

    delete con;

    return (result ? (RETURN_EXECUTE_OK) : (RETURN_EXECUTE_KO) );
}

bool MySQL::executeScript(sql::Connection *con, string file){
    bool resultOk = true;
    ifstream scriptFile;
    string script;
    sql::Statement *stmt;

    scriptFile.open( file );

    stmt = con->createStatement();

    if (scriptFile.is_open()){
        string line;
        while ( resultOk && getline (scriptFile,line) ){
            script += line + "\\n";
            try{
                stmt->execute( line );
                //LOG( "SQL Execute OK!" );
            }catch(...){
                LOG( "SQL Execute ERROR! " << line);
                resultOk = false;
            }
        } 
        scriptFile.close();
    }
    // std::replace( script.begin(), script.end(), '"', "'");
    //DEBUG cout << "File: " << script << "\n";

    delete stmt;
    return resultOk;
}