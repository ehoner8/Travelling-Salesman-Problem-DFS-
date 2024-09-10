#include "stack.h"

#include <assert.h>
#include <inttypes.h>
#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

typedef struct stack {
    uint32_t capacity;
    uint32_t top;
    uint32_t *items;
} Stack;

Stack *stack_create(uint32_t capacity) {
    Stack *s = (Stack *) malloc(sizeof(Stack));
    s->capacity = capacity;
    s->top = 0;
    s->items = calloc(s->capacity, sizeof(uint32_t));

    return s;
}

void stack_free(Stack **sp) {
    if (sp != NULL && *sp != NULL) {
        if ((*sp)->items) {
            free((*sp)->items);
            (*sp)->items = NULL;
        }

        free(*sp);
    }

    if (sp != NULL) {
        *sp = NULL;
    }
}

bool stack_push(Stack *s, uint32_t val) {
    if (stack_full(s)) {
        return false;
    }
    s->items[s->top] = val;
    s->top++;
    return true;
}

bool stack_pop(Stack *s, uint32_t *val) {
    if (stack_empty(s)) {
        return false;
    }
    *val = s->items[s->top - 1];
    s->top--;
    return true;
}

bool stack_peek(const Stack *s, uint32_t *val) {
    if (stack_empty(s)) {
        return false;
    }
    *val = s->items[s->top - 1];
    return true;
}

bool stack_empty(const Stack *s) {
    if (s->top == 0) {
        return true;
    }
    return false;
}

bool stack_full(const Stack *s) {
    if (s->capacity == s->top) {
        return true;
    }
    return false;
}

uint32_t stack_size(const Stack *s) {
    return s->top;
}

uint32_t stack_capacity(const Stack *s) {
    return s->capacity;
}

void stack_copy(Stack *dst, const Stack *src) {
    assert(dst->capacity >= src->top);
    for (uint32_t e = 0; e < src->top; e++) {
        dst->items[e] = src->items[e];
    }
    dst->top = src->top;
}

void stack_print(const Stack *s, FILE *outfile, char *vals[]) {
    for (uint32_t i = 0; i < s->top; i += 1) {
        fprintf(outfile, "%s\n", vals[s->items[i]]);
    }
}
