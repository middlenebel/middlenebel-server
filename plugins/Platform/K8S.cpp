
#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/version.hpp>
#include <boost/asio/connect.hpp>
#include <boost/asio/ip/tcp.hpp>

#include <boost/asio.hpp>

#include <cstdlib>
#include <iostream>
#include <string>

//if include <json/value.h> line fails (latest kernels), try also:
 #include <jsoncpp/json/json.h>
// #include <json/value.h>
#include <fstream>

#include "../../inc/Core.hpp"
#include "../../inc/Component.hpp"
#include "../../inc/Util.hpp"
#include "../../incPlugins/K8S.hpp"

#include "../../incPlugins/K8SDeployment.hpp"
#include "../../incPlugins/K8SNamespace.hpp"
#include "../../incPlugins/K8SService.hpp"
#include "../../incPlugins/K8SLabel.hpp"

#include "../../incPlugins/Kafka.hpp"

namespace beast = boost::beast;     // from <boost/beast.hpp>
namespace http = beast::http;       // from <boost/beast/http.hpp>
namespace net = boost::asio;        // from <boost/asio.hpp>
using tcp = net::ip::tcp;           // from <boost/asio/ip/tcp.hpp>

K8S::K8S(Component* parent):Component( parent ){
     init();
     parent->addComponent(this);
}

void K8S::init(){
     attributes[ATT_CLASSNAME]="K8S";
     attributes[ATT_NAME]="K8S";
     attributes[ATT_CLASSNAME]="K8S";
     attributes[ATT_NAME]="K8S";
     string config = getConfig();
     if ( config == MIDDLENEBEL_EXIT_FAILURE){
          LOG( "K8S Starting proxy...");
          system("kubectl proxy --port 8091 &");
          LOG( "K8S Proxy started...");
          sleep(1);
          config = getConfig();
     }
     if ( config != MIDDLENEBEL_EXIT_FAILURE)
          parseJsonConfig( config );
}

string K8S::getConfig(){
     LOG( "K8S Getting configuration...");
     string config = "UNDEF";
     try{
          config = Util::getHttp( "localhost", "8091", "/api/");
          //DEBUG LOG( "K8S Config: " << config );
          //KILL kill -9 `pgrep -f "kubectl proxy --port 8081"`
     }catch(...){
          LOG( "K8S No connection with K8S!!!" );
     }
     return config;
}

void K8S::parse(){
    LOG( "Parse by K8S...");
    string key="", value="";
    string key="", value="";
    while ( readToken() ){
        //DEBUG LOG( "\nTOKEN: " << parser->token );

          if (isBlanc()) { SEG_SHOW_BLANC(); }
          else if (isToken( TK_POINT )){ return; }
          else if (isToken( TK_K8S_Deployment )){ parseDeployment(); }
          else if (isToken( TK_K8S_CreateNamespaces )){ parseNamespace(); }
          else if (isToken( TK_K8S_Service )){ parseService(); }
          else if (isToken( TK_Kafka )){ loadPlugin(token);  }
          else PARSE_ATT_KEY_TOKEN(attributes, key, token);
    }
}

void K8S::parseDeployment(){
     //DEBUG LOG( "Depoyment");
     // K8SDeployment* deployment = new K8SDeployment(this);
     K8SDeployment* deployment;
     CREATE_NEBEL( "k8s/deployment", deployment , K8SDeployment(this) );
     k8s_deployments.push_back( deployment );
     bool parsedName = false;
     string key="", value="";
     string key="", value="";
     while ( readToken() ){
          //DEBUG         LOG( "\nDeployment TOKEN: " << token );

          if (isBlanc()) { SEG_SHOW_BLANC(); }
          else if (isToken( TK_POINT )){ return; }
          else if (isToken( TK_K8S_Labels )){ parseLabels( deployment ); }
          else if (isToken( TK_K8S_Env )){ parseEnv( deployment ); }
          else if ( !parsedName ){ // Gets the name of platform
               LOG( "Deploy " + token );
               //deployment->name = token;
               deployment->attributes[ATT_NAME]=token;
               deployment->attributes[ATT_NAME]=token;
               parsedName = true;
          } else PARSE_ATT_KEY_TOKEN(deployment->attributes, key, token);
     }
}

void K8S::parseNamespace(){
     //DEBUG LOG( ATT_NAMESPACE);
     //DEBUG LOG( ATT_NAMESPACE);
     // K8SNamespace* aNamespace = new K8SNamespace(this);
     K8SNamespace* aNamespace;
     CREATE_NEBEL( "k8s/namespace", aNamespace , K8SNamespace(this) );
     k8s_namespaces.push_back( aNamespace );
     bool parsedName = false;
     string key="", value="";
     while ( readToken() ){
          //DEBUG         LOG( "\nDeployment TOKEN: " << token );

          if (isBlanc()) { SEG_SHOW_BLANC(); }
          else if (isToken( TK_POINT )){ return; }
          else if ( !parsedName ){ // Gets the name of platform
               LOG( "Namespace " + token );
               aNamespace->attributes[ATT_NAME] = token;
               aNamespace->attributes[ATT_NAME] = token;
               parsedName = true;
          } else PARSE_ATT_KEY_TOKEN(aNamespace->attributes, key, token);
     }
}

void K8S::parseService(){
     //DEBUG LOG( "Service");
     K8SService* service;
     CREATE_NEBEL( "k8s/service", service , K8SService(this) );
     k8s_services.push_back( service );
     bool parsedName = false;
     string key="", value="";
     while ( readToken() ){
          //DEBUG         LOG( "\nDeployment TOKEN: " << token );

          if (isBlanc()) { SEG_SHOW_BLANC(); }
          else if (isToken( TK_POINT )){ return; }
          else if (isToken( TK_K8S_Labels )){ parseLabels( service ); }
          else if ( !parsedName ){ // Gets the name of platform
               LOG( "Service " + token );
               service->attributes[ATT_NAME] = token;
               service->attributes[ATT_NAME] = token;
               parsedName = true;
          } else PARSE_ATT_KEY_TOKEN(service->attributes, key, token);

     }
}

void K8S::parseLabels( Component* k8sElement ){
     LOG( "Labels");
     // K8SLabel* label = new K8SLabel( k8sElement );
     K8SLabel* label;
     CREATE_NEBEL( "k8s/label", label , K8SLabel( k8sElement ) );
     // k8sElement->labels.insert( label );
     string key="", value="";
     while ( readToken() ){
          //DEBUG         LOG( "\K8SLabel TOKEN: " << token );

          if (isBlanc()) { SEG_SHOW_BLANC(); }
          else if (isToken( TK_POINT )){ return; }
          else PARSE_ATT_KEY_TOKEN(label->attributes, key, token);
     }
}

void K8S::parseEnv( K8SDeployment* k8sDep ){
     LOG( "Env");

     string key="", value="";
     while ( readToken() ){
          //DEBUG         LOG( "\K8SEnv TOKEN: " << token );

          if (isBlanc()) { SEG_SHOW_BLANC(); }
          else if (isToken( TK_POINT )){ return; }
          else PARSE_ATT_KEY_TOKEN(k8sDep->env, key, token);
     }
}

void K8S::parseJsonConfig(string json){
     Json::Value api;
     Json::Reader reader; //std::stringstream sstr(json);
     bool isOk = reader.parse( json, api ); // sstr >> api;
     if (!isOk){
          LOG( "K8S Error in API json!");
          return;
     }
     //DEBUG      LOG( "API " << api["versions"] );

     const Json::Value versions = api["versions"];
     JSON_ARRAY_TO_LIST(versions, k8s_versions);
     // for (std::list<string>::iterator child = k8s_versions.begin(); child != k8s_versions.end(); ++child)
     //      LOG( "VERSION " << (*child) );

     LOG( "K8S Config loaded." );
}

string K8S::getJsonComponent(){
     //DEBUG     LOG( "K8S getJsonComponent...");
     string components="{";
     components+=JSON_PROPERTY(ATT_CLASSNAME, attributes[ATT_CLASSNAME])+",\n";
     components+=JSON_PROPERTY(ATT_NAME, attributes[ATT_NAME])+",\n";
     components+=JSON_PROPERTY(ATT_CLASSNAME, attributes[ATT_CLASSNAME])+",\n";
     components+=JSON_PROPERTY(ATT_NAME, attributes[ATT_NAME])+",\n";
     components+=JSON_ARRAY( "actions", getJsonActions() )+",\n";
     components+=JSON_ARRAY("childs", getJsonChilds() )+",\n";
     string json_versions = "";
     LIST_TO_STRING(k8s_versions, json_versions);
     if (json_versions.empty())
          json_versions = "\"K8S not detected\"";
     components+=JSON_ARRAY("versions", json_versions )+",\n";
     components+=JSON_ARRAY("deployments", getJsonDeploys() )+",\n";
     components+=JSON_ARRAY("namespaces", getJsonNamespaces() )+",\n";
     components+=JSON_ARRAY("services", getJsonServices() )+"\n";
     components+="}";
     return components;
}

string K8S::getJsonDeploys(){
    string jsonDeploys = "";
    for (std::list<K8SDeployment*>::iterator deploy = k8s_deployments.begin(); deploy != k8s_deployments.end(); ++deploy){
        jsonDeploys += (*deploy)->getJsonComponent()+"\n";
        jsonDeploys += (std::next(deploy) != k8s_deployments.end() ? ",\n" : "\n");
    }
    return jsonDeploys;
}

string K8S::getJsonNamespaces(){
    string jsonNamespaces = "";
    for (std::list<K8SNamespace*>::iterator aNamespace = k8s_namespaces.begin(); aNamespace != k8s_namespaces.end(); ++aNamespace){
        jsonNamespaces += (*aNamespace)->getJsonComponent()+"\n";
        jsonNamespaces += (std::next(aNamespace) != k8s_namespaces.end() ? ",\n" : "\n");
    }
    return jsonNamespaces;
}

string K8S::getJsonServices(){
    string jsonServices = "";
    for (std::list<K8SService*>::iterator service = k8s_services.begin(); service != k8s_services.end(); ++service){
        jsonServices += (*service)->getJsonComponent()+"\n";
        jsonServices += (std::next(service) != k8s_services.end() ? ",\n" : "\n");
    }
    return jsonServices;
}

string K8S::doPlay(){
     string result = "";
     for (std::list<K8SNamespace*>::iterator aNamespace = k8s_namespaces.begin(); aNamespace != k8s_namespaces.end(); ++aNamespace){
          result += (*aNamespace)->apply();
     }
     for (std::list<K8SDeployment*>::iterator deploy = k8s_deployments.begin(); deploy != k8s_deployments.end(); ++deploy){
          result += (*deploy)->apply();
     }
     for (std::list<K8SService*>::iterator service = k8s_services.begin(); service != k8s_services.end(); ++service){
          result += (*service)->apply();
     }
     return result;
}

string K8S::doDestroy(){
     string result = "";
     for (std::list<K8SDeployment*>::iterator deploy = k8s_deployments.begin(); deploy != k8s_deployments.end(); ++deploy){
          result += (*deploy)->destroy();
     }
     for (std::list<K8SService*>::iterator service = k8s_services.begin(); service != k8s_services.end(); ++service){
          result += (*service)->destroy();
     }
     for (std::list<K8SNamespace*>::iterator aNamespace = k8s_namespaces.begin(); aNamespace != k8s_namespaces.end(); ++aNamespace){
          result += (*aNamespace)->destroy();
     }
     return result + Component::doDestroy();
     return result + Component::doDestroy();
}

string K8S::doQuit(){
     LOG( attributes[ATT_NAME] + ".doQuit! Childs: " + to_string(childs.size()) + " Plugins: "+ to_string(plugins.size()) );
     string result = (string) "doQuit " + attributes[ATT_NAME]+"\n";
     LOG( attributes[ATT_NAME] + ".doQuit! Childs: " + to_string(childs.size()) + " Plugins: "+ to_string(plugins.size()) );
     string result = (string) "doQuit " + attributes[ATT_NAME]+"\n";
     for (std::list<K8SDeployment*>::iterator deploy = k8s_deployments.begin(); deploy != k8s_deployments.end(); ++deploy){
         result += "Delete K8SDeployment "+(*deploy)->attributes[ATT_NAME]+"\n";
         result += "Delete K8SDeployment "+(*deploy)->attributes[ATT_NAME]+"\n";
          //     delete(*deploy);
          DELETE_NEBEL( "K8S/deploy" , *deploy );
     }
     k8s_deployments.clear();
     for (std::list<K8SService*>::iterator service = k8s_services.begin(); service != k8s_services.end(); ++service){
          result += "Delete K8SService "+(*service)->attributes[ATT_NAME]+"\n";
          result += "Delete K8SService "+(*service)->attributes[ATT_NAME]+"\n";
          // delete(*service);
          DELETE_NEBEL( "K8S/service" , *service);
     }
     k8s_services.clear();
     for (std::list<K8SNamespace*>::iterator aNamespace = k8s_namespaces.begin(); aNamespace != k8s_namespaces.end(); ++aNamespace){
          result += "Delete K8SNamespace "+(*aNamespace)->attributes[ATT_NAME]+"\n";
          result += "Delete K8SNamespace "+(*aNamespace)->attributes[ATT_NAME]+"\n";
          // delete(*aNamespace);
          DELETE_NEBEL( "K8S/aNamespace" , *aNamespace);
     }
     k8s_namespaces.clear();
     return Component::doQuit();
     return Component::doQuit();
}

int K8S::getObjectNum(){
    int n = objectsNum;

    for (std::list<K8SDeployment*>::iterator deploy = k8s_deployments.begin(); deploy != k8s_deployments.end(); ++deploy){
         n += (*deploy)->getObjectNum();
     }
     for (std::list<K8SService*>::iterator service = k8s_services.begin(); service != k8s_services.end(); ++service){
          n += (*service)->getObjectNum();
     }
     for (std::list<K8SNamespace*>::iterator aNamespace = k8s_namespaces.begin(); aNamespace != k8s_namespaces.end(); ++aNamespace){
          n += (*aNamespace)->getObjectNum();
     }
     return n;
}

/* K8S cubectl
--------------

kubectl get namespaces
kubectl get pods -n kube-system

kubectl apply -f https://raw.githubusercontent.com/kubernetes/dashboard/v2.7.0/aio/deploy/recommended.yaml

kubectl -n kubernetes-dashboard create token admin-user --duration=6000h
kubectl -n kubernetes-dashboard create token admin-user --duration=6000h
kubectl  create token --help
eyJhbGciOiJSUzI1NiIsImtpZCI6Im8wdU5ocUIzN2o4Z01oQ2dRS1lZalEza203N19QbTFfMDNCUGlwQ3drMzQifQ.eyJhdWQiOlsiaHR0cHM6Ly9rdWJlcm5ldGVzLmRlZmF1bHQuc3ZjLmNsdXN0ZXIubG9jYWwiXSwiZXhwIjoxNzAyNTUyMjYwLCJpYXQiOjE3MDI1NDg2NjAsImlzcyI6Imh0dHBzOi8va3ViZXJuZXRlcy5kZWZhdWx0LnN2Yy5jbHVzdGVyLmxvY2FsIiwia3ViZXJuZXRlcy5pbyI6eyJuYW1lc3BhY2UiOiJrdWJlcm5ldGVzLWRhc2hib2FyZCIsInNlcnZpY2VhY2NvdW50Ijp7Im5hbWUiOiJhZG1pbi11c2VyIiwidWlkIjoiZjJjMDk4MDMtNzc1Mi00ODM1LWFiNzYtODhjMTE4ZjI3OTBlIn19LCJuYmYiOjE3MDI1NDg2NjAsInN1YiI6InN5c3RlbTpzZXJ2aWNlYWNjb3VudDprdWJlcm5ldGVzLWRhc2hib2FyZDphZG1pbi11c2VyIn0.nDRrmi4RHdOgqJwLhvBavdaQxi3BkzFo4_9Wl2Jeuv_IdLrqd-wXr243anN1Bm-T1xOW9oGwrJj2MLPoRcieJclt6LpNY47ZDZvfVPzQv07yl02UoVY3ODEWFjGWpURbVi-PsD3rlxEQj9RhbxPvL7FANshwJ4Fqea3xljGAP0-2VTVIGxmcACr_wtO5KywaPwAlQtMeeGU7ZVwLgQyCuyiC20-sxlhqOqzsSa8hVvezXpUcCzOljVIKhaXyD5yLjlRTsQD5HN3jOCaNh5iG6bJRY7heVVnE2FHVzyE0FlOo7pA4aisysCOgKOC7Ff3vJeYMExnAjoJ8Ingkf-QlBA


http://localhost:8001/api/v1/namespaces/kubernetes-dashboard/services/https:kubernetes-dashboard:/proxy/

kubectl edit deployment -n kube-system kubernetes-dashboard --token-ttl=43200

--- KAFKA-CAT
kubectl get pods -n kafka
kubectl port-forward kafka-broker-6847548f89-7dk79 9092 -n kafka

echo "hello world!" | kafkacat -P -b localhost:9092 -t purchases

kcat -C -b localhost:9092 -t purchases

kubectl exec --stdin --tty mysql-nebel-855649bd5c-7c85d -- /bin/bash
kubectl -n mysql-nebel exec --stdin --tty mysql-nebel-855649bd5c-7c85d -- /bin/bash

kubectl port-forward mysql-nebel-855649bd5c-zzpbc 3306 -n mysql-nebel
*/