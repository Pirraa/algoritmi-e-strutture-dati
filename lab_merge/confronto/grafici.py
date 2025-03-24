import matplotlib.pyplot as plt
import csv

# Funzione per leggere il file CSV e restituire le dimensioni (size) e i tempi (elapsed_time)
def read_csv(file_path):
    sizes = []
    elapsed_times = []
    
    with open(file_path, mode='r') as file:
        reader = csv.reader(file)
        next(reader)  # Salta l'intestazione
        for row in reader:
            sizes.append(int(row[0]))  # Dimensione dell'array
            elapsed_times.append(float(row[1]))  # Tempo medio di esecuzione
            
    return sizes, elapsed_times

# Funzione per tracciare il grafico
def plot_graph(sizes, elapsed_times, algorithm_name):
    plt.plot(sizes, elapsed_times, marker='o', label=algorithm_name)
    plt.xlabel('Size')
    plt.ylabel('Elapsed Time (seconds)')
    plt.title(f'Performance of {algorithm_name}')
    plt.grid(True)

# Funzione principale
def main():
    # Leggi i dati dai file CSV
    sizes_insertion, elapsed_times_insertion = read_csv('insertion.csv')
    sizes_merge, elapsed_times_merge = read_csv('merge.csv')
    
    # Crea il grafico per Insertion Sort
    plt.figure(figsize=(10, 5))
    plot_graph(sizes_insertion, elapsed_times_insertion, 'Insertion Sort')

    # Aggiungi il grafico per Merge Sort sulla stessa figura
    plot_graph(sizes_merge, elapsed_times_merge, 'Merge Sort')

    # Mostra il grafico con entrambi gli algoritmi
    plt.legend()  # Aggiungi una legenda per distinguere i due algoritmi
    plt.show()

if __name__ == '__main__':
    main()
