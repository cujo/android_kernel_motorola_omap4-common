/*
 * Author: Manuel Manz alias dtrail <mnl.manz@gmail.com>
 *
 * Copyright 2013 Manuel Manz
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 */

#include <linux/module.h>
#include <linux/kobject.h>
#include <linux/sysfs.h>
#include <linux/earlysuspend.h>
#include <linux/mutex.h>
#include <linux/battery_friend.h>

#define BATTERY_FRIEND_VERSION_MAJOR 1
#define BATTERY_FRIEND_VERSION_MINOR 1

static DEFINE_MUTEX(battery_mutex);

bool battery_friend_early_suspend_active __read_mostly;
bool battery_friend_active __read_mostly = false;

unsigned int /*scr_on_min = 300000,*/ scr_off_max = 700000, scr_min = 200000; /*scr_max = 1000000;*/

// temporary frequency table for input range
unsigned int freq_1 = 100000;
unsigned int freq_2 = 200000;
unsigned int freq_3 = 300000;
unsigned int freq_4 = 400000;
unsigned int freq_5 = 500000;
unsigned int freq_6 = 600000;
unsigned int freq_7 = 700000;
unsigned int freq_8 = 800000;
unsigned int freq_9 = 900000;
unsigned int freq_10 = 1000000;
unsigned int freq_11 = 1100000;
unsigned int freq_12 = 1200000;
unsigned int freq_13 = 1300000;

static ssize_t battery_friend_active_show(struct kobject *kobj,
		struct kobj_attribute *attr, char *buf)
{
	return sprintf(buf, "%u\n", (battery_friend_active ? 1 : 0));
}

static ssize_t battery_friend_active_store(struct kobject *kobj,
		struct kobj_attribute *attr, const char *buf, size_t count)
{
	unsigned int data;

	if (sscanf(buf, "%u\n", &data) == 1) {
		if (data == 1) {
			pr_info("%s: Battery_Friend enabled\n", __FUNCTION__);
			battery_friend_active = true;
		}
		else if (data == 0) {
			pr_info("%s: Battery_Friend disabled\n", __FUNCTION__);
			battery_friend_active = false;
		}
		else
			pr_info("%s: bad value: %u\n", __FUNCTION__, data);
	} else
		pr_info("%s: unknown input!\n", __FUNCTION__);

	return count;
}

/* // Screen on min freq
static ssize_t battery_friend_screen_on_min_freq_show(struct kobject *kobj,
		struct kobj_attribute *attr, char *buf)
{
	return sprintf(buf, "%u\n", scr_on_min);
}

static ssize_t battery_friend_screen_on_min_freq_store(struct kobject *kobj,
		struct kobj_attribute *attr, const char *buf, size_t count)
{
	unsigned int data;

		if(sscanf(buf, "%u\n", &data)) {
			if (data < 100000) {
			pr_info("%s: bad value: %u\n", __FUNCTION__, data);
			}
	   		else if (data > 1300000) {
				pr_info("%s: bad value: %u\n", __FUNCTION__, data);
				}
		} else {
			pr_info("%s: battery friend screen_on_min_freq set to: %u\n", __FUNCTION__, data);
			scr_on_min = data;
		}

	return count;
} */

// Screen off max freq
static ssize_t battery_friend_screen_off_max_freq_show(struct kobject *kobj,
		struct kobj_attribute *attr, char *buf)
{
	return sprintf(buf, "%u\n", scr_off_max);
}

static ssize_t battery_friend_screen_off_max_freq_store(struct kobject *kobj,
		struct kobj_attribute *attr, const char *buf, size_t size)
{
	unsigned int data;

		(sscanf(buf, "%u\n", &data));

		if (data == freq_1 || data == freq_2 || data == freq_3 || data == freq_4 || data == freq_5 || data == freq_6 || data == freq_7 || data == freq_8 || data == freq_9 || data == freq_10 || data == freq_11 || data == freq_12 || data == freq_13) {
			pr_info("%s: screen_off_max_freq set to: %u Mhz\n", __FUNCTION__, data/1000);
			scr_off_max = data;
		}
		else {
			pr_info("%s: bad value: %u\n", __FUNCTION__, data);
			}

	return size;
}

// min freq
static ssize_t battery_friend_min_freq_show(struct kobject *kobj,
		struct kobj_attribute *attr, char *buf)
{
	return sprintf(buf, "%u\n", scr_min);
}

static ssize_t battery_friend_min_freq_store(struct kobject *kobj,
		struct kobj_attribute *attr, const char *buf, size_t size)
{
	unsigned int data;

		(sscanf(buf, "%u\n", &data));

		if (data == freq_1 || data == freq_2 || data == freq_3 || data == freq_4 || data == freq_5 || data == freq_6 || data == freq_7 || data == freq_8 || data == freq_9 || data == freq_10 || data == freq_11 || data == freq_12 || data == freq_13) {
			pr_info("%s: min_freq set to: %u Mhz\n", __FUNCTION__, data/1000);
			scr_min = data;
		}
		else {
			pr_info("%s: bad value: %u\n", __FUNCTION__, data);
			}

	return size;
}

// max freq
/*static ssize_t battery_friend_max_freq_show(struct kobject *kobj,
		struct kobj_attribute *attr, char *buf)
{
	return sprintf(buf, "%u\n", scr_max);
}

static ssize_t battery_friend_max_freq_store(struct kobject *kobj,
		struct kobj_attribute *attr, const char *buf, size_t count)
{
	unsigned int data;

		if (sscanf(buf, "%u\n", &data) < 100000) {
			pr_info("%s: bad value: %u\n", __FUNCTION__, data);
			}
	   	else if (sscanf(buf, "%u\n", &data) > 1300000) {
				pr_info("%s: bad value: %u\n", __FUNCTION__, data);
				}
		} else {
			pr_info("%s: battery friend max_freq set to: %u Mhz\n", __FUNCTION__, data/1000);
			scr_max = data;
		}
	
	return count;
} */

static ssize_t battery_friend_version_show(struct kobject *kobj,
		struct kobj_attribute *attr, char *buf)
{
	return sprintf(buf, "version: %u.%u by dtrail\n",
		BATTERY_FRIEND_VERSION_MAJOR,
		BATTERY_FRIEND_VERSION_MINOR);
}

static ssize_t battery_friend_earlysuspend_show(struct kobject *kobj,
		struct kobj_attribute *attr, char *buf)
{
	return sprintf(buf, "early suspend active: %u\n", battery_friend_early_suspend_active);
}

static struct kobj_attribute battery_friend_active_attribute = 
	__ATTR(battery_friend_active, 0755,
		battery_friend_active_show,
		battery_friend_active_store);

/*static struct kobj_attribute battery_friend_screen_on_min_freq_attribute = 
	__ATTR(battery_friend_screen_on_min_freq, 0666,
		battery_friend_screen_on_min_freq_show,
		battery_friend_screen_on_min_freq_store); */

static struct kobj_attribute battery_friend_screen_off_max_freq_attribute = 
	__ATTR(battery_friend_screen_off_max_freq, 0755,
		battery_friend_screen_off_max_freq_show,
		battery_friend_screen_off_max_freq_store);

static struct kobj_attribute battery_friend_min_freq_attribute = 
	__ATTR(battery_friend_min_freq, 0755,
		battery_friend_min_freq_show,
		battery_friend_min_freq_store);

/*static struct kobj_attribute battery_friend_max_freq_attribute = 
	__ATTR(battery_friend_max_freq, 0755,
		battery_friend_max_freq_show,
		battery_friend_max_freq_store); */

static struct kobj_attribute battery_friend_version_attribute = 
	__ATTR(battery_friend_version, 0444, battery_friend_version_show, NULL);

static struct kobj_attribute battery_friend_earlysuspend_attribute = 
	__ATTR(battery_friend_earlysuspend, 0444, battery_friend_earlysuspend_show, NULL);

static struct attribute *battery_friend_active_attrs[] =
	{
		&battery_friend_active_attribute.attr,
	//	&battery_friend_screen_on_min_freq_attribute.attr,
		&battery_friend_screen_off_max_freq_attribute.attr,
	//	&battery_friend_max_freq_attribute.attr,
		&battery_friend_min_freq_attribute.attr,
		&battery_friend_version_attribute.attr,
		&battery_friend_earlysuspend_attribute.attr,
		NULL,
	};

static struct attribute_group battery_friend_active_attr_group =
	{
		.attrs = battery_friend_active_attrs,
	};

static struct kobject *battery_friend_kobj;

static void battery_friend_early_suspend(struct early_suspend *h)
{
	mutex_lock(&battery_mutex);
	if (battery_friend_active) {
		battery_friend_early_suspend_active = true;
#if 1
		
		/* Nothing atm */

#endif
	}
	mutex_unlock(&battery_mutex);
}

static void battery_friend_late_resume(struct early_suspend *h)
{
	mutex_lock(&battery_mutex);
	battery_friend_early_suspend_active = false;
	mutex_unlock(&battery_mutex);
}

static struct early_suspend battery_friend_early_suspend_handler = 
	{
		.level = EARLY_SUSPEND_LEVEL_BLANK_SCREEN,
		.suspend = battery_friend_early_suspend,
		.resume = battery_friend_late_resume,
	};

static int battery_friend_init(void)
{
	int sysfs_result;


	battery_friend_kobj = kobject_create_and_add("battery_friend", kernel_kobj);
	if (!battery_friend_kobj) {
		pr_err("%s battery_friend kobject create failed!\n", __FUNCTION__);
		return -ENOMEM;
        }

	sysfs_result = sysfs_create_group(battery_friend_kobj,
			&battery_friend_active_attr_group);

        if (sysfs_result) {
		pr_info("%s battery_friend sysfs create failed!\n", __FUNCTION__);
		kobject_put(battery_friend_kobj);
	}
	return sysfs_result;
}

static void battery_friend_exit(void)
{
	unregister_early_suspend(&battery_friend_early_suspend_handler);

	if (battery_friend_kobj != NULL)
		kobject_put(battery_friend_kobj);
}

module_init(battery_friend_init);
module_exit(battery_friend_exit);
