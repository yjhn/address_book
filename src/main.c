#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <signal.h>
#include <unistd.h>

#include <address_book.h>
#include "user_interface.h"

#define PATH_BUFFER_SIZE 512
#define FILE_NAME "addresses.csv"

void sigint_handler(int signum);

void get_file_path(char *buffer, size_t buf_size);
void save_to_file(const struct AddressBook *addresses);
struct AddressBook populate_from_file();

volatile sig_atomic_t keep_running = 1;

int main(void)
{
	struct sigaction sa;
	memset(&sa, 0, sizeof(sa));
	sa.sa_handler = sigint_handler;
	sigaction(SIGINT, &sa, NULL);

	struct AddressBook addr_book = populate_from_file();
	int retval = 0;

	while (keep_running) {
		int status = execute_user_command(&addr_book);
		switch (status) {
		case 1:
			// User asked to exit.
			goto end;
		case -1:
			// Error.
			retval = 1;
			goto end;
		default:
			continue;
		}
	}

end:
	save_to_file(&addr_book);
	delete_address_book(&addr_book);

	return retval;
}

void get_file_path(char *buffer, size_t buf_size)
{
	char *home_dir = getenv("HOME");
	snprintf(buffer, buf_size, "%s/%s", home_dir, FILE_NAME);
}

struct AddressBook populate_from_file()
{
	char file_path[PATH_BUFFER_SIZE];
	get_file_path(file_path, sizeof(file_path));
	struct AddressBook addr_book = create_address_book();

	FILE *file = fopen(file_path, "r");
	if (file == NULL) {
		// Allocate only in this special case to avoid polluting the stack.
		char *error = malloc((sizeof(file_path) + 50) * sizeof(*error));
		sprintf(error, "Failed to open file '%s' for reading",
			file_path);
		perror(error);
		free(error);
		return addr_book;
	}
	addr_book = read_addresses(file);
	fclose(file);

	return addr_book;
}

void save_to_file(const struct AddressBook *addresses)
{
	char file_path[PATH_BUFFER_SIZE];
	get_file_path(file_path, sizeof(file_path));
	FILE *file = fopen(file_path, "w");
	if (file == NULL) {
		// Allocate only in this special case to avoid polluting the stack.
		char *error = malloc((sizeof(file_path) + 50) * sizeof(*error));
		sprintf(error, "Failed to open file '%s' for writing",
			file_path);
		perror(error);
		free(error);
		return;
	}
	save_addresses_to_file(addresses, file);
	fclose(file);
}

void sigint_handler(int signum)
{
	// Avoid unused parameter warning.
	(void)signum;

	const char *msg =
		"CTRL+C pressed, will exit after this action is completed";
	// 1 is stdout.
	write(STDOUT_FILENO, msg, strlen(msg));
	keep_running = 0;
}
