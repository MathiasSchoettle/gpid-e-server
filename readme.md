mosquitto conf
    
    Usage: docker run -it -p 1883:1883 -p 9001:9001 -v "$(pwd)"/mosquitto.conf:/mosquitto/config/mosquitto.conf -v /mosquitto/data -v /mosquitto/log eclipse-mosquitto

docker for postgres
    docker run --name gpid -e POSTGRES_PASSWORD=gpid -d -p 5432:5432 postgres