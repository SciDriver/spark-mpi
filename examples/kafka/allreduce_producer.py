from kafka import KafkaProducer

from datetime import timedelta, datetime, tzinfo

import numpy as np
import pickle

producer = KafkaProducer(bootstrap_servers=['localhost:9092'])

partitions = 4

# n = 2*1000000
n = 2*100000

sendbuf = np.arange(n, dtype=np.float32)

for j in range(partitions):
    topic = "topic-" + str(j)
    print(topic)
    for i in range(4):
        sendbuf[n-1] = j+i+5
        raw_bytes = pickle.dumps(sendbuf)    
        producer.send(topic, raw_bytes)
    
