#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/cpumask.h>

#include <linux/cpumask.h>
#include <linux/fs.h>
#include <linux/init.h>
#include <linux/interrupt.h>
#include <linux/kernel_stat.h>
#include <linux/proc_fs.h>
#include <linux/sched.h>
#include <linux/sched/stat.h>
#include <linux/seq_file.h>
#include <linux/slab.h>
#include <linux/time.h>
#include <linux/irqnr.h>
#include <linux/sched/cputime.h>
#include <linux/tick.h>


MODULE_LICENSE("GPL");
MODULE_AUTHOR("Thiago F. Pappacena");
MODULE_DESCRIPTION("Module to automatically enable/disable cores of the system depending on the load.");
MODULE_VERSION("0.1.0");

static u64 get_idle_time(struct kernel_cpustat *kcs, int cpu) {
	u64 idle, idle_usecs = -1ULL;

	if (cpu_online(cpu))
		idle_usecs = get_cpu_idle_time_us(cpu, NULL);

	if (idle_usecs == -1ULL)
		/* !NO_HZ or cpu offline so we can rely on cpustat.idle */
		idle = kcs->cpustat[CPUTIME_IDLE];
	else
		idle = idle_usecs * NSEC_PER_USEC;

	return idle;
}


int get_cpu_usage(void) {
    // See fs/proc/stat.c for more details.
    int i, user = 0, nice = 0, system = 0, idle = 0;

    for_each_possible_cpu(i) {
        struct kernel_cpustat kcpustat;
        u64 *cpustat = kcpustat.cpustat;

        kcpustat = kcpustat_cpu(i);

        user		+= cpustat[CPUTIME_USER];
		nice		+= cpustat[CPUTIME_NICE];
		system		+= cpustat[CPUTIME_SYSTEM];
        idle += get_idle_time(&kcpustat, i);
        
        printk(KERN_INFO "Processor %d -> %d / %d / %d / %d", 
                i, user, nice, system, idle);
    }
    return idle;
}

static int __init cpuautoscaling_init(void) {
    // kernel_cpu_stat *kcs;
    printk(KERN_INFO "Hello World! %d/%d (%d%%) :)\n", 
        num_online_cpus(), num_possible_cpus(), get_cpu_usage());
    return 0;
}
static void __exit cpuautoscaling_exit(void) {
    printk(KERN_INFO "Goodbye, World!\n");
}

module_init(cpuautoscaling_init);
module_exit(cpuautoscaling_exit);
