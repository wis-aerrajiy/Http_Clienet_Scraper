NAME = Http

SRC = ./HttpClient.cpp ./Source/HttpExption.cpp ./Source/HttpRequest.cpp ./Source/RequestSender.cpp

OBJ = $(SRC:.cpp=.o)
CC = c++
CFLAGS =  -std=c++11
all: $(NAME)
$(NAME): $(OBJ)
	$(CC) $(CFLAGS) -o $(NAME) $(OBJ)
	# clear

%.o: %.cpp
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ)
fclean: clean
	rm -f $(NAME)
re: fclean all

.PHONY: all clean fclean re