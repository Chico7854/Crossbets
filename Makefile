# Compilador
CC = gcc

# Executável de saída
OUTPUT = cassino

# Arquivos fonte
SOURCES = main.c \
           blackjack/blackjack.c \
           roulette/roulette.c \
           baccarat/baccarat.c\
           common.c

# Arquivos de cabeçalho
HEADERS = blackjack/blackjack.h \
           roulette/roulette.h \
           baccarat/baccarat.h \
           common.h

# Compila o executável
$(OUTPUT): $(SOURCES) $(HEADERS)
	$(CC) $(SOURCES) -o $(OUTPUT)

# Limpa os arquivos de compilação
clean:
	rm -f $(OUTPUT)
