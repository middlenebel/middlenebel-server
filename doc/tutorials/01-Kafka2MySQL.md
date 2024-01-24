

# Kafka 2 MySQL Tutorial

The main goal of this tutorial is learn about Spring Kafka java programming.

Suposse that you have a Kafka server with a topic and you want to make a consumer that read messages from the topic and insert them in a MySQL database.

We can follow for example this tutorial about java Kafka Spring programming:

https://spring.io/projects/spring-kafka/

Then we will need a Kafka server running for development.

We can use this Nebel script with the Middlenebel tool to manage our platform:

```
Platform: Kafka2MySQL
    DockerDesktop.
    K8S:
        CreateNamespaces: kafka2mysql.
    .
    K8S-Kafka: kafka1
        namespace kafka2mysql
        PortForward: kafka-broker 9092 kafka2mysql.
        KProducer
        KConsumer
    .
.

```
Note that you need edit hosts in:
Windows: C:\Windows\System32\drivers\etc\hosts
WSL/Linux: /etc/hosts

# For write messages
KafkaTemplate is auto-configured, and you can autowire it directly... (prefered in the constructor)
```
@Component
public class MyBean {

    private final KafkaTemplate<String, String> kafkaTemplate;

    public MyBean(KafkaTemplate<String, String> kafkaTemplate) {
        this.kafkaTemplate = kafkaTemplate;
    }

    // ...

}
```
