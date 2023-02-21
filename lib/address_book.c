#include <stdlib.h>
#include <string.h>

#include "address_book.h"

#define LINE_BUFFER_SIZE 1024

static struct ListElement *make_elem(const struct Address addr,
				     struct ListElement *next);

void save_addresses_to_file(const struct AddressBook *addresses, FILE *file)
{
	struct ListElement *elem = addresses->head;
	while (elem != NULL) {
		struct Address record = elem->data;
		fprintf(file, "%s,%s,%s,%s\n", record.name, record.surname,
			record.email, record.phone);
		elem = elem->next;
	}
}

struct AddressBook create_address_book()
{
	struct AddressBook addr_book = { .head = NULL, .size = 0 };
	return addr_book;
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
	struct ListElement *last = make_elem(addr, NULL);
	addresses.head = last;
	addresses.size += 1;

	while (fgets(line, LINE_BUFFER_SIZE, file) != NULL) {
		len = strlen(line);
		if (line[len - 1] == '\n') {
			line[len - 1] = '\0';
		}

		addr = parse_line(line);
		last->next = make_elem(addr, NULL);
		last = last->next;
		addresses.size += 1;
	}

	return addresses;
}

// Always appends null terminator.
void safe_strncpy(char *dest, const char *src, size_t dest_size)
{
	strncpy(dest, src, dest_size);
	dest[dest_size - 1] = '\0';
}

struct Address make_address(char *name, char *surname, char *email, char *phone)
{
	struct Address addr;
	safe_strncpy(addr.name, name, sizeof(addr.name));
	safe_strncpy(addr.surname, surname, sizeof(addr.surname));
	safe_strncpy(addr.email, email, sizeof(addr.email));
	safe_strncpy(addr.phone, phone, sizeof(addr.phone));

	return addr;
}

// Creates an address from a single CSV line.
struct Address parse_line(char *line)
{
	char *name = strtok(line, ",");
	char *surname = strtok(NULL, ",");
	char *email = strtok(NULL, ",");
	char *phone = strtok(NULL, ",");
	return make_address(name, surname, email, phone);
}

// Displays all addresses in a table.
void display_addresses(const struct AddressBook *addresses)
{
	printf("%zu records in total.\n", addresses->size);
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

// Add address in the specified position (after pos - 1 records).
// Returns false (and does nothing) if there are fewer than (pos - 1) records.
bool add_address(struct AddressBook *addresses, const size_t pos,
		 const struct Address addr)
{
	struct ListElement *new = make_elem(addr, NULL);
	addresses->size += 1;
	struct ListElement *elem = addresses->head;
	if (pos == 0) {
		new->next = elem;
		addresses->head = new;
	} else {
		if (addresses->size < pos) {
			return false;
		}

		for (size_t i = 1; i < pos; ++i) {
			elem = elem->next;
		}
		new->next = elem->next;
		elem->next = new;
	}

	return true;
}

bool delete_address(struct AddressBook *addresses, const size_t pos)
{
	if (addresses->size <= pos) {
		return false;
	}

	struct ListElement *elem = addresses->head;
	if (pos == 0) {
		addresses->head = elem->next;
		free(elem);
		return true;
	}
	// Iterate until one before elem to be deleted.
	for (size_t i = 0; i < pos - 1; ++i) {
		elem = elem->next;
	}
	struct ListElement *temp = elem->next->next;
	free(elem->next);
	elem->next = temp;

	return true;
}

struct Address *get_address(struct AddressBook *addresses, const size_t pos)
{
	if (addresses->size <= pos) {
		return NULL;
	}

	struct ListElement *elem = addresses->head;
	for (size_t i = 0; i < pos; ++i) {
		elem = elem->next;
	}
	return &elem->data;
}

void delete_all_addresses(struct AddressBook *addresses)
{
	if (addresses->size == 0) {
		return;
	}

	struct ListElement *elem = addresses->head;
	while (elem != NULL) {
		struct ListElement *next = elem->next;
		free(elem);
		elem = next;
	}
	addresses->head = NULL;
	addresses->size = 0;
}

void delete_address_book(struct AddressBook *addresses)
{
	delete_all_addresses(addresses);
}
