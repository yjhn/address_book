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
void display_addresses(const struct AddressBook *addresses);
void add_address_to_end(struct AddressBook *addresses,
			const struct Address addr);
_Bool add_address(struct AddressBook *addresses, const size_t pos,
		  const struct Address addr);
_Bool delete_address(struct AddressBook *addresses, const size_t pos);
struct Address *get_address(const struct AddressBook *addresses,
			    const size_t pos);
void delete_all_addresses(struct AddressBook *addresses);
struct Address *find_address_by_name(const struct AddressBook *addresses,
				     const char *name);
struct Address *find_address_by_surname(const struct AddressBook *addresses,
					const char *surname);
struct Address *find_address_by_email(const struct AddressBook *addresses,
				      const char *email);
struct Address *find_address_by_phone(const struct AddressBook *addresses,
				      const char *phone);
static struct ListElement *make_elem(const struct Address addr,
				     struct ListElement *next);

int main(void)
{
	char *home_dir = getenv("HOME");
	char file_path[PATH_BUFFER_SIZE];
	snprintf(file_path, PATH_BUFFER_SIZE, "%s/%s", home_dir, FILE_NAME);

	struct AddressBook addr_book = { .head = NULL, .size = 0 };

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

// next can be null
static struct ListElement *make_elem(const struct Address addr,
				     struct ListElement *next)
{
	struct ListElement *elem = malloc(sizeof(*elem));
	elem->data = addr;
	elem->next = next;
	return elem;
}

// Constructs an address book from addresses in the given file.
struct AddressBook read_addresses(FILE *file)
{
	struct AddressBook addresses = { .head = NULL, .size = 0 };

	char line[LINE_BUFFER_SIZE];
	if (fgets(line, LINE_BUFFER_SIZE, file) == NULL) {
		return addresses;
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
		last->next = make_elem(addr, NULL);
		last = last->next;
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

// Displays all addresses in a table.
void display_addresses(const struct AddressBook *addresses)
{
	printf("%ld records in total.\n", addresses->size);
	struct ListElement *elem = addresses->head;
	printf("%-15s\t%-15s\t%-30s\t%-12s\n", "NAME", "SURNAME", "EMAIL",
	       "PHONE");
	while (elem != NULL) {
		struct Address record = elem->data;
		printf("%-15s\t%-15s\t%-30s\t%-12s\n", record.name,
		       record.surname, record.email, record.phone);
		elem = elem->next;
	}
}

void add_address_to_end(struct AddressBook *addresses,
			const struct Address addr)
{
	struct ListElement *new = make_elem(addr, NULL);
	addresses->size += 1;
	struct ListElement *elem = addresses->head;
	if (elem == NULL) {
		addresses->head = new;
		return;
	}
	while (elem->next != NULL) {
		elem = elem->next;
	}
	elem->next = new;
}

_Bool add_address(struct AddressBook *addresses, const size_t pos,
		  const struct Address addr);
_Bool delete_address(struct AddressBook *addresses, const size_t pos);
struct Address *get_address(const struct AddressBook *addresses,
			    const size_t pos);
void delete_all_addresses(struct AddressBook *addresses);
struct Address *find_address_by_name(const struct AddressBook *addresses,
				     const char *name);
struct Address *find_address_by_surname(const struct AddressBook *addresses,
					const char *surname);
struct Address *find_address_by_email(const struct AddressBook *addresses,
				      const char *email);
struct Address *find_address_by_phone(const struct AddressBook *addresses,
				      const char *phone);
