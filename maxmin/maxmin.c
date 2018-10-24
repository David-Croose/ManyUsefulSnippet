// is it in there
// true --- return 1
// false --- return 0
static int isitinthere(int val, int *buf, unsigned int bufcnt)
{
    unsigned int i;

    for(i = 0; i < bufcnt; i++)
    {
        if(buf[i] == val)
        {
            return 1;
        }
    }
    return 0;
}

// 从数组buf中排除掉数组exc(exclude)中的值后找出最大值
static int get_max(int *buf, unsigned int bufcnt, int *exc, unsigned int exccnt)
{
    unsigned int i;
    int max;

    // 先在buf中，exc以外随便找一个数当做最大值
    for(i = 0; i < bufcnt; i++)
    {
        if(!isitinthere(buf[i], exc, exccnt))
        {
            max = buf[i];
            break;
        }
    }

    for(i = 0; i < bufcnt; i++)
    {
        if(isitinthere(buf[i], exc, exccnt))
        {
            continue;
        }
        if(buf[i] > max)
        {
            max = buf[i];
        }
    }
    return max;
}

// 从数组buf中排除掉数组exc(exclude)中的值后找出最小值
static int get_min(int *buf, unsigned int bufcnt, int *exc, unsigned int exccnt)
{
    unsigned int i;
    int min;

    // 先在buf中，exc以外随便找一个数当做最小值
    for(i = 0; i < bufcnt; i++)
    {
        if(!isitinthere(buf[i], exc, exccnt))
        {
            min = buf[i];
            break;
        }
    }

    for(i = 0; i < bufcnt; i++)
    {
        if(isitinthere(buf[i], exc, exccnt))
        {
            continue;
        }
        if(buf[i] < min)
        {
            min = buf[i];
        }
    }
    return min;
}

// 获取数组中的第n(1,2,3,...)个最大值
int get_nmax(int *buf, unsigned int bufcnt, unsigned int n, int *max)
{
    unsigned int i;
    static int exc[MAXDATA_DELNUM];
    unsigned int exc_seq = 0;

    if(!buf || !bufcnt || !n || !max)
    {
        return 1;
    }
    if(n > bufcnt || n > MAXDATA_DELNUM)
    {
        return 2;
    }

    for(i = 0; i < n; i++)
    {
        *max = get_max(buf, bufcnt, exc, exc_seq);
        exc[exc_seq++] = *max;
    }
    return 0;
}

// 获取数组中的第n(1,2,3,...)个最小值
int get_nmin(int *buf, unsigned int bufcnt, unsigned int n, int *min)
{
    unsigned int i;
    static int exc[MINDATA_DELNUM];
    unsigned int exc_seq = 0;

    if(!buf || !bufcnt || !n || !min)
    {
        return 1;
    }
    if(n > bufcnt || n > MINDATA_DELNUM)
    {
        return 2;
    }

    for(i = 0; i < n; i++)
    {
        *min = get_min(buf, bufcnt, exc, exc_seq);
        exc[exc_seq++] = *min;
    }
    return 0;
}
