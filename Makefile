# Makefile for TCP- CCA Assignment3

all: Receiver Sender

Receiver: Receiver.c
	gcc Receiver.c -o Receiver

Sender: Sender.c
	gcc Sender.c -o Sender

clean:
	rm -f *.o Receiver Sender

runs:
	./Receiver

runc:
	./Sender

runs-strace:
	strace -f ./Sender

runc-strace:
	strace -f ./Receiver