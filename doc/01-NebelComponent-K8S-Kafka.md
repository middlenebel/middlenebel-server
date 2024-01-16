
# NebelComponent K8S-Kafka

Is a component to deploy a Kafka server on Kubernetes.

Has 'name' attribute.


## Commands
Can be:

- **PortForward** Start a PortForwarding for kafka-broker:9092. 
    + Needs in /etc/hosts an entry like: '127.0.0.1 kafka-broker'


## Actions:
- **KProducer** Sends a message to the default topic.
- **KConsumer** Reads a message from the default topic.