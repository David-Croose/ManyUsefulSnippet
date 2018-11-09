/********************************************************************************

                                  sort chain

            a queue that contains the nearest datas you put in, and
        you can get the middle-value(not average-value) from the queue.

********************************************************************************/

#include <string.h>
#include "sortchain.h"

#define MAX_SEQ     (4000000000)     /* more larger more better */

/**
 * judge is a @handle full
 * @param handle: the sort chain handle
 * @return SCH_TRUE : full
 *         SCH_FALSE : not full
 */
static char is_full(schh_t *handle)
{
    return handle->full_flag;
}

/**
 * find an empty node from @handle
 * @param handle: the sort chain handle
 * @param node: the pointer variable stores the empty node's address
 * @return: see @schres_t
 */
static schres_t find_emptynode(schh_t *handle, schnode_t **node)
{
    unsigned int i;

    for(i = 0; i < SCH_NODES_TOTAL; i++)
    {
        if(handle->nodes[i].hasdata_flag == SCH_FALSE)
        {
            *node = &handle->nodes[i];
            return SCHRES_OK;
        }
    }

    return SCHRES_ERR;
}

/**
 * delete the oldest node of @handle
 * @param handle: the sort chain handle
 */
static void delete_oldestdata(schh_t *handle)
{
    schnode_t *anynode;

    // if the first one is the oldest one
    if(handle->head->seq == handle->oldestseq)
    {
        anynode = handle->head;
        handle->head = handle->head->next;
        anynode->next = NULL;
        anynode->hasdata_flag = SCH_FALSE;
        anynode->data = 0;
        handle->oldestseq = anynode->seq + 1;
        anynode->seq = 0;
        return;
    }

    // if the oldest one isn't in the first place
    for(anynode = handle->head; anynode != NULL; anynode = anynode->next)
    {
        if(anynode->next->seq == handle->oldestseq)
        {
            anynode->next = anynode->next->next;
            anynode->next->next = NULL;
            anynode->next->hasdata_flag = SCH_FALSE;
            handle->oldestseq = anynode->next->seq + 1;
            anynode->next->seq = 0;
            anynode->next->data = 0;
            return;
        }
    }
}

/**
 * insert a data to @handle
 * @param handle: the sort chain handle
 * @param data: the data to be inserted
 * @note: this function assumes that the @handle is not full. so be sure
 *        the @handle is not full before calling this function.
 */
static schres_t insert_newestdata(schh_t *handle, schdat_t data)
{
    schnode_t *node;
    schnode_t *anynode;

    // find an empty node to fill
    if(find_emptynode(handle, &node) != SCH_TRUE)
    {
        return SCHRES_ERR;
    }

    // fill the node
    (*node).data = data;
    (*node).seq = handle->newestseq++;
    (*node).hasdata_flag = SCH_TRUE;
    (*node).next = NULL;

    // insert the data
    for(anynode = handle->head; anynode != NULL; anynode = anynode->next)
    {
        // if data > head, make it compare to the latter datas one by one until
        // we get it's position
        if(anynode->data < data)
        {
            // if anynode < data < anynode.next, then the data could lay
            // between them
            if(anynode->next != NULL)
            {
                if(data < anynode->next->data)
                {
                    (*node).next = anynode->next;
                    anynode->next = node;
                }
            }
            else
            {
                // if anynode < data < anynode.next==NULL, then the data
                // would be in the end position
                anynode->next = node;
                (*node).next = NULL;
            }
        }
        else
        {
            // data <= head, we place it in the left of the head, and make
            // it be the head
            (*node).next = handle->head;
            handle->head = node;
        }
    }

    // set the chain's full flag
    if(handle->full_flag != SCH_TRUE)
    {
        if(handle->newestseq - handle->oldestseq + 1 >= SCH_NODES_TOTAL)
        {
            handle->full_flag = SCH_TRUE;
        }
    }

    return SCHRES_OK;
}

/**
 * is the sequence number goes to the top(@MAX_SEQ)
 * @param handle: the sort chain handle
 * @return SCH_TRUE: the sequence had reached to the top
 *         SCH_FALSE: the sequence hadn't reached to the top yet
 */
static char is_seqtop(schh_t *handle)
{
    if(handle->newestseq >= MAX_SEQ)
    {
        return SCH_TRUE;
    }
    return SCH_FALSE;
}

/**
 * reset all nodes' sequence
 * @param handle: the sort chain handle
 */
static void reset_all_seq(schh_t *handle)
{
    unsigned int i;

    for(i = 0; i < SCH_NODES_TOTAL; i++)
    {
        handle->nodes[i].seq -= handle->oldestseq;
    }
}

/**
 * get the middle-value of the @handle
 * @param handle: the sort chain handle
 * @param mid: the pointer variable stores the middle-value
 */
static void get_mid(schh_t *handle, schdat_t *mid)
{
    unsigned int i;
    schnode_t *anynode = handle->head;

    for(i = 0; i < SCH_NODES_TOTAL / 2; i++)
    {
        anynode = anynode->next;
    }
    *mid = anynode->data;
}

/**
 * sort chain initialization
 * @param handle: the sort chain handle
 * @return: see @schres_t
 */
schres_t sortchain_init(schh_t *handle)
{
    if(!handle)
    {
        return SCHRES_ERR;
    }

    memset(handle, 0, sizeof(schh_t));
    handle->head = &handle->nodes[0];
    return SCHRES_OK;
}

/**
 * add a data to @handle, and if it returns SCHRES_OK, you can get the
 * middle-value
 * @param handle: the sort chain handle
 * @param data: the data to be added in
 * @param mid: the pointer variable stores the middle-value
 * @return: see @schres_t
 */
schres_t sortchain_add(schh_t *handle, schdat_t data, schdat_t *mid)
{
    if(!handle || !mid)
    {
        return SCHRES_ERR;
    }

    if(is_full(handle) == SCH_TRUE)
    {
        delete_oldestdata(handle);
        insert_newestdata(handle, data);
        if(is_seqtop(handle) == SCH_TRUE)
        {
            reset_all_seq(handle);
        }
        get_mid(handle, mid);
    }
    else
    {
        insert_newestdata(handle, data);
        return SCHRES_NOTREADY;
    }

    return SCHRES_OK;
}

void demo_main(void)
{
    schh_t datalib;
    schdat_t dat = 34;
    schdat_t mid;

    sortchain_init(&datalib);
    sortchain_add(&datalib, dat, &mid);

}
