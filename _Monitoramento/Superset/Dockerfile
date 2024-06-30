FROM apache/superset:4.0.1

USER root

RUN apt-get update -y
RUN apt-get upgrade -y
RUN apt install git -y
RUN apt install htop -y


RUN pip install --upgrade pip 

RUN pip install 'gunicorn==21.2.0'
RUN pip install "packaging<24,>=21"


COPY config/local_requirements.txt .
RUN pip install -r local_requirements.txt

COPY config/superset_config.py /app/pythonpath/superset_config.py

ENV SUPERSET_CONFIG_PATH /app/pythonpath/superset_config.py

USER superset
