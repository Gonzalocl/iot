docker-compose up

python simulated_arduino.py localhost 2
python simulated_arduino.py localhost 3

python query_lab.py localhost 1
python query_lab.py localhost 2
python query_lab.py localhost 3
python query_lab.py localhost \*

python2 accumulator-server.py --port 1111 --url /alerta --pretty-print -v

python subscribe_lab.py localhost \*

docker container stop sesion5_orion_1 sesion5_mongo_1
