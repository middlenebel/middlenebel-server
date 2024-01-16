


# Port Forwarding

**Connection with MySQL on K8S**
If MySQL is installed in namespace **mysql-nebel** and was deployed with name **mysql-nebel** (name of the pods)

kubectl port-forward `kubectl get pod -l app=mysql-nebel -n mysql-nebel -o jsonpath={.items[0].metadata.name}` 3306 -n mysql-nebel &