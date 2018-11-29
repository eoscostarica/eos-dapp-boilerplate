#!/bin/bash
echo "Building postgres docker volume"
docker volume create --name=postgres-data-volume

echo "Starting docker containers"
docker-compose up -d

# wait 10s for the docker services to start
sleep 10s
cd services/frontend
yarn start

cd ../..
