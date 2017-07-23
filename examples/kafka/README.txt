Kafka-Spark-MPI demo that processes the Kafka topics by
MPI application which runs on Spark workers

--------------------------------------------------------
On a single node
--------------------------------------------------------

1. Start the Zookeeper server (port: 2181) and 
the Kafka server (port: 9092), check topics

zookeeper-server-start.sh zookeeper.properties
kafka-server-start.sh server.properties
kafka-topics.sh --list --zookeeper localhost:2181

2. Define the port of the PMI server

export HYDRA_PROXY_PORT=55555

3. Submit allreduce_consumer.py

spark-submit ./allreduce_consumer.py 

4. Run allreduce_producer.py

python allreduce_producer.py
python allreduce_producer.py

5. Stop the Kafka and Zookeeper servers

kafka-server-stop.sh
zookeeper-server-stop.sh



