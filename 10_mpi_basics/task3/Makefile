  GNU nano 6.2                                                                                         Makefile                                                                                                  # kompilator c
CCOMP = mpicc

# konsolidator
LOADER = mpicc

# program uruchomieniowy MPI
MPI_run = mpiexec -oversubscribe

# opcje optymalizacji:
# wersja do debugowania
OPT = -g -DDEBUG
# wersja zoptymalizowana do mierzenia czasu
# OPT = -O3

# pliki naglowkowe
#INC = -I../pomiar_czasu

# biblioteki
#LIB = -L../pomiar_czasu -lm
LIB = -lm

all: MPI_structure run

run: MPI_structure
        $(MPI_run) -np 5 ./MPI_structure

# zaleznosci i komendy
MPI_structure : MPI_structure.o
        $(LOADER) $(OPT) MPI_structure.o -o MPI_structure $(LIB)

# jak uzyskac plik moj_program.o ?
MPI_structure.o: MPI_structure.c
        $(CCOMP) -c $(OPT) MPI_structure.c

clean:
        rm -f *.o MPI_structure

