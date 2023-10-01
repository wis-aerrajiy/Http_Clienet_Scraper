NAME = Http

SRC = ./HttpClient.cpp ./Source/HttpExption.cpp ./Source/HttpRequest.cpp ./Source/RequestSender.cpp ./Source/HttpResponse.cpp

OBJ = $(SRC:.cpp=.o)
CC = c++
CFLAGS = -std=c++11 -lssl -lcrypto -fsanitize=address
all: $(NAME)
$(NAME): $(OBJ)
	$(CC) $(OBJ) $(CFLAGS) -o $(NAME)

%.o: %.cpp
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ)
fclean: clean
	rm -f $(NAME)
re: fclean all

.PHONY: all clean fclean re