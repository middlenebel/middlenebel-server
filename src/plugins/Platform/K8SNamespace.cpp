
#include <cstdlib>
#include <iostream>
#include <string>

#include "../../inc/Core.hpp"
#include "../../inc/Component.hpp"
#include "../../inc/utils/Util.hpp"
#include "../../incPlugins/K8S.hpp"
#include "../../incPlugins/K8SNamespace.hpp"

K8SNamespace::K8SNamespace(Component* parent):Component(parent){
     init();
}

void K8SNamespace::init(){
     attributes[ATT_CLASSNAME]="K8SNamespace";
     attributes[ATT_NAME]="Undef";
}

string K8SNamespace::getJsonComponent(){
     //DEBUG LOG( "K8S Namespace getJsonComponent..." );
     string components="{";
     components+=JSON_PROPERTY(ATT_NAME, attributes[ATT_NAME])+",\n";
     string json_attributes = "";
     MAP_TO_STRING(attributes, json_attributes);
     components+=JSON_ARRAY("attributes", json_attributes )+"\n";
     components+="}";
     return components;
}

string K8SNamespace::apply(){
     cout << "K8SNamespace::apply\n";
     string result = "";
     string command = "kubectl create namespace "+attributes[ATT_NAME]; //TODO can be a list of names
     result = systemCommandList( command, "K8SNamespace", attributes[ATT_NAME], "?", "Create namespace "+attributes[ATT_NAME] );
     LOG( "Create namespace "+attributes[ATT_NAME] );
     return result;
}
string K8SNamespace::destroy(){//TODO rename
     string result = "OK";
     try{
          string name=attributes[ATT_NAME];
          if (name=="")
               return "No namespace found to remove";
          string command = "kubectl delete namespace "+name; //+" &";
          //DEBUG LOG( "command: " + command ); 
          // int ret = system(command.c_str());
          string resultCommand = systemCommand( command );
          LOG( "Destroy namespace "+attributes[ATT_NAME] + ": " + resultCommand );
     }catch(...){
          result = "ERROR K8SNamespace.destroy";
     }
     return result;
}

//TODO This give error:
// error: resource mapping not found for name: "kafka" namespace: "" from "namespace-kafka.tmp.yaml": no matches for kind "Namespace" in version "apps/v1"
// ensure CRDs are installed first
//https://stackoverflow.com/questions/72242713/kubectl-error-upon-applying-agones-fleet-ensure-crds-are-installed-first
string K8SNamespace::getYaml(){
     return (string)
"apiVersion: apps/v1\n"+
"kind: Namespace\n"+
"metadata:\n"+
"  name: "+attributes[ATT_NAME]+"\n"+
"  labels:\n"+
"    app: "+attributes[ATT_NAME]+"\n";
}