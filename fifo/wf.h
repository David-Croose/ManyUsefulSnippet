#ifndef _WF_H_
#define _WF_H_

#define CONFIG_WF_FIFO_SIZE    (10)
#define ARR_CNT(x)    (sizeof(x) / sizeof(x[0]))  /* array count */

typedef short wf_t;
typedef enum {
	WF_OK = 0,
	WF_ERR_PA,		// parameter error
	WF_ERR_NM,		// no memory
} wf_res_t;

struct NODE {
	wf_t fifo[CONFIG_WF_FIFO_SIZE];
	unsigned int head;
	unsigned int end;
};

#endif

