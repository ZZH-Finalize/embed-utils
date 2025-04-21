#ifndef __MEM_CONF_H__
#define __MEM_CONF_H__

#ifndef CONFIG_DEFAULT_POOL_SIZE
#define CONFIG_DEFAULT_POOL_SIZE 4 // 4K bytes
#endif

#if defined(CONFIG_ENABLE_TEST_CASES) && !defined(CONFIG_TESTCASE_POOL_SIZE)
#define CONFIG_TESTCASE_POOL_SIZE 2 // 1K bytes
#endif

#endif // __MEM_CONF_H__
