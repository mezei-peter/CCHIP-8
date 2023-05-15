build:
	gcc -Wall -std=c17 ./src/*c -o ./target/emulator

run:
	./target/emulator $(ARGS)

clean:
	rm ./target/emulator