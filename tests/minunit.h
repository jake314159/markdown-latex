 /* file: minunit.h                               */
 /* Minimal unit tester                           */
 /* http://www.jera.com/techinfo/jtns/jtn002.html */
#define mu_assert(message, test) do { if (!(test)) return message; } while (0)
#define mu_run_test(test) do { char *message = test(); tests_run++; \
if (message == 0) {printf("[SUCCESS]\n");} \
else { printf("[Fail] %s\n", message); return message; }\
} while (0)
extern int tests_run;
