#!/bin/sh

echo "Building..."
make build

echo "4 processes"
mpirun --oversubscribe -np 4 lelann

echo "5 processes"
mpirun --oversubscribe -np 5 lelann

echo "8 processes"
mpirun --oversubscribe -np 8 lelann

echo "10 processes"
mpirun --oversubscribe -np 10 lelann

echo "20 processes"
mpirun --oversubscribe -np 20 lelann

echo "30 processes"
mpirun --oversubscribe -np 30 lelann

echo "Cleaning..."
make clean