CC = gcc

CFLAGS = -Wall -Wextra -Wpedantic -std=c11 -g
CPPFLAGS = -Iinclude

TARGET = server

SRC = $(wildcard src/*.c)
OBJ = $(SRC:.c=.o)

IMAGE = async-server-dev
CONTAINER = async-server

# ----- C build -----

$(TARGET): $(OBJ)
	$(CC) $(OBJ) -o $(TARGET)

src/%.o: src/%.c
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

all: $(TARGET)

clean:
	rm -f $(OBJ) $(TARGET)


# ----- Docker -----

docker-build:
	docker build -t $(IMAGE) .

docker-shell:
	docker run --rm -it \
		--name $(CONTAINER) \
		-p 8080:8080 \
		-v "$(CURDIR):/app" \
		$(IMAGE)

docker-run:
	docker run --rm -it \
		--name $(CONTAINER) \
		-p 8080:8080 \
		-v "$(CURDIR):/app" \
		$(IMAGE) \
		bash -c "make clean && make && ./$(TARGET)"
