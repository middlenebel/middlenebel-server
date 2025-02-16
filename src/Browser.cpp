
#include <filesystem>
#include <jsoncpp/json/json.h>

#include "inc/Browser.hpp"
#include "inc/utils/Util.hpp"

#define LOG_COUT( A ) ( cout << A << "\n" )

namespace fs = std::filesystem;

Browser::Browser(){
    //newFileName = "";
    fileName = "";
}
       
string Browser::getBrowserReload(string base){
    string json = "";
    string separator = "";
    for(const fs::directory_entry& entry : fs::directory_iterator("./scripts" + base)) {
        string name = entry.path().filename().string();
cout << "DEBUG: Browser.Item " << name << "\n";
        json += separator;
        if ( is_regular_file( entry ) ){
            string baseFolder = (base=="") ? "/" : base;
            json += (string) "{" + 
                "\"name\":\"" + name +"\","+
                "\"base\":\"" + baseFolder +"\""+"}";

        } else if (is_directory( entry )) {

            std::string pathName = base + "/" + name; // Path to the directory

            string folderContent = getBrowserReload( pathName );
            if (folderContent=="")
                folderContent="{\"name\":\".\",\"base\":\""+ pathName+ "\"}";
                
            json += (string) "{" + 
                "\"name\":\"" + name +"\","+
                "\"base\":\"" + pathName +"\","+
                "\"children\":["+ folderContent +"]"+
            "}";

        }
        separator = ",";
    }
    return json;
}

string Browser::doBrowserAction(string json){
    string result = "KO";
    string message = "Server ERROR!";
    string script = "";

    Json::Value value;
    Json::Reader reader;
    bool isOk = reader.parse( json, value );
    if (isOk){
        const Json::Value actionValue = value["action"];
        string actionStr = actionValue.asString();

        const Json::Value baseValue = value["base"];
        string baseStr = baseValue.asString();

        const Json::Value newValue = value["newValue"];
        string newValueStr = newValue.asString();

        const Json::Value nameValue = value["value"];
        string nameStr = nameValue.asString();

        const Json::Value isFolderValue = value["isFolder"];
        bool isFolder = isFolderValue.asBool();

        string path = "./scripts"+baseStr;
        string pathTo = "./scripts"+baseStr;
        if (actionStr == "addFolder"){
            path += ((baseStr!="/")?"/":"") + newValueStr;
            fs::create_directory(path);
            result = "OK";
            message = (string)"Folder "+path+" created!";
        }else if (actionStr == "delete"){
            if (!isFolder){
                path += ((baseStr!="/")?"/":"") + nameStr;
            }
            fs::remove_all(path); // TODO delete all script folder
            result = "OK";
            message = (string)"Folder "+path+" deleted!";
        }else if (actionStr == "addFile"){
            if (!isFolder){
                path += ((baseStr!="/")?"/":"") + newValueStr;
            }
            std::fstream f(path, std::ios::out);
            f.close();
            result = "OK";
            message = (string)"Folder "+path+" deleted!";
        }else if (actionStr == "rename"){
            //if (!isFolder){
                path += ((baseStr!="/")?"/":"") + nameStr;
            //}
            pathTo += ((baseStr!="/")?"/":"") + newValueStr;
            LOG_COUT("Rename from "+path+" to "+pathTo);
            try{
                fs::rename(path, pathTo);
                result = "OK";
                message = (string)"Folder "+path+" renamed to "+pathTo;
            }catch(const std::exception& ex){
                result = "KO";
                message = (string)"Exception "+ex.what();
                LOG_COUT("Exception "+message);
            }
        }else if (actionStr == "loadScript"){
            if (isFolder){
                message = (string)"A folder can't be loaded as script!";
            }else{
                path += ((baseStr!="/")?"/":"") + nameStr;
                //this->newFileName = path;
                this->fileName = path;
                script = Util::loadFile(this->fileName);
            }
            result = "OK";
            message = (string)"Script "+path+" loaded!";
        }
    }

    return (string) "{ \"result\" : \""+ result +"\" , \"message\" : \""+ message +"\" , \"script\" : \""+ script +"\" }";
}



