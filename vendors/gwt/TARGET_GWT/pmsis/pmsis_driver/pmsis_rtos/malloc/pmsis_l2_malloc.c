#include <stdio.h>
#include <string.h>
#include "pmsis.h"
#include "pmsis/rtos/os_frontend_api/pmsis_task.h"
#include "pmsis/rtos/malloc/pmsis_l2_malloc.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*******************************************************************************
 * Driver data
 *****************************************************************************/

#if (__PMSIS_L2_MALLOC_NATIVE__ == 0)
malloc_t __l2_malloc;
pmsis_spinlock_t __l2_malloc_spinlock;

/*******************************************************************************
 * API implementation
 ******************************************************************************/

void *pi_l2_malloc(int size)
{
    void *ret_ptr;
    pmsis_spinlock_take(&__l2_malloc_spinlock);
    ret_ptr = __malloc(&__l2_malloc, size);
    pmsis_spinlock_release(&__l2_malloc_spinlock);
    if(ret_ptr == NULL)
    {
        printf("Malloc failed\n");
    }
    return ret_ptr;
}

void pi_l2_free(void *_chunk, int size)
{
    pmsis_spinlock_take(&__l2_malloc_spinlock);
    __malloc_free(&__l2_malloc, _chunk, size);
    pmsis_spinlock_release(&__l2_malloc_spinlock);
}

void *pi_l2_malloc_align(int size, int align)
{
    void *ret_ptr;
    pmsis_spinlock_take(&__l2_malloc_spinlock);
    ret_ptr = __malloc_align(&__l2_malloc, size, align);
    pmsis_spinlock_release(&__l2_malloc_spinlock);
    return ret_ptr;
}

/** \brief malloc init: initialize mutex and heap
 * Warning: thread unsafe by nature
 * \param heapstart heap start address
 * \param heap_size size of the heap
 */
void pmsis_l2_malloc_init(void *heapstart, uint32_t heap_size)
{
    __malloc_init(&__l2_malloc, heapstart, heap_size);
    pmsis_spinlock_init(&__l2_malloc_spinlock);
}

void pmsis_l2_malloc_set_malloc_struct(malloc_t malloc_struct)
{
    pmsis_spinlock_take(&__l2_malloc_spinlock);
    memcpy(&__l2_malloc,&malloc_struct,sizeof(malloc_t));
    pmsis_spinlock_release(&__l2_malloc_spinlock);
}

malloc_t pmsis_l2_malloc_get_malloc_struct(void)
{
    malloc_t malloc_struct;
    pmsis_spinlock_take(&__l2_malloc_spinlock);
    memcpy(&malloc_struct,&__l2_malloc,sizeof(malloc_t));
    pmsis_spinlock_release(&__l2_malloc_spinlock);
    return malloc_struct;
}
#endif
