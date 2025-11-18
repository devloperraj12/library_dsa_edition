#include "../include/db.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static const char *BOOKS_FILE = "books.dat";
static const char *MEMBERS_FILE = "members.dat";
static const char *ISSUES_FILE = "issues.dat";

typedef struct {
    int memberId;
    int bookId;
} IssueRecord;

void db_init(void) {
    FILE *f;
    f = fopen(BOOKS_FILE, "ab+"); if (f) fclose(f);
    f = fopen(MEMBERS_FILE, "ab+"); if (f) fclose(f);
    f = fopen(ISSUES_FILE, "ab+"); if (f) fclose(f);
}

/* Helper: read all books into dynamically allocated array; returns count via out_count */
static Book* read_all_books(size_t *out_count) {
    FILE *f = fopen(BOOKS_FILE, "rb");
    *out_count = 0;
    if (!f) return NULL;
    fseek(f, 0, SEEK_END);
    long sz = ftell(f);
    if (sz <= 0) { fclose(f); return NULL; }
    fseek(f, 0, SEEK_SET);
    size_t count = sz / sizeof(Book);
    Book *arr = malloc(count * sizeof(Book));
    if (!arr) { fclose(f); return NULL; }
    fread(arr, sizeof(Book), count, f);
    fclose(f);
    *out_count = count;
    return arr;
}

/* Helper: write all books (overwrite file) */
static int write_all_books(Book *arr, size_t count) {
    FILE *f = fopen(BOOKS_FILE, "wb");
    if (!f) return -1;
    fwrite(arr, sizeof(Book), count, f);
    fclose(f);
    return 0;
}

int db_addBook(Book b) {
    size_t n;
    Book *arr = read_all_books(&n);
    int maxid = 0;
    for (size_t i = 0; i < n; ++i) if (arr[i].id > maxid) maxid = arr[i].id;
    b.id = maxid + 1;
    if (arr == NULL) {
        arr = malloc(sizeof(Book));
        n = 0;
    } else {
        arr = realloc(arr, (n+1)*sizeof(Book));
    }
    arr[n] = b;
    n++;
    int r = write_all_books(arr, n);
    free(arr);
    return (r==0) ? b.id : -1;
}

void db_listBooks(void) {
    size_t n;
    Book *arr = read_all_books(&n);
    if (!arr || n == 0) {
        puts("No books available.");
        free(arr);
        return;
    }
    for (size_t i = 0; i < n; ++i) {
        printf("ID:%d | %s | %s | Qty:%d\n", arr[i].id, arr[i].title, arr[i].author, arr[i].quantity);
    }
    free(arr);
}

Book* db_searchBookById(int id) {
    size_t n;
    Book *arr = read_all_books(&n);
    if (!arr) return NULL;
    for (size_t i = 0; i < n; ++i) {
        if (arr[i].id == id) {
            Book *res = malloc(sizeof(Book));
            *res = arr[i];
            free(arr);
            return res;
        }
    }
    free(arr);
    return NULL;
}

int db_updateBookQuantity(int id, int quantity) {
    size_t n;
    Book *arr = read_all_books(&n);
    if (!arr) return -1;
    int found = -1;
    for (size_t i = 0; i < n; ++i) {
        if (arr[i].id == id) {
            arr[i].quantity = quantity;
            found = 0;
            break;
        }
    }
    if (found == -1) { free(arr); return -1; }
    int r = write_all_books(arr, n);
    free(arr);
    return (r==0) ? 0 : -1;
}

int db_deleteBookById(int id) {
    size_t n;
    Book *arr = read_all_books(&n);
    if (!arr) return -1;
    size_t write_idx = 0;
    int found = -1;
    for (size_t i = 0; i < n; ++i) {
        if (arr[i].id == id) { found = 0; continue; }
        arr[write_idx++] = arr[i];
    }
    if (found == -1) { free(arr); return -1; }
    int r = write_all_books(arr, write_idx);
    free(arr);
    return (r==0) ? 0 : -1;
}

/* Members */
static Member* read_all_members(size_t *out_count) {
    FILE *f = fopen(MEMBERS_FILE, "rb");
    *out_count = 0;
    if (!f) return NULL;
    fseek(f, 0, SEEK_END);
    long sz = ftell(f);
    if (sz <= 0) { fclose(f); return NULL; }
    fseek(f, 0, SEEK_SET);
    size_t count = sz / sizeof(Member);
    Member *arr = malloc(count * sizeof(Member));
    if (!arr) { fclose(f); return NULL; }
    fread(arr, sizeof(Member), count, f);
    fclose(f);
    *out_count = count;
    return arr;
}
static int write_all_members(Member *arr, size_t count) {
    FILE *f = fopen(MEMBERS_FILE, "wb");
    if (!f) return -1;
    fwrite(arr, sizeof(Member), count, f);
    fclose(f);
    return 0;
}

int db_addMember(const char* name) {
    size_t n;
    Member *arr = read_all_members(&n);
    int maxid = 0;
    for (size_t i = 0; i < n; ++i) if (arr[i].id > maxid) maxid = arr[i].id;
    Member m;
    m.id = maxid + 1;
    strncpy(m.name, name ? name : "", NAME_MAX-1);
    m.name[NAME_MAX-1] = '\\0';
    if (arr == NULL) {
        arr = malloc(sizeof(Member));
        n = 0;
    } else {
        arr = realloc(arr, (n+1)*sizeof(Member));
    }
    arr[n] = m; n++;
    int r = write_all_members(arr, n);
    free(arr);
    return (r==0) ? m.id : -1;
}

void db_listMembers(void) {
    size_t n;
    Member *arr = read_all_members(&n);
    if (!arr || n == 0) {
        puts("No members.");
        free(arr);
        return;
    }
    for (size_t i=0;i<n;++i) {
        printf("ID:%d | %s\n", arr[i].id, arr[i].name);
    }
    free(arr);
}

Member* db_searchMemberById(int id) {
    size_t n;
    Member *arr = read_all_members(&n);
    if (!arr) return NULL;
    for (size_t i=0;i<n;++i) {
        if (arr[i].id == id) {
            Member *res = malloc(sizeof(Member));
            *res = arr[i];
            free(arr);
            return res;
        }
    }
    free(arr);
    return NULL;
}

/* Issue / Return handling using issues.dat */
static IssueRecord* read_all_issues(size_t *out_count) {
    FILE *f = fopen(ISSUES_FILE, "rb");
    *out_count = 0;
    if (!f) return NULL;
    fseek(f, 0, SEEK_END);
    long sz = ftell(f);
    if (sz <= 0) { fclose(f); return NULL; }
    fseek(f, 0, SEEK_SET);
    size_t count = sz / sizeof(IssueRecord);
    IssueRecord *arr = malloc(count * sizeof(IssueRecord));
    if (!arr) { fclose(f); return NULL; }
    fread(arr, sizeof(IssueRecord), count, f);
    fclose(f);
    *out_count = count;
    return arr;
}
static int write_all_issues(IssueRecord *arr, size_t count) {
    FILE *f = fopen(ISSUES_FILE, "wb");
    if (!f) return -1;
    fwrite(arr, sizeof(IssueRecord), count, f);
    fclose(f);
    return 0;
}

int db_issueBook(int memberId, int bookId) {
    /* check member exists */
    if (!db_searchMemberById(memberId)) return -1;
    /* find book and decrease quantity */
    size_t n;
    Book *arr = read_all_books(&n);
    if (!arr) return -1;
    int found = -1;
    for (size_t i=0;i<n;++i) {
        if (arr[i].id == bookId) {
            if (arr[i].quantity <= 0) { free(arr); return -1; }
            arr[i].quantity -= 1;
            found = 0;
            break;
        }
    }
    if (found == -1) { free(arr); return -1; }
    write_all_books(arr, n);
    free(arr);

    /* add issue record */
    size_t m;
    IssueRecord *issues = read_all_issues(&m);
    if (issues == NULL) { issues = malloc(sizeof(IssueRecord)); m = 0; }
    else { issues = realloc(issues, (m+1)*sizeof(IssueRecord)); }
    issues[m].memberId = memberId; issues[m].bookId = bookId; m++;
    write_all_issues(issues, m);
    free(issues);
    return 0;
}

int db_returnBook(int memberId, int bookId) {
    /* remove issue record if exists */
    size_t m;
    IssueRecord *issues = read_all_issues(&m);
    if (!issues) return -1;
    size_t write_idx = 0;
    int found = -1;
    for (size_t i=0;i<m;++i) {
        if (issues[i].memberId == memberId && issues[i].bookId == bookId) { found = 0; continue; }
        issues[write_idx++] = issues[i];
    }
    if (found == -1) { free(issues); return -1; }
    write_all_issues(issues, write_idx);
    free(issues);

    /* increment book quantity */
    size_t n;
    Book *arr = read_all_books(&n);
    if (!arr) return -1;
    for (size_t i=0;i<n;++i) {
        if (arr[i].id == bookId) {
            arr[i].quantity += 1;
            break;
        }
    }
    write_all_books(arr, n);
    free(arr);
    return 0;
}
