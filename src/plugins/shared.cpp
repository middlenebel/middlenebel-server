
#include <stdlib.h>
#include <chrono>
#include <thread>

// #include "/usr/include/mysql_connection.h"
#include "../inc/mysql_connection.h"
#include "../inc/mysql_driver.h"
#include "/usr/include/mysql_error.h"

// https://stackoverflow.com/questions/20583026/mysql-connector-c-preparedstatement-forward-declaration-of-class-sqlprepar
#include "/usr/include/cppconn/prepared_statement.h"

#include "../incPlugins/shared.hpp"
#include "../inc/Component.hpp"

using namespace std;

Shared* shared = nullptr;

extern "C" void* loadPlugin(Component* parent){
    cout << "Loading plugin Shared...\n";
    shared = new Shared(parent);
    return (void*)shared;
}

extern "C" void parse(void* instance ){
    cout << "Shared loaded!\n";
    ((Shared*) instance)->onLoad(); //This plugin not parse scripts.
}

Shared::Shared(Component* parent):Component(parent){ 
  LOG("Shared made by " << parent->attributes[ATT_NAME]);
  parent->addComponent(this);
  parent->objectsNum++; //On deleting objects the parent delete this as his child.
}

void Shared::onLoad(){
    LOG( "Hello World! v.a0.1.Tooling MySQL Library loaded" );
}

string Shared::execute( string json ){
    LOG( "Hello World! v.a0.1.Tooling MySQL Test Connection" );

    sql::mysql::MySQL_Driver *driver;
    sql::Connection *con;
    sql::Statement *stmt;

    driver = sql::mysql::get_mysql_driver_instance();
    con = driver->connect("tcp://localhost:"+getAtt(ATT_PORT), "root", getAtt("pass"));

    LOG( "Connection isValid? " << (con->isValid()?"true":"false") );

    // con->setSchema("NEBEL_DB"); //Throw exception if not exists.
    bool result = false;
    stmt = con->createStatement();
    stmt->execute("CREATE DATABASE IF NOT EXISTS NEBEL_DB"); //CREATE DATABASE IF NOT EXISTS NEBEL_DB
    stmt->execute("USE NEBEL_DB");
    result = stmt->execute("DROP TABLE IF EXISTS nebel");
    LOG("DROP TABLE Result: " << (result?"true":"false"));

    stmt->execute("CREATE TABLE nebel(id INT, label CHAR(1))");
    stmt->execute("INSERT INTO nebel(id, label) VALUES (1, 'a')");

    delete stmt;

    delete con;

    return RETURN_EXECUTE_OK;
}

