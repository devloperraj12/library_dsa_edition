#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/db.h"

static void flush_stdin(void) {
    int c;
    while ((c = getchar()) != EOF && c != '\n');
}

int read_int_with_prompt(const char *prompt, int *out) {
    char buf[64];
    printf("%s", prompt);
    if (!fgets(buf, sizeof(buf), stdin)) return 0;
    char *end;
    long val = strtol(buf, &end, 10);
    if (end == buf || (*end != '\\n' && *end != '\\0' && *end != '\\r')) {
        return 0;
    }
    *out = (int)val;
    return 1;
}

int main(void) {
    db_init();

    for (;;) {
        printf("\nLibrary - DSA Edition\n");
        printf("1. Add Book\n");
        printf("2. List Books\n");
        printf("3. Search Book by ID\n");
        printf("4. Delete Book by ID\n");
        printf("5. Update Book Quantity\n");
        printf("6. Add Member\n");
        printf("7. List Members\n");
        printf("8. Issue Book to Member\n");
        printf("9. Return Book from Member\n");
        printf("10. Exit\n");
        printf("Enter choice: ");

        int ch;
        if (scanf("%d", &ch) != 1) {
            printf("Invalid input. Please enter a number.\n");
            flush_stdin();
            continue;
        }
        flush_stdin();

        if (ch == 1) {
            Book b;
            memset(&b, 0, sizeof(b));
            printf("Enter title: ");
            if (!fgets(b.title, TITLE_MAX, stdin)) { printf("Input error\n"); continue; }
            b.title[strcspn(b.title, "\r\n")] = 0;
            printf("Enter author: ");
            if (!fgets(b.author, AUTHOR_MAX, stdin)) { printf("Input error\n"); continue; }
            b.author[strcspn(b.author, "\r\n")] = 0;
            if (!read_int_with_prompt("Enter quantity: ", &b.quantity)) {
                b.quantity = 1;
            }
            int id = db_addBook(b);
            if (id > 0) printf("[+] Book added with ID %d\n", id);
            else printf("[!] Failed to add book (check file permissions).\n");
        }
        else if (ch == 2) {
            db_listBooks();
        }
        else if (ch == 3) {
            int id;
            if (!read_int_with_prompt("Enter book ID: ", &id)) { printf("Invalid ID\n"); continue; }
            Book *b = db_searchBookById(id);
            if (b) {
                printf("ID:%d | %s | %s | Qty:%d\n", b->id, b->title, b->author, b->quantity);
                free(b);
            } else printf("Book not found\n");
        }
        else if (ch == 4) {
            int id;
            if (!read_int_with_prompt("Enter book ID to delete: ", &id)) { printf("Invalid ID\n"); continue; }
            if (db_deleteBookById(id) == 0) printf("Deleted (if existed).\n");
            else printf("Book not found or delete failed.\n");
        }
        else if (ch == 5) {
            int id, q;
            if (!read_int_with_prompt("Enter book ID to update quantity: ", &id)) { printf("Invalid ID\n"); continue; }
            if (!read_int_with_prompt("Enter new quantity: ", &q)) { printf("Invalid quantity\n"); continue; }
            if (db_updateBookQuantity(id, q) == 0) printf("Quantity updated\n");
            else printf("Book not found\n");
        }
        else if (ch == 6) {
            char name[NAME_MAX];
            printf("Enter member name: ");
            if (!fgets(name, NAME_MAX, stdin)) { printf("Input error\n"); continue; }
            name[strcspn(name, "\r\n")] = 0;
            int mid = db_addMember(name);
            if (mid > 0) printf("[+] Member added with ID %d\n", mid);
            else printf("[!] Failed to add member\n");
        }
        else if (ch == 7) {
            db_listMembers();
        }
        else if (ch == 8) {
            int mid, bid;
            if (!read_int_with_prompt("Enter member ID: ", &mid)) { printf("Invalid ID\n"); continue; }
            if (!read_int_with_prompt("Enter book ID to issue: ", &bid)) { printf("Invalid ID\n"); continue; }
            int r = db_issueBook(mid, bid);
            if (r == 0) printf("[+] Issued book %d to member %d\n", bid, mid);
            else printf("[!] Issue failed (check member/book/availability)\n");
        }
        else if (ch == 9) {
            int mid, bid;
            if (!read_int_with_prompt("Enter member ID: ", &mid)) { printf("Invalid ID\n"); continue; }
            if (!read_int_with_prompt("Enter book ID to return: ", &bid)) { printf("Invalid ID\n"); continue; }
            int r = db_returnBook(mid, bid);
            if (r == 0) printf("[+] Returned book %d from member %d\n", bid, mid);
            else printf("[!] Return failed (record not found)\n");
        }
        else if (ch == 10) {
            printf("Goodbye!\n");
            break;
        }
        else {
            printf("Invalid option\n");
        }
    }

    return 0;
}
