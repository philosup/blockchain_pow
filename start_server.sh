#!/bin/bash

./main&
sleep 0.5
./main&
sleep 0.5
./main&
sleep 0.5

curl -d '{"difficulty":19, "begin":0, "end":10000000}' -H "Content-Type: application/json" -X POST http://localhost:8080/mine_all
