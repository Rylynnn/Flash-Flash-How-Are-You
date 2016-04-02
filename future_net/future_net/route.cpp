
#include "route.h"
#include "lib_record.h"
#include <stdio.h>

//你要完成的功能总入口
void search_route(char *topo[5000], int edge_num, char *demand)
{
    unsigned short result[] = {6,28,34,8,21,15,25,10,13,14,31};//示例中的一个解

    for (int i = 0; i < 11; i++)
        record_result(result[i]);
}
