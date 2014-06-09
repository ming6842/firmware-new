#ifndef __ASSERT_H
#define __ASSERT_H

#if !(defined(NOASSERT))

	#define ASSERT(condition) \
		if(!(condition)) {while(1);}

#endif

#endif
