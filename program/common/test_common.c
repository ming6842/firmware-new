#include "bound.h"
#include <stdio.h>
#include "test.h"
#include "test_common.h"
void test_bound(void)
{	

	float f_max = 100.0f;
	float f_min = 50.2f;
	float f = f_max + 100.5f;


	f = bound_float(f, f_min, f_max);
	TEST_ASSERT( (f==f_max));

	f = f_max - 50.0f;
	f = bound_float(f, f_min, f_max);
	TEST_ASSERT( ( (f<=f_max) &&( f>=f_min) ));

	f = f_min - 333.25f;
	f = bound_float(f, f_min, f_max);
	TEST_ASSERT( f == f_min);

	uint16_t _u16_max = 1200;
	uint16_t _u16_min =  400;
	uint16_t _u16 = _u16_max +300;
	_u16 = bound_uint16(_u16, _u16_min, _u16_max);
	TEST_ASSERT( (_u16==_u16_max));

	_u16 = _u16_max - 200;
	_u16 = bound_uint16(_u16, _u16_min, _u16_max);
	TEST_ASSERT( ( (_u16<=_u16_max) &&( _u16>=_u16_min) ));

	_u16 = _u16_min - 200;
	_u16 = bound_uint16(_u16, _u16_min, _u16_max);
	TEST_ASSERT( (_u16==_u16_min));


	
	int16_t _16_max = 1200;
	int16_t _16_min =  -400;
	int16_t _16 = _16_max+200;
	_16 = bound_int16(_16, _16_min, _16_max);
	TEST_ASSERT( (_16==_16_max));

	_16 = _16_max - 200;
	_16 = bound_int16(_16, _16_min, _16_max);
	TEST_ASSERT( ( (_16<=_16_max) &&( _16>=_16_min) ));

	_16 = _16_min -200;
	_16 = bound_int16(_16, _16_min, _16_max);
	TEST_ASSERT( (_16==_16_min));

	uint32_t _u32_max = 0x00ffffff;
	uint32_t _u32_min = 0x0000ffff;
	uint32_t _u32 = _u32_max +200 ;
	_u32 = bound_uint32(_u32, _u32_min, _u32_max);
	TEST_ASSERT( (_u32==_u32_max));

	_u32 = _u32_max - 200;
	_u32 = bound_uint32(_u32, _u32_min, _u32_max);
	TEST_ASSERT( ( (_u32<=_u32_max) &&( _u32>=_u32_min) ));

	_u32 = _u32_min - 200;
	_u32 = bound_uint32(_u32, _u32_min, _u32_max);
	TEST_ASSERT( (_u32==_u32_min));


	int32_t _32_max =  2000000000;
	int32_t _32_min = -2000000000;
	int32_t _32 = _32_max + 200;
	_32 = bound_int32(_32, _32_min, _32_max);
	TEST_ASSERT( (_32==_32_max));

	_32 = _32_max - 200;
	_32 = bound_int32(_32, _32_min, _32_max);
	TEST_ASSERT( ( (_32<=_32_max) &&( _32>=_32_min) ));

	_32 = _32_min - 200;
	_32 = bound_int32(_32, _32_min, _32_max);
	TEST_ASSERT( (_32==_32_min));

	uint64_t _u64_max = 0x0fffffffffffffff;
	uint64_t _u64_min = 0x00ffffffffffffff;
	uint64_t _u64 = _u64_max + 200;
	_u64 = bound_uint64(_u64, _u64_min, _u64_max);
	TEST_ASSERT( (_u64==_u64_max));

	_u64 = _u64_max-5000;
	_u64 = bound_uint64(_u64, _u64_min, _u64_max);
	TEST_ASSERT( ( (_u64<=_u64_max) &&( _u64>=_u64_min) ));

	_u64 = _u64_min-200;
	_u64 = bound_uint64(_u64, _u64_min, _u64_max);
	TEST_ASSERT( (_u64==_u64_min));


	int64_t _64 = 0x0fffffffffffffff;
	int64_t _64_max = 0x0fffffffffffffff;
	int64_t _64_min = 0x000000000000ffff;
	_64 = bound_int64(_64, _64_min, _64_max);
	TEST_ASSERT( (_64==_64_max));

	_64 = _64_max - 400;
	_64 = bound_int64(_64, _64_min, _64_max);
	TEST_ASSERT( ( (_64<=_64_max) &&( _64>=_64_min) ));

	_64 = _64_min - 200;
	_64 = bound_int64(_64, _64_min, _64_max);
	TEST_ASSERT( (_64==_64_min));











	printf("pass!\r\n");



}