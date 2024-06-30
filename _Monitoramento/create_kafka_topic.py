import requests
import json

url = "http://localhost:8083/connectors"

payload = json.dumps({
  "name": "mqtt-source-connector",
  "config": {
    "connector.class": "io.confluent.connect.mqtt.MqttSourceConnector",
    "tasks.max": "1",
    "mqtt.server.uri": "tcp://mosquitto:1883",
    "mqtt.topics": "estacao/meteorologica",
    "kafka.topic": "teste",
    "mqtt.qos": "2",
    "mqtt.username": "teste",
    "mqtt.password": "123",
    "value.converter": "org.apache.kafka.connect.converters.ByteArrayConverter",
    "confluent.topic.bootstrap.servers": "kafka:9092",
    "confluent.topic.replication.factor": "1"
  }
})
headers = {
  'Content-Type': 'application/json'
}

response = requests.request("POST", url, headers=headers, data=payload)

print(response.text)
