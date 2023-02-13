#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#define PATH_BUFFER_SIZE 512
#define FILE_NAME "addresses.csv"

#define LINE_BUFFER_SIZE 1024

struct Address {
	char *name;
	char *surname;
	char *email;
	char *phone;
};

struct ListElement {
	struct Address data;
	struct ListElement *next;
};

struct AddressBook {
	struct ListElement *head;
	size_t size;
};

struct AddressBook read_addresses(FILE *file);
struct Address parse_line(char *line);
void display_addresses(const struct *AddressBook);
void add_address_to_end(struct *AddressBook);
void add_address(struct *AddressBook, size_t pos);

int main(void)
{
	char *home_dir = getenv("HOME");
	char file_path[PATH_BUFFER_SIZE];
	snprintf(file_path, PATH_BUFFER_SIZE, "%s/%s", home_dir, FILE_NAME);

	struct AddressBook addr_book {
		.head = NULL, .size = 0
	};

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

	fclose(file);

	return 0;
}

// Constructs an address book from addresses in the given file.
struct AddressBook read_addresses(FILE *file)
{
	struct AddressBook addresses {
		.head = NULL, .size = 0
	};

	char line[LINE_BUFFER_SIZE];
	if (fgets(line, LINE_BUFFER_SIZE, file) == NULL) {
		return NULL;
	}
	size_t len = strlen(line);
	if (line[len - 1] == '\n') {
		line[len - 1] = '\0';
	}

	struct Address addr = parse_line(line);
	struct ListElement *last = malloc(sizeof(*last));
	last->data = addr;
	last->next = NULL;
	addresses.head = last;
	addresses.size += 1;

	while (fgets(line, LINE_BUFFER_SIZE, file) != NULL) {
		size_t len = strlen(line);
		if (line[len - 1] == '\n') {
			line[len - 1] = '\0';
		}

		struct Address addr = parse_line(line);
		struct ListElement *elem = malloc(sizeof(*elem));
		elem->data = addr;
		elem->next = NULL;
		last->next = elem;
		addresses.size += 1;
	}

	return addresses;
}

// Creates an address from a single CSV line.
struct Address parse_line(char *line)
{
	char *name_ref = strtok(line, ",");
	// strdup is in POSIX and C23
	char *name = strdup(name_ref);

	char *surname_ref = strtok(NULL, ",");
	char *surname = strdup(surname_ref);

	char *email_ref = strtok(NULL, ",");
	char *email = strdup(email_ref);

	char *phone_ref = strtok(NULL, ",");
	char *phone = strdup(phone_ref);

	struct Address addr = {
		.name = name, .surname = surname, .email = email, .phone = phone
	};
	return addr;
}

// Displays all records in CSV format.
void print_addresses(const struct ListElement *head)
{
	printf("%15s\t%15s\t%30s\t%12s\n", "NAME", "SURNAME", "EMAIL", "PHONE");
	while (head != NULL) {
		struct Address record = head->data;
		printf("%s,%s,%s,%s\n", record.name, record.surname,
		       record.email, record.phone);
		head = head->next;
	}
}
