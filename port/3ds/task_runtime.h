#ifndef KATAM3DS_TASK_RUNTIME_H
#define KATAM3DS_TASK_RUNTIME_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#define KATAM3DS_TASK_CAPACITY 128

typedef struct KatamTask KatamTask;
typedef void (*KatamTaskMain)(KatamTask *task, void *user);
typedef void (*KatamTaskDestroy)(KatamTask *task, void *user);

struct KatamTask {
    KatamTaskMain main;
    KatamTaskDestroy destroy;
    void *user;
    uint16_t priority;
    uint16_t generation;
    bool active;
    bool pending_destroy;
};

typedef struct {
    KatamTask slots[KATAM3DS_TASK_CAPACITY];
    size_t active_count;
    bool iterating;
} KatamTaskRuntime;

void KatamTaskRuntime_Init(KatamTaskRuntime *runtime);
KatamTask *KatamTask_Create(KatamTaskRuntime *runtime, uint16_t priority,
                            KatamTaskMain main, KatamTaskDestroy destroy,
                            void *user);
void KatamTask_Destroy(KatamTaskRuntime *runtime, KatamTask *task);
void KatamTaskRuntime_Update(KatamTaskRuntime *runtime);
size_t KatamTaskRuntime_Count(const KatamTaskRuntime *runtime);

#ifdef __cplusplus
}
#endif
#endif
