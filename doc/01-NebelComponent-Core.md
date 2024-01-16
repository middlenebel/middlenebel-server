

# NebelComponent Core

Main object of Middlenebel system. Runs in the back-end of MibbleNebel-Lab system.

Load the current script (by default scripts/middlenebel.nebel) and parse it.

Make and system trre of NebelComponents for the infrastructure.

Execute an HTTP server over localhost:8080 with the next endPoints:

- /status
- /components
    + Return a tree of NebelComponents in JSON format beggining by Core component.
- /play
    + Deploy the infrastructure on targets systems.
    + If has NebelComponents (like K8S or K8S-Kafka) deploy them on Kubernetes of DockerDesktop.
- /destroy
    + Destroy the infrastructure. 
    + If deployments, services or namespaces was created on Kubernetes become deleted.
- /quit
    + Exit from the server and close the back-end.
- /reload
    + Delete from memory al NebelComponents and parse againg the nebel script and restart the HTTP Server
- /save-script
    + Save the content of the script from the Front-End.
- /browserReload
    + Reload and return the content of "scripts" folder and subfolders.
- /browserAction
    + Executa file and folder actions: addFolder, addFile, delete, rename, loadScript.
- /getLog
    + Return the content of Back-End log (the server).
- /executeAction
    + Execute an plugin action, depends of each plugin capabilities.


