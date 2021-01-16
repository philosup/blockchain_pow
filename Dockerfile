# Dockerfile

FROM ubuntu:20.04 AS complier

RUN apt update &&  apt install -y g++ 

WORKDIR /tmp

COPY . /tmp
RUN g++ -g main.cpp -o main -lpthread



FROM ubuntu:20.04
RUN apt update &&  apt install -y curl 

RUN mkdir -p /work
WORKDIR /work

COPY --from=complier /tmp/main main
COPY --from=complier /tmp/start_server.sh start_server.sh
COPY --from=complier /tmp/stop_server.sh stop_server.sh
RUN chmod 755 start_server.sh
RUN chmod 755 stop_server.sh