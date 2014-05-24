#ifndef __TEST_H
#define __TEST_H
//#define TEST_ASSERT(message, test) do { if (!(test)) return message; } while (0)
// #define PASS 0
// #define FAILED -1
int16_t test_pass_count, test_failed_count;

#define TEST_ASSERT(test) do { if (!(test)){ test_failed_count++; \
				printf("\r\nTEST FAILED!=> assert(\x1b[31m" #test "\x1b[37m),"\
					"in file:%s, at line %d\r\n"\
					"--------------------------------------------------------------------------------\r\n"\
					, __FILE__, __LINE__); \
				return; \
			} \
			} while (0)
#define mu_run_test(test) do { char *message = test(); tests_run++; \
                                if (message) return message; } while (0)
#endif
