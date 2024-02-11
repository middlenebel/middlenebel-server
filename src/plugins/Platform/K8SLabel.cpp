
#include <cstdlib>
#include <iostream>
#include <string>

#include "../../inc/Core.hpp"
#include "../../inc/utils/Util.hpp"
#include "../../incPlugins/K8S.hpp"
#include "../../incPlugins/K8SLabel.hpp"

K8SLabel::K8SLabel(Component* parent):Component(parent){
}

string K8SLabel::getJsonLabels(){
     //DEBUG     
     LOG( "K8S Labels..." ); //TODO why not core directly. 
     string json_labels = "";
     MAP_TO_STRING(labels, json_labels);
     return json_labels;
}

//TODO
string K8SLabel::getYamlLabels(){
     return (string)
"  labels:\n";
// +
// "    app: "+name+"-label\n"+
// "  name: "+name+"-label\n"+
// "  namespace: "+attributes[ATT_NAMESPACE]+"\n"+
// "spec:\n"+
// "  type: "+attributes["type"]+"\n"+
// "  ports:\n"+
// "    - name: "+name+"-port\n"+
// "      port: "+attributes[ATT_PORTRTRT]+"\n"+
// "      nodePort: "+attributes["nodePort"]+"\n"+
// "      targetPort: "+attributes["targetPort"]+"\n"+
// "  selector:\n"+
// "    app: "+name+"\n";
}
