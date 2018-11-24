#!/bin/bash
echo "Starting docker containers"
docker-compose up -d

cd services/frontend
yarn start

cd ../..
