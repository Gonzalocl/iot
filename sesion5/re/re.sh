nc -lp 1026 > req
python simulated_arduino.py localhost 2

# start fiware
nc localhost 1026 < req > rep
# stop fiware

for i in $(seq 0 9); do
  echo "$i"
  nc -lp 1026 < rep > "req$i"
done
python simulated_arduino.py localhost 2

rm -f rep req*
