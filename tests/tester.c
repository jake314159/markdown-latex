 /* file minunit_example.c */
 
#include <stdio.h>
#include "minunit.h"
 
#include "test_stringops.h"

int tests_run = 0;

int main(int argc, char **argv) {

    int successCount = 0;
    int failCount = 0;

    if (all_stringops_tests() != 0) {
        failCount++;
    }
    else {
        successCount++;        
    }


    printf("Tests run: %d\n", tests_run);

    if(failCount > 0) {
        printf("SOME TESTS FAILED\n");
    } else {
        printf("ALL TESTS PASSED\n");
    }

    return failCount != 0;
}
