
#include <cstdlib>
#include <iostream>
#include <string>

#include "../../inc/Core.hpp"
#include "../../inc/Component.hpp"
#include "../../inc/Util.hpp"
#include "../../incPlugins/K8S.hpp"
#include "../../incPlugins/K8S_Deployment.hpp"

K8SDeployment::K8SDeployment(Component* parent):Component(parent){
     init();
}

void K8SDeployment::init(){
     attributes[ATT_CLASSNAME]="K8SDepoyment";
     attributes[ATT_NAME]="Undef";
}

string K8SDeployment::getJsonComponent(){
     //DEBUG LOG( "K8S Deployment getJsonComponent..." );
     string components="{";
     components+=JSON_PROPERTY(ATT_NAME, attributes[ATT_NAME])+",\n";
     // string json_attributes = "";
     // MAP_TO_STRING(attributes, json_attributes);
     // components+=JSON_ARRAY("attributes", json_attributes )+"\n";
     components+=JSON_ARRAY("attributes", getJsonAttributes() )+"\n";
     components+="}";
     return components;
}

string K8SDeployment::apply(){
     string result = "OK";
     try{
          string yaml = getYaml();
          //DEBUG LOG( "YAML:\n" + yaml + "\n";
          string fileName="./outyamls/deploy-"+attributes[ATT_NAME]+".tmp.yaml";
          string nameSpace=getAtt(ATT_NAMESPACE, "default");
          string command = "kubectl apply -f "+fileName+" -n "+nameSpace; //+" &";
          
          ofstream yamlFile(fileName);
          yamlFile << yaml;
          yamlFile.close();
          // int ret = system(command.c_str());
          string resultCommand = systemCommand( command );
          LOG( "Apply deployment "+attributes[ATT_NAME] + ": " + resultCommand );
     }catch(...){
          result = "ERROR";
     }
     return result;
}
string K8SDeployment::destroy(){
     string result = "OK";
     try{
          string name=attributes[ATT_NAME];
          string nameSpace=getAtt(ATT_NAMESPACE, "default");
          string command = "kubectl delete deployment "+name+" -n "+nameSpace; //+" &";
          //DEBUG LOG( "command: " + command ); 
          // int ret = system(command.c_str());
          string resultCommand = systemCommand( command );
          LOG( "Destroy deployment "+attributes[ATT_NAME] + ": " + resultCommand );
     }catch(...){
          result = "ERROR";
     }
     return result;
}

string K8SDeployment::getYaml(){
     string yaml = (string)
"apiVersion: apps/v1\n"+
"kind: Deployment\n"+
"metadata:\n"+
"  name: "+attributes[ATT_NAME]+"\n"+
"  labels:\n"+
"    app: "+attributes[ATT_NAME]+"\n"+
"spec:\n"+
"  replicas: "+getAtt("replicas","1")+"\n"+
"  selector:\n"+
"    matchLabels:\n"+
"      app: "+attributes[ATT_NAME]+"\n"+
"      id: \""+getAtt("id","0")+"\"\n"+
"  template:\n"+
"    metadata:\n"+
"      labels:\n"+
"        app: "+attributes[ATT_NAME]+"\n"+
"        id: \""+getAtt("id","0")+"\"\n"+
"    spec:\n"+
"      hostname: "+getAtt("hostname","localhost")+"\n"+
"      containers:\n"+
"      - name: "+attributes[ATT_NAME]+"\n"+
"        image: "+attributes["image"]+"\n"+
"        imagePullPolicy: IfNotPresent\n"+
"        ports:\n"+
"        - containerPort: "+attributes["containerPort"]+"\n";

     if (this->env.size() > 0){
          yaml += (string) "        env:\n";
/*
"    spec:\n"+ 
"      - env:\n"+
"        - name: KAFKA_BROKER_ID
"          value: "1"
*/
          for (std::map<string,string>::iterator child =this->env.begin(); child != this->env.end(); ++child){ \
               yaml += (string) "        - name: " + (child->first) + "\n";
               yaml += (string) "          value: \"" + (child->second) + "\"\n";
          } 
     }
     return yaml;
}