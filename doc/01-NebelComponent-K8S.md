
# NebelComponent K8S

Is a component to deploy artifacts on the DockerDesktop Kubernetes.

Has not 'name' attribute (because there are only a Kubernetes instance).

For example, the script:

```
    ...
    K8S: 
        [Commands]
        attributes
        <deployments>
        <services>
    .
```


## Deployments:

Use the **Deployment** keyword and give it a 'name' (in this example 'webSErver1').


```
Deployment: webserver1
    image 'nginx:1.14.2'
    containerPort 81
    replicas 2
.
```

### Attributes for deployments
- ** namespace ** The namespace for the deployment.
- ** image ** The image to be deployed.
- ** containerPort ** The port that ear the application.
- ** replicas ** The number of replicas running in parallel.

### Subcomponents


The attributes depend of the deployment, but are necessary: **namespace**, **image**.

## Services
Keyword 'Service' with a name for the instance.

In the body has the attributes for the service:

```
Service: zookeeper
    type NodePort
    port 2181
    nodePort 30181
    targetPort 2181
.
```
