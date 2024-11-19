#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/timer.h>
#include <linux/jiffies.h>
#include <linux/sched/loadavg.h>

MODULE_LICENSE("GPL);
MODULE_AUTHOR("281443-279460");
MODULE_DESCRIPTION("moduł do zasobów");
MODULE_VERSION("1.0");

static unsigned int interval = 10;
module_param(interval, uint, 0644);
MODULE_PARM_DESC(interval, "opóźnienie pomiędzy sprawdzeniami zuzycia procesora");

static struct timer_list cpu_timer;


static void log_cpu_usage(struct timer_list *t)
{
    unsigned long load = avenrun[0]; 
    unsigned long load_percent = load * 100 / FIXED_1;

    printk(KERN_INFO "CPU: %lu%%\n", load_percent);

    mod_timer(&cpu_timer, jiffies + interval * HZ);
}

static int __init monitor_cpu_usage_init(void)
{
    printk(KERN_INFO "włączenie modułu. opóźnienie: %u sekund.\n", interval);

    timer_setup(&cpu_timer, log_cpu_usage, 0);

    mod_timer(&cpu_timer, jiffies + interval * HZ);

    return 0;
}

static void __exit monitor_cpu_usage_exit(void)
{
    printk(KERN_INFO "wyłączenie modułu.\n");

    del_timer(&cpu_timer);
}

module_init(monitor_cpu_usage_init);
module_exit(monitor_cpu_usage_exit);
