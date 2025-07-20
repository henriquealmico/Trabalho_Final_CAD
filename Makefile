# Compiladores e flags
CC      = gcc
MPICC   = mpicc
CFLAGS  = -O2 -Wall

# Diretórios de cada implementação
SERIAL_DIR  = serial
OPENMP_DIR  = openmp
MPI_DIR     = mpi

# Nome dos executáveis
SERIAL_EXE  = $(SERIAL_DIR)/odd_even_serial
OPENMP_EXE  = $(OPENMP_DIR)/odd_even_openmp
MPI_EXE     = $(MPI_DIR)/odd_even_mpi

.PHONY: all serial openmp mpi clean test

all: serial openmp mpi

# --------------------------------------------------
# 1) Build serial
# --------------------------------------------------
serial: $(SERIAL_EXE)

$(SERIAL_EXE): $(SERIAL_DIR)/odd_even_serial.c
	@echo "Compilando versão serial..."
	$(CC) $(CFLAGS) -o $@ $<

# --------------------------------------------------
# 2) Build OpenMP
# --------------------------------------------------
openmp: $(OPENMP_EXE)

$(OPENMP_EXE): $(OPENMP_DIR)/odd_even_openmp.c
	@echo "Compilando versão OpenMP..."
	$(CC) $(CFLAGS) -fopenmp -o $@ $<

# --------------------------------------------------
# 3) Build MPI
# --------------------------------------------------
mpi: $(MPI_EXE)

$(MPI_EXE): $(MPI_DIR)/odd_even_mpi.c
	@echo "Compilando versão MPI..."
	$(MPICC) $(CFLAGS) -o $@ $<

# --------------------------------------------------
# Limpeza de binários
# --------------------------------------------------
clean:
	@echo "Removendo executáveis..."
	-rm -f $(SERIAL_EXE) $(OPENMP_EXE) $(MPI_EXE)

# --------------------------------------------------
# Teste rápido de execução
# --------------------------------------------------
test: all
	@echo "\n=== Teste serial ==="
	@$(SERIAL_EXE) 1000
	@echo "\n=== Teste OpenMP ==="
	@$(OPENMP_EXE) 1000 2 static 1000
	@echo "\n=== Teste MPI ==="
	@mpirun -np 2 $(MPI_EXE) 1000
