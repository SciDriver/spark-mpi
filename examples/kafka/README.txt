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

2. Start the PMI server

export HYDRA_PROXY_PORT=55555

/opt/spark-mpi/bin/pmiserv -n 4 hello &

3. Run allreduce_producer.py, check topics

python allreduce_producer.py
kafka-topics.sh --list --zookeeper localhost:2181

4. Submit allreduce_consumer.py

spark-submit ./allreduce_consumer.py localhost:2181

5. Stop the PMI proxy, Kafka and Zookeeper servers

pkill -9 "hydra_pmi_proxy"
kafka-server-stop.sh
zookeeper-server-stop.sh



