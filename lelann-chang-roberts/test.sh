#!/bin/sh

echo "Building..."
make build

echo "4 processes"
mpirun --oversubscribe -np 4 lelann-chang-roberts

echo "5 processes"
mpirun --oversubscribe -np 5 lelann-chang-roberts

echo "8 processes"
mpirun --oversubscribe -np 8 lelann-chang-roberts

echo "10 processes"
mpirun --oversubscribe -np 10 lelann-chang-roberts

echo "20 processes"
mpirun --oversubscribe -np 20 lelann-chang-roberts

echo "30 processes"
mpirun --oversubscribe -np 30 lelann-chang-roberts

echo "Cleaning..."
make clean