build:
	gcc -Wall -std=c17 ./src/*c -o ./target/emulator

run:
	./target/emulator

clean:
	rm ./target/emulator