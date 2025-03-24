#!/bin/bash

# Compilazione dei file C con gcc
echo "Compilazione di insertion.c..."
gcc insertion.c -o insertion.out

echo "Compilazione di merge.c..."
gcc merge.c -o merge.out

# Verifica se la compilazione è andata a buon fine
if [ $? -eq 0 ]; then
    echo "Compilazione completata con successo!"

    # Esegui i file compilati
    echo "Esecuzione di insertion.out..."
    ./insertion.out

    echo "Esecuzione di merge.out..."
    ./merge.out

    # Esegui lo script Python solo dopo aver eseguito i file C
    echo "Esecuzione dello script grafico.py..."
    python3 grafici.py
else
    echo "Errore durante la compilazione. Impossibile eseguire lo script grafico."
fi
