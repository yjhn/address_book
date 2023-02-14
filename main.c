#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "address_book.c"

#define PATH_BUFFER_SIZE 512
#define FILE_NAME "addresses.csv"

int main(void)
{
	char *home_dir = getenv("HOME");
	char file_path[PATH_BUFFER_SIZE];
	snprintf(file_path, PATH_BUFFER_SIZE, "%s/%s", home_dir, FILE_NAME);

	struct AddressBook addr_book = create_address_book();

	FILE *file = fopen(file_path, "r");
	if (file == NULL) {
		// Allocate only in this special case to avoid polluting the stack.
		char *error = malloc((PATH_BUFFER_SIZE + 22) * sizeof(*error));
		snprintf(error, PATH_BUFFER_SIZE + 22,
			 "Failed to open file '%s'", file_path);
		perror(error);
		free(error);
	} else {
		addr_book = read_addresses(file);
	}
	display_addresses(&addr_book);

	delete_address_book(&addr_book);
	fclose(file);

	return 0;
}
