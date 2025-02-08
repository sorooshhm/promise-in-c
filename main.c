#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>

typedef void *(*Action)(void *arg);

enum STATE
{
    FULFILLED = 2,
    PENDING = 1,
};

typedef struct
{
    enum STATE state;
    Action action;
    Action callback;
    pthread_t thread;
} Promise;

void initPromise(Promise *promise, Action action, Action callback)
{
    pthread_t t;

    if (pthread_create(&t, NULL, action, promise))
    {
        exit(1);
    }
    promise->action = action;
    promise->callback = callback;
    promise->thread = t;
}

void *action(void *arg)
{
    printf("\naction");
    sleep(1);
    char *s = malloc(50 * sizeof(char));
    sprintf(s, "Hello from promise");
    ((Promise *)arg)->state = FULFILLED;
    ((Promise *)arg)->callback(s);
    return (void *)s;
};
void *callback(void *arg)
{
    char *s = (char *)arg;
    printf("\n %s <- the result", s);
    return NULL;
};

int main()
{
    Promise promise;
    initPromise(&promise, action, callback);
    printf("hello world");
    while (promise.state != FULFILLED)
    {
        sleep(1);
    }

    printf("after promise");

    return 0;
}