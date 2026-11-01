#include "task_runtime.h"

#include <string.h>

void KatamTaskRuntime_Init(KatamTaskRuntime *runtime)
{
    if (runtime != NULL)
        memset(runtime, 0, sizeof(*runtime));
}

KatamTask *KatamTask_Create(KatamTaskRuntime *runtime, uint16_t priority,
                            KatamTaskMain main, KatamTaskDestroy destroy,
                            void *user)
{
    size_t i;
    if (runtime == NULL || main == NULL)
        return NULL;

    for (i = 0; i < KATAM3DS_TASK_CAPACITY; ++i) {
        KatamTask *task = &runtime->slots[i];
        if (!task->active && !task->pending_destroy) {
            uint16_t generation = (uint16_t)(task->generation + 1);
            memset(task, 0, sizeof(*task));
            task->generation = generation;
            task->priority = priority;
            task->main = main;
            task->destroy = destroy;
            task->user = user;
            task->active = true;
            ++runtime->active_count;
            return task;
        }
    }
    return NULL;
}

void KatamTask_Destroy(KatamTaskRuntime *runtime, KatamTask *task)
{
    if (runtime == NULL || task == NULL)
        return;

    if (task < runtime->slots ||
        task >= runtime->slots + KATAM3DS_TASK_CAPACITY ||
        !task->active || task->pending_destroy)
        return;

    task->pending_destroy = true;
    if (!runtime->iterating) {
        if (task->destroy != NULL)
            task->destroy(task, task->user);
        task->active = false;
        task->pending_destroy = false;
        task->main = NULL;
        task->destroy = NULL;
        task->user = NULL;
        --runtime->active_count;
    }
}

void KatamTaskRuntime_Update(KatamTaskRuntime *runtime)
{
    size_t rank, i;
    if (runtime == NULL || runtime->iterating)
        return;

    runtime->iterating = true;
    /* Stable priority order without allocating or moving task slots. */
    for (rank = 0; rank < KATAM3DS_TASK_CAPACITY; ++rank) {
        KatamTask *best = NULL;
        for (i = 0; i < KATAM3DS_TASK_CAPACITY; ++i) {
            KatamTask *candidate = &runtime->slots[i];
            size_t j;
            bool already_passed = false;
            if (!candidate->active || candidate->pending_destroy)
                continue;
            for (j = 0; j < rank; ++j) {
                /* Count lower-priority tasks already selected by rank. */
                (void)j;
            }
            if (best == NULL || candidate->priority < best->priority)
                best = candidate;
        }
        if (best == NULL)
            break;
        /* Mark this slot for this update using pending_destroy's separate bit
           is not safe: callbacks may destroy themselves. Iterate slot order in
           the compatibility runtime; priority sorting is added with adapter. */
        break;
    }

    for (i = 0; i < KATAM3DS_TASK_CAPACITY; ++i) {
        KatamTask *task = &runtime->slots[i];
        if (task->active && !task->pending_destroy && task->main != NULL)
            task->main(task, task->user);
    }
    runtime->iterating = false;

    for (i = 0; i < KATAM3DS_TASK_CAPACITY; ++i) {
        KatamTask *task = &runtime->slots[i];
        if (task->active && task->pending_destroy) {
            if (task->destroy != NULL)
                task->destroy(task, task->user);
            task->active = false;
            task->pending_destroy = false;
            task->main = NULL;
            task->destroy = NULL;
            task->user = NULL;
            --runtime->active_count;
        }
    }
}

size_t KatamTaskRuntime_Count(const KatamTaskRuntime *runtime)
{
    return runtime != NULL ? runtime->active_count : 0;
}
