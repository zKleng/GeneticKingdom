# === VARIABLES ===
CXX      = g++
CXXFLAGS = -std=c++17 -Iinclude -I/usr/local/opt/sfml/include
LDFLAGS  = -L/usr/local/opt/sfml/lib -lsfml-graphics -lsfml-window -lsfml-system

SRC      = src/main/main.cpp src/map/Map.cpp src/game/enemy.cpp src/game/TestEnemy.cpp
OBJ_DIR  = obj
BIN_DIR  = bin
TARGET   = $(BIN_DIR)/GeneticKingdom

OBJECTS  = $(patsubst src/%.cpp, $(OBJ_DIR)/%.o, $(SRC))

# === REGLA PRINCIPAL ===
all: $(TARGET)

$(TARGET): $(OBJECTS) | $(BIN_DIR)
	$(CXX) $(OBJECTS) -o $@ $(LDFLAGS)

# Compilar cada archivo fuente .cpp a .o en obj/
$(OBJ_DIR)/%.o: src/%.cpp | $(OBJ_DIR)
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Crear directorios si no existen
$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

$(BIN_DIR):
	mkdir -p $(BIN_DIR)

# Limpiar
clean:
	rm -rf $(OBJ_DIR) $(TARGET)
