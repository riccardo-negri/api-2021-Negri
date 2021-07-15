# Progetto Algoritmi e Strutture Dati 2020-2021
[Link Verificatore](https://dum-e.deib.polimi.it/)

Comando per compilare (C11/gcc) :
```shell
/usr/bin/gcc -DEVAL -Wall -Werror -std=gnu11 -O2 -pipe -static -s -o main main.c -lm
```

Utilizzare Valgrind (compilare il sorgente con il flag ```-g3```) perrilevare errori nell’uso della memoria dinamica,
come accessi ad aree non allocate, e i leaks di memoria, ovvero situazioni in cui non viene
liberata memoria precedentemente allocata e non più referenziata:
```shell
valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./main
```

Callgrind è un altro tool di Valgrind che consente di visualizzare il tempo impiegato nelle
diverse funzioni/istruzioni del programma:
```shell
valgrind --tool=callgrind --callgrind-out-file=outputcallgrind ./main
```
Per analizzarlo in kcachegrind:
```shell
kcachegrind outputcallgrind
```

Mass-If è un altro tool di valgrind che consente di visualizzare l’andamento della memoria
allocata nel corso dell’esecuzione:
```shell
valgrind --tool=massif --massif-out-file=outputfile ./main
```
Per visualizzarlo in massif-visualizer:
```shell
massif-visualizer outputfile
```

AddressSanitizer (aka ASan) è un memory error detector per C/C++.
Per utilizzarlo bisogna compilare il programma usando gcc con il flag ```-fsanitize=address``` (oltre a quelli riportati sopra).
