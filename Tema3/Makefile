# APD - Tema 1
# Octombrie 2021

build:
	@echo "Building..."
	@mpicc -g tema3.c -o tema3 -lm
	@echo "Done"

clean:
	@echo "Cleaning..."
	@rm -rf tema3
	@echo "Done"
run:
	@mpirun -np 9 --oversubscribe ./tema3 20
