
all: clean soup

clean:
	rm -rf soup

soup: soup.cpp
	$(CC) -o soup soup.cpp
	
