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
def plot_graph(sizes, elapsed_times):
    plt.plot(sizes, elapsed_times, marker='o', color='b')
    plt.xlabel('Size')
    plt.ylabel('Elapsed Time (seconds)')
    plt.title('Performance of Insertion Sort')
    plt.grid(True)
    plt.show()

# Funzione principale
def main():
    # Leggi i dati dal file CSV
    sizes, elapsed_times = read_csv('experiment_results.csv')
    
    # Traccia il grafico
    plot_graph(sizes, elapsed_times)

if __name__ == '__main__':
    main()
