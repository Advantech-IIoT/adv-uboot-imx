/* SPDX-License-Identifier: GPL-2.0+ */
/*
 * Copyright 2018 NXP
 */

#ifndef __IMX8M_ADV_H
#define __IMX8M_ADV_H

#include <linux/sizes.h>
#include <linux/stringify.h>
#include <asm/arch/imx-regs.h>
#include "imx_env.h"

#define ADV_IMX8_DDR_1G		0x0
#define ADV_IMX8_DDR_2G     0x1
#define ADV_IMX8_DDR_4G     0x2

#define TEE_LOAD_ADDR_1G 	0x7e000000
#define TEE_LOAD_ADDR_2G 	0x7e000000
#define TEE_LOAD_ADDR_4G 	0xfe000000

#define CONFIG_SYS_BOOTM_LEN		(64 * SZ_1M)

#define CONFIG_SPL_MAX_SIZE		(156 * 1024)
#define CONFIG_SYS_MONITOR_LEN		(512 * 1024)

#ifdef CONFIG_SPL_BUILD
/*#define CONFIG_ENABLE_DDR_TRAINING_DEBUG*/
#define CONFIG_SPL_STACK		0x187FF0
#define CONFIG_SPL_BSS_START_ADDR      0x00180000
#define CONFIG_SPL_BSS_MAX_SIZE        0x2000	/* 8 KB */
#define CONFIG_SYS_SPL_MALLOC_START    0x42200000
#define CONFIG_SYS_SPL_MALLOC_SIZE    0x80000	/* 512 KB */
#define CONFIG_SYS_SPL_PTE_RAM_BASE    0x41580000

/* malloc f used before GD_FLG_FULL_MALLOC_INIT set */
#define CONFIG_MALLOC_F_ADDR		0x182000
/* For RAW image gives a error info not panic */
#define CONFIG_SPL_ABORT_ON_RAW_IMAGE

#undef CONFIG_DM_MMC

#define CONFIG_POWER_PFUZE100
#define CONFIG_POWER_PFUZE100_I2C_ADDR 0x08
#endif

/* ENET Config */
/* ENET1 */
#if defined(CONFIG_FEC_MXC)
#define CONFIG_ETHPRIME                 "FEC"
#define PHY_ANEG_TIMEOUT 20000

#define CONFIG_FEC_XCV_TYPE             RGMII
#define CONFIG_FEC_MXC_PHYADDR          0

#define IMX_FEC_BASE			0x30BE0000
#endif

#ifdef CONFIG_DISTRO_DEFAULTS
#define BOOT_TARGET_DEVICES(func) \
	func(USB, usb, 0) \
	func(MMC, mmc, 1) \
	func(MMC, mmc, 0)

#include <config_distro_bootcmd.h>
#else
#define BOOTENV
#endif

#if defined(CONFIG_TARGET_IMX8MQ_ECU150)
#define FDTFILE_DEFAULT			"fsl-imx8mq-ecu150.dtb"
#define CONSOLE					"console=ttymxc0,115200 earlycon=ec_imx6q,0x30860000,115200"
#elif defined(CONFIG_TARGET_IMX8MQ_ECU150FL)
#define FDTFILE_DEFAULT			"fsl-imx8mq-ecu150fl.dtb"
#define CONSOLE					"console=ttymxc3,115200 earlycon=ec_imx6q,0x30a60000,115200"
#elif defined(CONFIG_TARGET_IMX8MQ_ECU1370)
#define FDTFILE_DEFAULT			"fsl-imx8mq-ecu1370.dtb"
#define CONSOLE					"console=ttymxc3,115200 earlycon=ec_imx6q,0x30a60000,115200"
#elif defined(CONFIG_TARGET_IMX8MQ_ECU150A1)
#define FDTFILE_DEFAULT			"fsl-imx8mq-ecu150a1.dtb"
#define CONSOLE					"console=ttymxc0,115200 earlycon=ec_imx6q,0x30860000,115200"
#elif defined(CONFIG_TARGET_IMX8MQ_ECU150F)
#define FDTFILE_DEFAULT			"fsl-imx8mq-ecu150f.dtb"
#define CONSOLE					"console=ttymxc0,115200 earlycon=ec_imx6q,0x30860000,115200"
#else
#define FDTFILE_DEFAULT			"fsl-imx8mq-ecu150.dtb"
#define CONSOLE					"console=ttymxc0,115200 earlycon=ec_imx6q,0x30860000,115200"
#endif

/*
 * Another approach is add the clocks for inmates into clks_init_on
 * in clk-imx8mq.c, then clk_ingore_unused could be removed.
 */
#define JAILHOUSE_ENV \
	"jh_clk= \0 " \
	"jh_mmcboot=setenv fdtfile imx8mq-evk-root.dtb; " \
		"setenv jh_clk clk_ignore_unused mem=1872M; " \
			   "if run loadimage; then " \
				   "run mmcboot; " \
			   "else run jh_netboot; fi; \0" \
	"jh_netboot=setenv fdtfile imx8mq-evk-root.dtb; setenv jh_clk clk_ignore_unused mem=1872MB; run netboot; \0 "

#define CONFIG_MFG_ENV_SETTINGS \
	CONFIG_MFG_ENV_SETTINGS_DEFAULT \
	"initrd_addr=0x43800000\0" \
	"initrd_high=0xffffffffffffffff\0" \
	"emmc_dev=0\0"\
	"sd_dev=1\0" \

/* Initial environment variables */
#define CONFIG_EXTRA_ENV_SETTINGS		\
	CONFIG_MFG_ENV_SETTINGS \
	BOOTENV \
	JAILHOUSE_ENV \
	"prepare_mcore=setenv mcore_clk clk-imx8mq.mcore_booted;\0" \
	"scriptaddr=0x43500000\0" \
	"kernel_addr_r=" __stringify(CONFIG_SYS_LOAD_ADDR) "\0" \
	"bsp_script=boot.scr\0" \
	"image=Image\0" \
	"splashimage=0x50000000\0" \
	" "CONSOLE" \0" \
	"fdt_addr_r=0x43000000\0"			\
	"fdt_addr=0x43000000\0"			\
	"fdt_high=0xffffffffffffffff\0"		\
	"boot_fdt=try\0" \
	"fdtfile="FDTFILE_DEFAULT"\0" \
	"bootm_size=0x10000000\0" \
	"mmcdev="__stringify(CONFIG_SYS_MMC_ENV_DEV)"\0" \
	"mmcpart=1\0" \
	"mmcroot=" CONFIG_MMCROOT " rootwait rw\0" \
	"mmcautodetect=yes\0" \
	"mmcargs=setenv bootargs ${jh_clk} ${mcore_clk} console=${console} root=${mmcroot}\0 " \
	"loadbootscript=fatload mmc ${mmcdev}:${mmcpart} ${loadaddr} ${bsp_script};\0" \
	"bootscript=echo Running bootscript from mmc ...; " \
		"source\0" \
	"loadimage=load mmc ${mmcdev}:${mmcpart} ${loadaddr} ${image}\0" \
	"loadfdt=load mmc ${mmcdev}:${mmcpart} ${fdt_addr_r} ${fdtfile}\0" \
	"mmcboot=echo Booting from mmc ...; " \
		"run mmcargs; " \
		"if test ${boot_fdt} = yes || test ${boot_fdt} = try; then " \
			"if run loadfdt; then " \
				"booti ${loadaddr} - ${fdt_addr_r}; " \
			"else " \
				"echo WARN: Cannot load the DT; " \
			"fi; " \
		"else " \
			"echo wait for boot; " \
		"fi;\0" \
    "emmcboot=echo Booting from emmc ...; setenv mmcdev 0; setenv mmcroot /dev/mmcblk0p2 rootwait rw;" \
		"run mmcargs; " \
		"mmc dev ${mmcdev};" \
		"if run loadimage; then " \
			"if run loadfdt; then " \
				"booti ${loadaddr} - ${fdt_addr_r}; " \
			"fi; " \
		"fi;\0" \
	"advufile=advupdate.txt\0" \
	"ramrootfstype=ext2 rootwait\0" \
	"loadramdisk=load mmc ${mmcdev} ${initrd_addr} ramdisk.gz\0" \
	"advrargs=setenv bootargs console=${console} " \
		"${optargs} " \
		"root=/dev/ram0 rw ramdisk_size=65536 "\
		"initrd=${initrd_addr},64M " \
		"rootfstype=${ramrootfstype}\0" \
	"advrfs=echo Advantech recovery file system ramdisk ...; " \
		"setenv devnum ${mmcdev}; " \
		"setenv devtype mmc; " \
		"run loadimage; " \
		"run loadramdisk; " \
		"run advrargs; " \
		"if run loadfdt; then " \
			"booti ${loadaddr} - ${fdt_addr_r}; " \
		"else " \
			"booti ${loadaddr};" \
		"fi\0" \
	"netargs=setenv bootargs ${jh_clk} console=${console} " \
		"root=/dev/nfs " \
		"ip=dhcp nfsroot=${serverip}:${nfsroot},v3,tcp\0" \
	"netboot=echo Booting from net ...; " \
		"run netargs;  " \
		"if test ${ip_dyn} = yes; then " \
			"setenv get_cmd dhcp; " \
		"else " \
			"setenv get_cmd tftp; " \
		"fi; " \
		"${get_cmd} ${loadaddr} ${image}; " \
		"if test ${boot_fdt} = yes || test ${boot_fdt} = try; then " \
			"if ${get_cmd} ${fdt_addr_r} ${fdtfile}; then " \
				"booti ${loadaddr} - ${fdt_addr_r}; " \
			"else " \
				"echo WARN: Cannot load the DT; " \
			"fi; " \
		"else " \
			"booti; " \
		"fi;\0" \
	"bsp_bootcmd=echo Running BSP bootcmd ...; " \
			"mmc dev ${mmcdev}; if mmc rescan; then " \
			   "if run loadbootscript; then " \
				   "run bootscript; " \
			   "else " \
				   "if run loadimage; then " \
					   "run mmcboot; " \
				   "else run netboot; " \
				   "fi; " \
			   "fi; " \
		   "else run emmcboot; fi;"

/* Link Definitions */

#define CONFIG_SYS_INIT_RAM_ADDR        0x40000000
#define CONFIG_SYS_INIT_RAM_SIZE        0x80000
#define CONFIG_SYS_INIT_SP_OFFSET \
	(CONFIG_SYS_INIT_RAM_SIZE - GENERATED_GBL_DATA_SIZE)
#define CONFIG_SYS_INIT_SP_ADDR \
	(CONFIG_SYS_INIT_RAM_ADDR + CONFIG_SYS_INIT_SP_OFFSET)

#define CONFIG_MMCROOT			"/dev/mmcblk1p2"  /* USDHC2 */

#define CONFIG_SYS_SDRAM_BASE           0x40000000
#define PHYS_SDRAM                      0x40000000
#define PHYS_SDRAM_SIZE					0xC0000000 /* 3GB DDR */

#if defined(CONFIG_TARGET_IMX8MQ_ECU150FL) || defined(CONFIG_TARGET_IMX8MQ_ECU150A1) || defined(CONFIG_TARGET_IMX8MQ_ECU1370)
#undef PHYS_SDRAM_SIZE
#define PHYS_SDRAM_SIZE					SZ_4G /* 4GB DDR */
#endif

#if defined(CONFIG_TARGET_IMX8MQ_ECU150FL) || defined(CONFIG_TARGET_IMX8MQ_ECU1370)
#define CONFIG_MXC_UART_BASE		UART4_BASE_ADDR
#else
#define CONFIG_MXC_UART_BASE		UART1_BASE_ADDR
#endif

/* Monitor Command Prompt */
#define CONFIG_SYS_CBSIZE		1024
#define CONFIG_SYS_MAXARGS		64
#define CONFIG_SYS_BARGSIZE		CONFIG_SYS_CBSIZE
#define CONFIG_SYS_PBSIZE		(CONFIG_SYS_CBSIZE + \
					sizeof(CONFIG_SYS_PROMPT) + 16)

#define CONFIG_SYS_FSL_USDHC_NUM	2
#define CONFIG_SYS_FSL_ESDHC_ADDR       0

#define CONFIG_CMD_READ
#define CONFIG_SERIAL_TAG
#define CONFIG_FASTBOOT_USB_DEV 0

#define CONFIG_USB_MAX_CONTROLLER_COUNT         2

#define CONFIG_USBD_HS
#define CONFIG_USB_GADGET_VBUS_DRAW 2

#ifdef CONFIG_ANDROID_SUPPORT
#include "imx8mq_evk_android.h"
#endif

#endif
