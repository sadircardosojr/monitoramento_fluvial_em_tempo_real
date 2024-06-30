FROM confluentinc/cp-kafka-connect:latest
USER root
# Baixar e instalar o JDK 8
RUN wget --no-check-certificate --no-cookies --header "Cookie: oraclelicense=accept-securebackup-cookie" http://download.oracle.com/otn-pub/java/jdk/8u141-b15/336fa29ff2bb4ef291e347e091f7f4a7/jdk-8u141-linux-x64.rpm \
    && yum install -y jdk-8u141-linux-x64.rpm \
    && rm jdk-8u141-linux-x64.rpm

# Baixar o Confluent Community 5.3.1
RUN wget http://packages.confluent.io/archive/5.3/confluent-community-5.3.1-2.12.tar.gz \
    && tar -xvf confluent-community-5.3.1-2.12.tar.gz \
    && rm confluent-community-5.3.1-2.12.tar.gz

# Conceder permissões de leitura e escrita ao diretório /var/log
RUN chmod ugo+rwx /var/log

# Baixar e extrair o Confluent Hub Client
RUN wget http://client.hub.confluent.io/confluent-hub-client-latest.tar.gz \
    && tar -xvf confluent-hub-client-latest.tar.gz -C confluent-5.3.1/ \
    && rm confluent-hub-client-latest.tar.gz

# Instalar o conector MQTT usando o Confluent Hub
RUN ./confluent-5.3.1/bin/confluent-hub install --no-prompt confluentinc/kafka-connect-mqtt:latest

# Instalar o conector com Clickhouse
#RUN confluent-hub install --no-prompt clickhouse/clickhouse-sink:1.1.0
#RUN curl -sSL "https://github.com/ClickHouse/clickhouse-kafka-connect/releases/download/v0.2.2/clickhouse-kafka-sink-0.2.2-jar-with-dependencies.jar" -o /usr/share/java/clickhouse-sink.jar
# Definir a variável de ambiente para o caminho do plugin
ENV CONNECT_PLUGIN_PATH="/usr/share/java,/usr/local/confluent-hub/share/java"

# Expor a porta do Kafka Connect REST
EXPOSE 8083
