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
    KatamTask *ordered[KATAM3DS_TASK_CAPACITY];
    size_t count = 0;
    size_t i, j;

    if (runtime == NULL || runtime->iterating)
        return;

    /* Snapshot active tasks, then stable-sort by ascending priority. */
    for (i = 0; i < KATAM3DS_TASK_CAPACITY; ++i) {
        KatamTask *task = &runtime->slots[i];
        if (task->active && !task->pending_destroy)
            ordered[count++] = task;
    }

    for (i = 1; i < count; ++i) {
        KatamTask *key = ordered[i];
        j = i;
        while (j > 0 && ordered[j - 1]->priority > key->priority) {
            ordered[j] = ordered[j - 1];
            --j;
        }
        ordered[j] = key;
    }

    runtime->iterating = true;
    for (i = 0; i < count; ++i) {
        KatamTask *task = ordered[i];
        /* A previous callback may have destroyed this task. */
        if (task->active && !task->pending_destroy && task->main != NULL)
            task->main(task, task->user);
    }
    runtime->iterating = false;

    /* Destruction is deferred until iteration has finished. */
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
