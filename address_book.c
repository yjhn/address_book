#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

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

// When empty, size = 0 and head = NULL.
struct AddressBook {
	struct ListElement *head;
	size_t size;
};

struct AddressBook create_address_book();
struct AddressBook read_addresses(FILE *file);
struct Address parse_line(char *line);
void display_addresses(const struct AddressBook *addresses);
void add_address_to_end(struct AddressBook *addresses,
			const struct Address addr);
bool add_address(struct AddressBook *addresses, const size_t pos,
		 const struct Address addr);
bool delete_address(struct AddressBook *addresses, const size_t pos);
struct Address *get_address(struct AddressBook *addresses, const size_t pos);
void delete_all_addresses(struct AddressBook *addresses);
void delete_address_book(struct AddressBook *addresses);
struct Address *find_address_by_name(struct AddressBook *addresses,
				     const char *name);
struct Address *find_address_by_surname(struct AddressBook *addresses,
					const char *surname);
struct Address *find_address_by_email(struct AddressBook *addresses,
				      const char *email);
struct Address *find_address_by_phone(struct AddressBook *addresses,
				      const char *phone);
static struct ListElement *make_elem(const struct Address addr,
				     struct ListElement *next);
static void delete_address_record(struct Address *addr)
{
	free(addr->name);
	free(addr->surname);
	free(addr->email);
	free(addr->phone);
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

// Add address in the specified position (after pos - 1 records).
// Returns false (and does nothing) if there are fewer than (pos - 1) records.
bool add_address(struct AddressBook *addresses, const size_t pos,
		 const struct Address addr)
{
	if (pos > 0 && addresses->size < pos - 1) {
		return false;
	}

	struct ListElement *new = make_elem(addr, NULL);
	addresses->size += 1;
	struct ListElement *elem = addresses->head;
	for (size_t i = 0; i < pos; ++i) {
		elem = elem->next;
	}
	new->next = elem->next;
	elem->next = new;
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
		delete_address_record(&elem->data);
		free(elem);
	} else {
		// Iterate until one before elem to be deleted.
		for (size_t i = 0; i < pos - 1; ++i) {
			elem = elem->next;
		}
		struct ListElement *temp = elem->next->next;
		delete_address_record(&elem->data);
		free(elem->next);
		elem->next = temp;
	}
	return true;
}

struct Address *get_address(struct AddressBook *addresses, const size_t pos)
{
	if (addresses->size <= pos) {
		return NULL;
	}

	struct ListElement *elem = addresses->head;
	for (size_t i = 0; i <= pos; ++i) {
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
		delete_address_record(&elem->data);
		free(elem);
		elem = next;
	}
	addresses->head = NULL;
	addresses->size = 0;
}

struct Address *find_address_by_name(struct AddressBook *addresses,
				     const char *name)
{
	struct ListElement *elem = addresses->head;
	for (size_t i = 0; i < addresses->size; ++i) {
		if (strcmp(elem->data.name, name) == 0) {
			return &elem->data;
		}
	}
	return NULL;
}

struct Address *find_address_by_surname(struct AddressBook *addresses,
					const char *surname)
{
	struct ListElement *elem = addresses->head;
	for (size_t i = 0; i < addresses->size; ++i) {
		if (strcmp(elem->data.surname, surname) == 0) {
			return &elem->data;
		}
	}
	return NULL;
}

struct Address *find_address_by_email(struct AddressBook *addresses,
				      const char *email)
{
	struct ListElement *elem = addresses->head;
	for (size_t i = 0; i < addresses->size; ++i) {
		if (strcmp(elem->data.email, email) == 0) {
			return &elem->data;
		}
	}
	return NULL;
}

struct Address *find_address_by_phone(struct AddressBook *addresses,
				      const char *phone)
{
	struct ListElement *elem = addresses->head;
	for (size_t i = 0; i < addresses->size; ++i) {
		if (strcmp(elem->data.phone, phone) == 0) {
			return &elem->data;
		}
	}
	return NULL;
}

void delete_address_book(struct AddressBook *addresses)
{
	delete_all_addresses(addresses);
}
