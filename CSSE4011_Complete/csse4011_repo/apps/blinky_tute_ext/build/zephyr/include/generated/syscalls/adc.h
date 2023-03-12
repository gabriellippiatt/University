
/* auto-generated by gen_syscalls.py, don't edit */
#ifndef Z_INCLUDE_SYSCALLS_ADC_H
#define Z_INCLUDE_SYSCALLS_ADC_H


#include <tracing/tracing_syscall.h>

#ifndef _ASMLANGUAGE

#include <syscall_list.h>
#include <syscall.h>

#include <linker/sections.h>


#if __GNUC__ > 4 || (__GNUC__ == 4 && __GNUC_MINOR__ >= 6)
#pragma GCC diagnostic push
#endif

#ifdef __GNUC__
#pragma GCC diagnostic ignored "-Wstrict-aliasing"
#if !defined(__XCC__)
#pragma GCC diagnostic ignored "-Warray-bounds"
#endif
#endif

#ifdef __cplusplus
extern "C" {
#endif

extern int z_impl_adc_channel_setup(const struct device * dev, const struct adc_channel_cfg * channel_cfg);

__pinned_func
static inline int adc_channel_setup(const struct device * dev, const struct adc_channel_cfg * channel_cfg)
{
#ifdef CONFIG_USERSPACE
	if (z_syscall_trap()) {
		/* coverity[OVERRUN] */
		return (int) arch_syscall_invoke2(*(uintptr_t *)&dev, *(uintptr_t *)&channel_cfg, K_SYSCALL_ADC_CHANNEL_SETUP);
	}
#endif
	compiler_barrier();
	return z_impl_adc_channel_setup(dev, channel_cfg);
}

#if (CONFIG_TRACING_SYSCALL == 1)
#ifndef DISABLE_SYSCALL_TRACING

#define adc_channel_setup(dev, channel_cfg) ({ 	int retval; 	sys_port_trace_syscall_enter(K_SYSCALL_ADC_CHANNEL_SETUP, adc_channel_setup, dev, channel_cfg); 	retval = adc_channel_setup(dev, channel_cfg); 	sys_port_trace_syscall_exit(K_SYSCALL_ADC_CHANNEL_SETUP, adc_channel_setup, dev, channel_cfg, retval); 	retval; })
#endif
#endif


extern int z_impl_adc_read(const struct device * dev, const struct adc_sequence * sequence);

__pinned_func
static inline int adc_read(const struct device * dev, const struct adc_sequence * sequence)
{
#ifdef CONFIG_USERSPACE
	if (z_syscall_trap()) {
		/* coverity[OVERRUN] */
		return (int) arch_syscall_invoke2(*(uintptr_t *)&dev, *(uintptr_t *)&sequence, K_SYSCALL_ADC_READ);
	}
#endif
	compiler_barrier();
	return z_impl_adc_read(dev, sequence);
}

#if (CONFIG_TRACING_SYSCALL == 1)
#ifndef DISABLE_SYSCALL_TRACING

#define adc_read(dev, sequence) ({ 	int retval; 	sys_port_trace_syscall_enter(K_SYSCALL_ADC_READ, adc_read, dev, sequence); 	retval = adc_read(dev, sequence); 	sys_port_trace_syscall_exit(K_SYSCALL_ADC_READ, adc_read, dev, sequence, retval); 	retval; })
#endif
#endif


extern int z_impl_adc_read_async(const struct device * dev, const struct adc_sequence * sequence, struct k_poll_signal * async);

__pinned_func
static inline int adc_read_async(const struct device * dev, const struct adc_sequence * sequence, struct k_poll_signal * async)
{
#ifdef CONFIG_USERSPACE
	if (z_syscall_trap()) {
		/* coverity[OVERRUN] */
		return (int) arch_syscall_invoke3(*(uintptr_t *)&dev, *(uintptr_t *)&sequence, *(uintptr_t *)&async, K_SYSCALL_ADC_READ_ASYNC);
	}
#endif
	compiler_barrier();
	return z_impl_adc_read_async(dev, sequence, async);
}

#if (CONFIG_TRACING_SYSCALL == 1)
#ifndef DISABLE_SYSCALL_TRACING

#define adc_read_async(dev, sequence, async) ({ 	int retval; 	sys_port_trace_syscall_enter(K_SYSCALL_ADC_READ_ASYNC, adc_read_async, dev, sequence, async); 	retval = adc_read_async(dev, sequence, async); 	sys_port_trace_syscall_exit(K_SYSCALL_ADC_READ_ASYNC, adc_read_async, dev, sequence, async, retval); 	retval; })
#endif
#endif


#ifdef __cplusplus
}
#endif

#if __GNUC__ > 4 || (__GNUC__ == 4 && __GNUC_MINOR__ >= 6)
#pragma GCC diagnostic pop
#endif

#endif
#endif /* include guard */