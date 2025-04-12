import matplotlib.pyplot as plt

# leggo tutte le righe del file contenente i risultati degli esperimenti
with open("report.txt", "r") as file:
    lines = file.readlines()

# la primissima riga del file (lines[0]) rappresenta l'asse x;
# anzitutto, splittiamo la stringa (e.g., "10 20 30" diventa una lista ["10", "20", "30"]);
# poi applichiamo il cast ad intero ad ogni elemento della lista (con map)
x_data = list(map(int, lines[0].split()))

# raccogliamo anche tutte le righe successive;
# y[0] contiene tutti i tempi del primo algoritmo, y[1] del secondo e così via.
y_data = [list(map(float, line.split())) for line in lines[1:]]

# etichette da stampare sul grafico
algos = ["INSERTION", "MERGE", "HYBRIDMERGE", "QUICK", "MOTQUICK", "TAILQUICK", "HEAPSORT", "INTROSORT"]

# creiamo un plot per ogni lista in y_data
for i, time in enumerate(y_data):
    plt.plot(x_data, time, label=algos[i])

plt.xlabel("Dimensione")
plt.ylabel("Tempo trascorso [s]")
plt.title("Esperimenti sugli ordinamenti")
plt.legend()
plt.show()