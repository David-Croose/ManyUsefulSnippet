#include "wf.h"
#include <string.h>
#include <stdio.h>

/**
 * write data into fifo.
 * @param node: the node to operate.
 * @param data: the write data.
 */
void fifo_in(struct NODE *node, wf_t data)
{
	node->fifo[node->head] = data;

	if(++node->head >= ARR_CNT(node->fifo))
	{
		node->head = 0;
	}
	if(node->head == node->end)
	{
		if(++node->end >= ARR_CNT(node->fifo))
		{
			node->end = 0;
		}
	}
}

/**
 * read out data from fifo.
 * @param node: the node to operate.
 * @param out: the memory to store the read out data.
 * @return: the result of the read operation.
 */
wf_res_t fifo_out(struct NODE *node, wf_t *out)
{
	if(node->head == node->end)
	{
		return WF_ERR_NM;
	}

	*out = node->fifo[node->end];
	if(++node->end >= ARR_CNT(node->fifo))
	{
		node->end = 0;
	}
	return WF_OK;
}

/**
 * just read fifo, don't drain it.
 * @param node: the node to operate.
 * @param read: the memory to store the read out data.
 * @param cnt: how many units to read.
 * @return: the result of the read operation.
 */
wf_res_t fifo_read(struct NODE *node, wf_t *read, int cnt)
{
	unsigned int _end = node->end;
	int i = 0;

	if(!node || !read || cnt <= 0)
	{
		return WF_ERR_PA;
	}

	if(node->head == node->end)
	{
		return WF_ERR_NM;
	}

	while(_end != node->head)
	{
		read[i] = node->fifo[_end];
		if(++i >= cnt)
		{
			break;
		}
		if(++_end >= ARR_CNT(node->fifo))
		{
			_end = 0;
		}
	}

	return WF_OK;
}

/**
 * reset a fifo.
 * @param node: the node to operate.
 */
void fifo_reset(struct NODE *node)
{
	memset(node, 0, sizeof(*node));
}

/**
 * get fifo capacity.
 * @param node: the node to operate.
 * @return: how many units the fifo contains.
 */
int fifo_cap(struct NODE *node)
{
	unsigned int cap = 0;
	unsigned int _end = node->end;

	while(_end != node->head)
	{
		cap++;
		if(++_end >= ARR_CNT(node->fifo))
		{
			_end = 0;
		}
	}
	return cap;
}

/**
 * get the max units of the fifo.
 * @param node: the node to operate.
 * @return: the max units of the fifo.
 */
int fifo_cnt(struct NODE *node)
{
	return ARR_CNT(node->fifo);
}

int main(void)
{
	struct NODE node;
	wf_t arr[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13};
	wf_t buf[ARR_CNT(node.fifo)] = {0};
	int i, j, cap;
	wf_t out;

	fifo_reset(&node);
	printf("fifo cnt=%d\n", fifo_cnt(&node));

	for(i = 0; i < ARR_CNT(arr); i++)
	{
		printf("write in:%d, ", arr[i]);
		fifo_in(&node, arr[i]);
		cap = fifo_cap(&node);
		printf("fifo cap=%d, ", cap);

		if(fifo_read(&node, buf, cap) == WF_OK)
		{
			for(j = 0; j < cap; j++)
			{
				printf("%d ", buf[j]);
			}
		}
		printf("\n");
	}
	printf("\n");

	cap = fifo_cap(&node);
	printf("read out:\n");
	for(i = 0; i < cap; i++)
	{
		if(fifo_out(&node, &out) == WF_OK)
		{
			printf("%d\n", out);
		}
		printf("fifo cap=%d\n", fifo_cap(&node));
	}
	printf("\n\n");

	cap = fifo_cap(&node);
	printf("fifo cap=%d\n", cap);
	printf("read out:\n");
	for(i = 0; i < ARR_CNT(arr); i++)
	{
		if(fifo_out(&node, &out) == WF_OK)
		{
			printf("%d ", out);
		}
	}
	printf("\n");

	printf("read:\n");
	if(fifo_read(&node, buf, ARR_CNT(buf)) == WF_OK)
	{
		for(j = 0; j < ARR_CNT(buf); j++)
		{
			printf("%d ", buf[j]);
		}
	}
	printf("\n");

	return 0;
}

