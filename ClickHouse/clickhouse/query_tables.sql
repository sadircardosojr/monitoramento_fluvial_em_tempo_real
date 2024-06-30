####################################################################################
#       Consultas para facilitar criação do banco de dados e conexão com Kafka.
####################################################################################


# Cria a base de dados de monitoramento..
CREATE database monitoramento;

# Criar a tabela que permite a leitura de dados do kafka através do Clickhouse
CREATE TABLE monitoramento.estacao
(
    `estacao` String,
    `latitude` Float64,
    `longitude` Float64,
    `altura` Float64,
    `temperatura` Float64,
    `umidade` Float64
)
ENGINE = Kafka(
   'kafka:9092', -- Deve ser o nome do serviço Kafka no Docker Compose
   'estacao', -- Nome do tópico Kafka
   'kafka-connect-group', -- Nome do grupo de consumidores Kafka Connect
   'JSONEachRow'
);




# Consulta os dados através do clickhouse na tabela criada
select * from monitoramento.estacao
settings stream_like_engine_allow_direct_select = 1;


# Criar a tabela MergeTree com as novas colunas
CREATE TABLE monitoramento.estacao_data
(
    `estacao` String,
    `latitude` Float64,
    `longitude` Float64,
    `altura` Float64,
    `temperatura` Int32,
    `umidade` Int32,
    `sensacao_termica` Float64,
    `timestamp` DateTime DEFAULT now()
)
ENGINE = MergeTree()
ORDER BY timestamp;

#  Criar a Materialized View para calcular e inserir os dados
CREATE MATERIALIZED VIEW monitoramento.estacao_view TO monitoramento.estacao_data AS
SELECT
    estacao,
    latitude,
    longitude,
    altura,
    temperatura,
    umidade,
    temperatura - 0.55 * (1 - umidade / 100) * (temperatura - 14.5) AS sensacao_termica,
    now() as timestamp
FROM monitoramento.estacao;

SELECT * FROM monitoramento.estacao_view