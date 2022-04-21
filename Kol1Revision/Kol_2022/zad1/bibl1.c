#include <stdlib.h>
#include <stdio.h>
#include "bibl1.h"

/*napisz biblioteke ladowana dynamicznie przez program zawierajaca funkcje:

int fun1(int a, int b);

zwracajaca sume a+b
oraz funkcje:

int fun2(int a, int b);
zwracajaca iloczyn a*b;
*/

int fun1(int a, int b){
    return a + b;
}

int fun2(int a, int b){
    return a*b;
}

