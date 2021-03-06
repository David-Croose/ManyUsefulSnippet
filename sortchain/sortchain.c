/********************************************************************************

                                  sort chain

            a queue that contains the nearest datas you put in, and
        you can get the middle-value(not average-value) from the queue.

********************************************************************************/

#include <stdio.h>
#include <string.h>
#include "sortchain.h"

// the sequence is in [0, MAX_SEQ) nomally, if @THRESHOLD > @MAX_SEQ, the
// sequence is in [0, THRESHOLD), in other words, the routine can adapt
// the @MAX_SEQ to @THRESHOLD automatically, so you don't need to change it.
#define MAX_SEQ     (4294967296 - 1)

#define THRESHOLD   (handle->thres)

/**
 * is the @handle full
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

    if(handle->sparenode != NULL)
    {
        *node = handle->sparenode;
        return SCHRES_OK;
    }
    else
    {
        for(i = 0; i < THRESHOLD; i++)
        {
            if(handle->nodes[i].hasdata_flag == SCH_FALSE)
            {
                *node = &handle->nodes[i];
                return SCHRES_OK;
            }
        }
    }

    return SCHRES_ERR;
}

/**
 * delete the oldest node from @handle
 * @param handle: the sort chain handle
 * @return: see @schres_t
 */
static schres_t delete_oldestdata(schh_t *handle)
{
    schnode_t *delnode;     // delete node

    // get the oldest node
    fifo_od(&handle->odqh, &delnode);

    // get the deleted node position, so we can get
    // the mid-node position in @insert_newestdata
    if(delnode == handle->midnode)
    {
        handle->del_flag = DELP_MID;
        handle->ldelnode = handle->midnode->prev;
    }
    else
    {
        if(delnode->data < handle->midnode->data)
        {
            handle->del_flag = DELP_LEFT;
        }
        else if(delnode->data > handle->midnode->data)
        {
            handle->del_flag = DELP_RIGHT;
        }
        else
        {
            // if delnode->data == handle->midnode->data
            if(delnode->sseq < handle->midnode->sseq)
            {
                handle->del_flag = DELP_LEFT;
            }
            else if(delnode->sseq > handle->midnode->sseq)
            {
                handle->del_flag = DELP_RIGHT;
            }
            else
            {
                return SCHRES_ERR;
            }
        }
    }

    // delete the oldest node
    if(delnode->next == NULL)
    {
        // if @delnode is in the end
        delnode->prev->next = NULL;
    }
    else if(delnode == handle->head)
    {
        // if @delnode is in the head
        handle->head = delnode->next;
        delnode->next->prev = handle->head;
    }
    else
    {
        delnode->prev->next = delnode->next;
        delnode->next->prev = delnode->prev;
    }
    memset(delnode, 0, sizeof(schnode_t));

    // update @oldestseq
    handle->oldestseq++;

    // update @sparenode
    handle->sparenode = delnode;

    return SCHRES_OK;
}

///////////////////////////////////////
static void print_all(schh_t *handle, schdat_t data)
{
#if 0
    schnode_t *anynode;
    static int aa;

    if(handle->full_flag == 1 && aa == 0)
    {
        aa = 1;
        printf("-------------------------------------\n");
    }

    for(anynode = handle->head; anynode != NULL; anynode = anynode->next)
    {
        printf("%d(%d) ", anynode->data, anynode->sseq);
    }

    if(handle->full_flag == 1)
    {
        printf("  ---  data=%d, mid=%d\n", data, handle->midnode->data);
    }
    else
    {
        printf("  ---  data=%d, mid=NA\n", data);
    }

#endif
}
///////////////////////////////////////

/**
 * update the sseq of @handle, the sseq should not exceed the @MAX_SEQ
 * the sseq: same-sequence
 * what is it for: in a sort-chain, there may are many same datas, they
 *                 lay in line. that's not a problem, but when we delete
 *                 the oldest data from a sort-chain while the oldest
 *                 node's data == mid node's data, how can we know the
 *                 oldest node is in the right of the mid node or left?
 *                 so we need sseq, when you insert a data to sort-chain
 *                 if it's data equals one of the datas in the sort-chain,
 *                 it's sseq will add itself 1. however, the sseq should
 *                 not exceed the @MAX_SEQ
 * @param handle: the sort chain handle
 * @param node: the node that will update it's sseq
 */
static void update_sseq(schh_t *handle, schnode_t *node)
{
    unsigned int sseq_min;
    schnode_t *anynode;

    if(node == handle->head || node->data != node->prev->data)
    {
        return;
    }

    (*node).sseq = node->prev->sseq + 1;
    if((*node).sseq >= MAX_SEQ)
    {
        // if the @sseq reachs to the top, reset it
        sseq_min = (*node).sseq - (handle->thres - 1);
        for(anynode = node; anynode->data == anynode->prev->data;
            anynode = anynode->prev)
        {
            if(anynode == handle->head)
            {
                break;
            }
            anynode->sseq -= sseq_min;
        }
        anynode->sseq -= sseq_min;
    }
}

/**
 * insert a data bigger then @lin to @handle
 * @param handle: the sort chain handle
 * @param node: the node stores the inserted data
 */
static void insert_biggerdata(schh_t *handle, schnode_t *node)
{
    schnode_t *anynode;

    for(anynode = handle->lin; anynode != NULL; anynode = anynode->next)
    {
        if(anynode->next != NULL)
        {
            if(node->data < anynode->next->data)
            {
                // if anynode <= data < anynode->next, then the data would
                // lay between them
                (*node).next = anynode->next;
                anynode->next->prev = node;
                anynode->next = node;
                (*node).prev = anynode;
                break;
            }
            else
            {
                // nothing to do here
            }
        }
        else
        {
            // if anynode <= data < anynode.next==NULL, then the data
            // would be in the end position
            (*node).next = NULL;
            (*node).prev = anynode;
            anynode->next = node;
            break;
        }
    }
}

/**
 * insert a data smaller then @lin to @handle
 * @param handle: the sort chain handle
 * @param node: the node stores the inserted data
 */
static void insert_smallerdata(schh_t *handle, schnode_t *node)
{
    schnode_t *anynode;

    if(node->data < handle->head->data)
    {
        // data < head, we place it in the left of the head, and make
        // it be the head
        (*node).next = handle->head;
        handle->head->prev = node;
        handle->head = node;
        (*node).prev = handle->head;
        return;
    }

    for(anynode = handle->lin; /* nothing here */; anynode = anynode->prev)
    {
        if(node->data >= anynode->prev->data)
        {
            // if anynode->prev <= data < anynode, then the data would lay
            // between them
            (*node).next = anynode;
            (*node).prev = anynode->prev;
            (*node).prev->next = node;
            (*node).next->prev = node;
            break;
        }
        else
        {
            // nothing to do
        }
    }
}

/**
 * insert the newest data to @handle
 * @param handle: the sort chain handle
 * @param data: the inserted data
 * @return: see @schres_t
 */
static schres_t insert_newestdata(schh_t *handle, schdat_t data)
{
    schnode_t *node, *anynode;
    unsigned int i;

    // find an empty node to fill
    if(find_emptynode(handle, &node) != SCHRES_OK)
    {
        return SCHRES_ERR;
    }

    // fill the node
    (*node).data = data;
    (*node).seq = handle->newestseq++;
    (*node).hasdata_flag = SCH_TRUE;
    (*node).next = NULL;
    (*node).prev = NULL;
    if(handle->head == NULL)
    {
        // if there is only one data in sort chain now
        handle->head = node;
        (*node).prev = handle->head;
        goto after_inserting;
    }

    // inserting the @data
    if(data >= handle->lin->data)
    {
        insert_biggerdata(handle, node);
    }
    else
    {
        insert_smallerdata(handle, node);
    }
    update_sseq(handle, node);

after_inserting:

    // updating related stuff
    if(handle->full_flag == SCH_FALSE
       && (handle->newestseq - handle->oldestseq) >= handle->thres)
    {
        handle->full_flag = SCH_TRUE;
    }
    handle->lin = node;

    // push the current node address to fifo
    fifo_in(&handle->odqh, &node);

    // get mid-value
    if(handle->del_flag != DELP_NONE)
    {
        switch(handle->del_flag)
        {
        case DELP_LEFT:
            if(data >= handle->midnode->data)
            {
                handle->midnode = handle->midnode->next;
            }
            else
            {
                // nothing to do here
            }
            break;

        case DELP_RIGHT:
            if(data >= handle->midnode->data)
            {
                // nothing to do here
            }
            else
            {
                handle->midnode = handle->midnode->prev;
            }
            break;

        case DELP_MID:
            if(data >= handle->ldelnode->data)
            {
                handle->midnode = handle->ldelnode->next;
            }
            else
            {
                handle->midnode = handle->ldelnode;
            }
            break;

        default:
            return SCHRES_ERR;
        }
    }
    else
    {
        if(handle->full_flag == SCH_TRUE)   // if this is the first time we come here
        {
            for(anynode = handle->head, i = 0; anynode != NULL; anynode = anynode->next, i++)
            {
                if(i == THRESHOLD / 2)
                {
                    handle->midnode = anynode;
                    break;
                }
            }
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
    if(handle->newestseq - 1 >= MAX_SEQ)
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

    for(i = 0; i < THRESHOLD; i++)
    {
        handle->nodes[i].seq -= handle->oldestseq;
    }
    handle->newestseq -= handle->oldestseq;
    handle->oldestseq -= handle->oldestseq;
}

/**
 * get the middle-value of the @handle
 * @param handle: the sort chain handle
 * @param mid: the pointer variable stores the middle-value
 */
static void get_mid(schh_t *handle, schdat_t *mid)
{
    *mid = handle->midnode->data;
}

/**
 * sort chain initialization
 * @param handle: the sort chain handle
 * @param thres: threshhold. the sortchain will pop out oldest data when
 *              the number of data reaches @thres
 *                @sortchain_add act fastest
 * @return: see @schres_t
 */
schres_t sortchain_init(schh_t *handle, unsigned int thres)
{
    if(!handle || !thres || thres > SCH_NODES_TOTAL)
    {
        return SCHRES_ERR;
    }

    memset(handle, 0, sizeof(schh_t));
    handle->thres = thres;
    fifo_init(&handle->odqh, handle->odqb, sizeof(schnode_t *), handle->thres + 1);
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
        if(delete_oldestdata(handle) != SCHRES_OK)
        {
            return SCHRES_ERR;
        }
        if(insert_newestdata(handle, data) != SCHRES_OK)
        {
            return SCHRES_ERR;
        }
        if(is_seqtop(handle) == SCH_TRUE)
        {
            reset_all_seq(handle);
        }
        get_mid(handle, mid);

        print_all(handle, data);
    }
    else
    {
        if(insert_newestdata(handle, data) != SCHRES_OK)
        {
            return SCHRES_ERR;
        }

        print_all(handle, data);

        if(is_full(handle) == SCH_TRUE)
        {
            if(is_seqtop(handle) == SCH_TRUE)
            {
                reset_all_seq(handle);
            }
            get_mid(handle, mid);
            return SCHRES_OK;
        }

        return SCHRES_NOTREADY;
    }

    return SCHRES_OK;
}

