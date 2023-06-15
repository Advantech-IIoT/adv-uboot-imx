// SPDX-License-Identifier: GPL-2.0+
/*
 * Copyright (C) 2019 DENX Software Engineering
 * Lukasz Majewski, DENX Software Engineering, lukma@denx.de
 */

#define LOG_CATEGORY UCLASS_CLK

#include <common.h>
#include <clk.h>
#include <clk-uclass.h>
#include <log.h>
#include <dm/device.h>
#include <dm/uclass.h>
#include <dm/lists.h>
#include <dm/device-internal.h>

int clk_register(struct clk *clk, const char *drv_name,
		 const char *name, const char *parent_name)
{
	struct udevice *parent = NULL;
	struct driver *drv;
	int ret;

	if (parent_name) {
		ret = uclass_get_device_by_name(UCLASS_CLK, parent_name, &parent);
		if (ret) {
			log_err("%s: failed to get %s device (parent of %s)\n",
				__func__, parent_name, name);
		} else {
			log_debug("%s: name: %s parent: %s [0x%p]\n", __func__, name,
				  parent->name, parent);
		}
	}

	drv = lists_driver_lookup_name(drv_name);
	if (!drv) {
		log_err("%s: %s is not a valid driver name\n",
			__func__, drv_name);
		return -ENOENT;
	}

	ret = device_bind(parent, drv, name, NULL, ofnode_null(), &clk->dev);
	if (ret) {
		log_err("%s: CLK: %s driver bind error [%d]!\n", __func__, name,
			ret);
		return ret;
	}

	clk->enable_count = 0;

	/* Store back pointer to clk from udevice */
	/* FIXME: This is not allowed...should be allocated by driver model */
	dev_set_uclass_priv(clk->dev, clk);

	return 0;
}

ulong clk_generic_get_rate(struct clk *clk)
{
	return clk_get_parent_rate(clk);
}

const char *clk_hw_get_name(const struct clk *hw)
{
	assert(hw);
	assert(hw->dev);

	return hw->dev->name;
}

bool clk_dev_binded(struct clk *clk)
{
	if (clk->dev && (dev_get_flags(clk->dev) & DM_FLAG_BOUND))
		return true;

	return false;
}