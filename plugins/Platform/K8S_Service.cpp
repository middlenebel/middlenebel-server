
#include <cstdlib>
#include <iostream>
#include <string>

#include "../../inc/Core.hpp"
#include "../../inc/Component.hpp"
#include "../../inc/Util.hpp"
#include "../../incPlugins/K8S.hpp"
#include "../../incPlugins/K8S_Service.hpp"

K8SService::K8SService(Component* parent):Component(parent){
     init();
}

void K8SService::init(){
     attributes["className"]="K8SService";
     attributes["name"]="Undef";
}

string K8SService::getJsonComponent(){
     //DEBUG LOG( "K8S Service getJsonComponent..." );
     string components="{";
     components+=JSON_PROPERTY("name", attributes["name"])+",\n";
     string json_attributes = "";
     MAP_TO_STRING(attributes, json_attributes);
     components+=JSON_ARRAY("attributes", json_attributes )+"\n";
     components+="}";
     return components;
}

string K8SService::apply(){
     string result = "OK";
     string yaml = getYaml();
     //DEBUG LOG( "YAML:\n" + yaml + "\n";
     string fileName="./outyamls/service-"+attributes["name"]+".tmp.yaml";
     string nameSpace=getAtt("namespace", "default");
     string command = (string)"kubectl apply -f "+fileName+" -n "+nameSpace; //+" &";
     try{
          ofstream yamlFile(fileName);
          yamlFile << yaml;
          yamlFile.close();
          // int ret = system(command.c_str());
          string resultCommand = systemCommand( command );
          LOG( "Apply service "+attributes["name"] + ": " + resultCommand );
     }catch(...){
          result = "ERROR";
     }
     // string command = "kubectl create namespace "+name;
     // int ret = system(command.c_str());
     return result;
}
string K8SService::destroy(){
     string result = "OK";
     try{
          string name=attributes["name"];
          string nameSpace=getAtt("namespace", "default");
          string command = (string)"kubectl delete service "+name+" -n "+nameSpace; //+" &";
          //DEBUG LOG( "command: " + command ); 
          // int ret = system(command.c_str());
          string resultCommand = systemCommand( command );
          LOG( "Destroy service "+attributes["name"] + ": " + resultCommand );
     }catch(...){
          result = "ERROR";
     }
     return result;
}
//TODO This give error:
// error: resource mapping not found for name: "kafka" namespace: "" from "namespace-kafka.tmp.yaml": no matches for kind "Namespace" in version "apps/v1"
// ensure CRDs are installed first
//https://stackoverflow.com/questions/72242713/kubectl-error-upon-applying-agones-fleet-ensure-crds-are-installed-first
string K8SService::getYaml(){
     return (string)
"apiVersion: v1\n"+
"kind: Service\n"+
"metadata:\n"+
"  labels:\n"+
"    app: "+attributes["name"]+"\n"+
"  name: "+attributes["name"]+"\n"+
"spec:\n"+
"  type: "+attributes["type"]+"\n"+
"  ports:\n"+
"    - name: "+attributes["name"]+"\n"+
"      port: "+attributes["port"]+"\n"+
"      nodePort: "+attributes["nodePort"]+"\n"+
"      targetPort: "+attributes["targetPort"]+"\n"+
"  selector:\n"+
"    app: "+attributes["name"]+"\n";
}
