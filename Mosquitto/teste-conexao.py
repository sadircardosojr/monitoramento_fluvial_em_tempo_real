#%%
import paho.mqtt.client as mqtt

USER = 'teste'
PWD = '123'
# Configurações do broker
broker = "localhost"
port = 1883
topic = "teste/topic"
message = "Hello MQTT 4"

# Callback - conexão ao broker
def on_connect(client, userdata, flags, rc):
    if rc == 0:
        print("Conectado ao broker MQTT!")
        # Subscreve ao tópico para testar a comunicação
        client.subscribe(topic)
    else:
        print("Falha na conexão, código de retorno: ", rc)

# Callback - mensagem recebida do servidor
def on_message(client, userdata, msg):
    print(f"Mensagem recebida: {msg.topic} -> {msg.payload.decode('utf-8')}")

# Callback - publicação
def on_publish(client, userdata, mid):
    print("Mensagem publicada!")

# Cria um novo cliente MQTT
client = mqtt.Client()

# Configura a autenticação
client.username_pw_set(USER, PWD)

# Atribui os callbacks
client.on_connect = on_connect
client.on_message = on_message
client.on_publish = on_publish

# Conecta ao broker
client.connect(broker, port, 60)

# Inicia o loop
client.loop_start()

# Publica uma mensagem
client.publish(topic, message)

# Aguarda para garantir que a mensagem foi recebida
import time
time.sleep(2)

# Termina o loop
client.loop_stop()
client.disconnect()
