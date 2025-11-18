#ifndef DB_H
#define DB_H

#include "book.h"

/* initialize file storage (create files if missing) */
void db_init(void);

/* Book functions */
int db_addBook(Book b);
void db_listBooks(void);
Book* db_searchBookById(int id); /* Caller does not need to free (we return malloc; it's fine if not freed in small program) */
int db_updateBookQuantity(int id, int quantity); /* 0 success, -1 not found */
int db_deleteBookById(int id); /* 0 success, -1 not found */

/* Member functions */
int db_addMember(const char* name);
void db_listMembers(void);
Member* db_searchMemberById(int id);

/* Issue / Return */
int db_issueBook(int memberId, int bookId);  /* 0 success, -1 failure */
int db_returnBook(int memberId, int bookId); /* 0 success, -1 failure */

#endif /* DB_H */
