# Networks
## TCP CC<br>
In this Ex I wrote two program files: Sender.c and Receiver.c. <br> 
The Sender is sending a file and the Receiver receive it and measure the time it took for his program to receive the file.<br>
The file sent in two parts (first half and second half [Each half is 50% of the file]) each half sent according to one of the CC algorithms- Cubic or Renu.<br>
I run the program with a packet lost tool in the following levels:<br>
0% lost, 10% lost, 15% lost, 20% lost.
