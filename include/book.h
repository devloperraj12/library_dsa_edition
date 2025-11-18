#ifndef BOOK_H
#define BOOK_H

#define TITLE_MAX 128
#define AUTHOR_MAX 64
#define NAME_MAX 64

typedef struct {
    int id;
    char title[TITLE_MAX];
    char author[AUTHOR_MAX];
    int quantity;
} Book;

typedef struct {
    int id;
    char name[NAME_MAX];
} Member;

#endif /* BOOK_H */
