/*
 * omap4-common.h: OMAP4 specific common header file
 *
 * Copyright (C) 2010 Texas Instruments, Inc.
 *
 * Author:
 *	Santosh Shilimkar <santosh.shilimkar@ti.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */
#ifndef OMAP_ARCH_OMAP4_COMMON_H
#define OMAP_ARCH_OMAP4_COMMON_H

#include <asm/proc-fns.h>
/*
 * Secure low power context save/restore API index
 */
#define HAL_SAVESECURERAM_INDEX		0x1a
#define HAL_SAVEHW_INDEX		0x1b
#define HAL_SAVEALL_INDEX		0x1c
#define HAL_SAVEGIC_INDEX		0x1d

/*
 * Secure HAL, PPA services available
 */
#define PPA_SERVICE_0			0x21
#define PPA_SERVICE_PL310_POR		0x23
#define PPA_SERVICE_DEFAULT_POR_NS_SMP	0x25
/*
 * Secure HAL API flags
 */
#define FLAG_START_CRITICAL		0x4
#define FLAG_IRQFIQ_MASK		0x3
#define FLAG_IRQ_ENABLE			0x2
#define FLAG_FIQ_ENABLE			0x1
#define NO_FLAG				0x0

/*
 * Secure HAL API entry ids
 */
#define HAL_DIAGREG_0                   0x114
/*
 * SAR restore phase USB HOST static port
 * configuration
 */
#define OMAP4_USBHOST_CLKSEL_UTMI_P2_INT_P1_INT		0x0
#define OMAP4_USBHOST_CLKSEL_UTMI_P2_INT_P1_EXT		0x1
#define OMAP4_USBHOST_CLKSEL_UTMI_P2_EXT_P1_INT		0x2
#define OMAP4_USBHOST_CLKSEL_UTMI_P2_EXT_P1_EXT		0x3

#ifndef __ASSEMBLER__

#ifdef CONFIG_CACHE_L2X0
extern void __iomem *omap4_get_l2cache_base(void);
#endif

#ifdef CONFIG_SMP
extern void __iomem *omap4_get_scu_base(void);
#else
static inline void __iomem *omap4_get_scu_base(void)
{
	return NULL;
}
#endif

extern void __iomem *omap4_get_gic_dist_base(void);
extern void __iomem *omap4_get_gic_cpu_base(void);
extern void __iomem *omap4_get_sar_ram_base(void);
extern void *omap_get_dram_barrier_base(void);
extern dma_addr_t omap4_secure_ram_phys;
extern void __init gic_init_irq(void);
extern void gic_cpu_enable(void);
extern void gic_cpu_disable(void);
extern void gic_dist_enable(void);
extern void gic_dist_disable(void);
extern u32 gic_cpu_read(u32 reg);
extern void omap_smc1(u32 fn, u32 arg);
extern void omap_bus_sync(void);
extern void omap_do_wfi(void);

extern bool gic_dist_disabled(void);
extern void gic_timer_retrigger(void);

#ifdef CONFIG_OMAP4_DPLL_CASCADING
extern bool omap4_abe_can_enter_dpll_cascading(void);
extern bool omap4_is_in_dpll_cascading(void);
#else
static inline bool omap4_abe_can_enter_dpll_cascading(void)
{
	return false;
}

static inline bool omap4_is_in_dpll_cascading(void)
{
	return false;
}
#endif
struct device;
extern int omap4_dpll_cascading_blocker_hold(struct device *dev);
extern int omap4_dpll_cascading_blocker_release(struct device *dev);

/*
 * Read MPIDR: Multiprocessor affinity register
 */
static inline unsigned int hard_smp_processor_id(void)
{
	unsigned int cpunum;

	asm volatile (
	"mrc	 p15, 0, %0, c0, c0, 5\n"
		: "=r" (cpunum));
	return cpunum &= 0x0f;
}

#ifdef CONFIG_SMP
/* Needed for secondary core boot */
extern void omap_secondary_startup(void);
extern u32 omap_modify_auxcoreboot0(u32 set_mask, u32 clear_mask);
extern void omap_auxcoreboot_addr(u32 cpu_addr);
extern u32 omap_read_auxcoreboot0(void);
#ifdef CONFIG_PM
extern int omap4_mpuss_init(void);
extern int omap4_enter_lowpower(unsigned int cpu, unsigned int power_state);
extern void omap4_cpu_suspend(unsigned int cpu, unsigned int save_state);
extern void omap4_cpu_resume(void);
extern u32 omap_smc2(u32 id, u32 falg, u32 pargs);
extern u32 omap4_secure_dispatcher(u32 idx, u32 flag, u32 nargs,
				u32 arg1, u32 arg2, u32 arg3, u32 arg4);
#else
static inline int omap4_enter_lowpower(unsigned int cpu,
					unsigned int power_state)
{
	cpu_do_idle();
	return 0;
}

static inline int omap4_mpuss_init(void)
{
	return 0;
}

static inline void omap4_cpu_suspend(unsigned int cpu, unsigned int save_state)
{
}

static inline void omap4_cpu_resume(void)
{
}

static inline u32 omap_smc2(u32 id, u32 falg, u32 pargs)
{
	return 0;
}
static inline u32 omap4_secure_dispatcher(u32 idx, u32 flag, u32 nargs,
				u32 arg1, u32 arg2, u32 arg3, u32 arg4)
{
	return 0;
}
#endif	/* CONFIG_PM */
#endif	/* CONFIG_SMP */

extern u32 omap_get_arm_rev(void);

/*
 * omap4_get_diagctrl0_errata_flags: Routine to get the flag value of the CP15
 * diagnostic control register.
 *
 * Return the value to be set into the CP15 Diagnostic control 0 reg
 */
static inline unsigned int omap4_get_diagctrl0_errata_flags(void)
{
	unsigned int ret  = 0;
	u32 arm_rev = omap_get_arm_rev();
#ifdef CONFIG_OMAP4_ARM_ERRATA_742230
	if ((arm_rev >= 0x10) && (arm_rev <= 0x22))
		ret |= (1 << 4);
#endif
#ifdef CONFIG_OMAP4_ARM_ERRATA_751472
	if (arm_rev < 0x30)	/* ARM revision less that r3p0 */
		ret |= (1 << 11);
#endif
#ifdef CONFIG_OMAP4_ARM_ERRATA_743622
	if ((arm_rev & 0xF0) == 0x20)	/* All ARM rev r2px impacted */
		ret |= (1 << 6);
#endif
	return ret;
}

extern int omap4_prcm_freq_update(void);

#ifdef CONFIG_PM
extern int omap4_sar_save(void);
extern void omap4_usb_sar_restore(void);
extern void omap4_sar_overwrite(void);
extern void omap4_sar_usbhost_init(u32 fck_source);
#else
void omap4_sar_save(void)
{
}
void omap4_usb_sar_restore(void)
{
}
void omap4_sar_overwrite(void)
{
}
void omap4_sar_usbhost_init(u32 fck_source)
{
}
#endif

#endif /* __ASSEMBLER__ */
#endif /* OMAP_ARCH_OMAP4_COMMON_H */
