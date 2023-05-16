build:
	gcc -Wall -std=c17 ./src/*c -lSDL2 -o ./target/emulator

run:
	./target/emulator $(ARGS)

clean:
	rm ./target/emulator