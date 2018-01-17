SRCS_COMMON_C=  debug.c \
	     	memory.c \
	     	conversion.c \
		circular_buffer.c \
		platform.c \
		logger_queue.c \
		logger_helper.c \
		logger.c \
		data_processing.c \
		timestamp.c \
		time_profiler.c \
		project3.c \
	    	main.c 

SRCS_KL25Z_S=	startup_MKL25Z4.S

SRCS_KL25Z_C=	system_MKL25Z4.c \
		mcg.c \
		gpio.c \
		uart0.c \
		dma.c \
		nordic_driver.c \
		spi.c \
		timer0.c \

TEST_SRCS = \
	test_memory.c \
	test_conversion.c \
	test_circular.c \
	test.c
