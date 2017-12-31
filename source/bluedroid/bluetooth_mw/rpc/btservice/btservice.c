// System header files

#include <signal.h>
#include <stdio.h>
#include <sched.h>

#include "u_bt_mw_types.h"
#include "bt_a2dp_alsa_playback.h"
#include "rh_init_mtk_bt_service.h"
#include "_rpc_ipc_util.h"

//#define MTK_BT_COREDUMP_BACKTRACE 1

#if defined(MTK_BT_COREDUMP_BACKTRACE)
#include <sys/prctl.h>
#include <execinfo.h>
#include <ucontext.h>

#if defined(MTK_BT_SYS_LOG)
__attribute__((visibility("default")))unsigned int ui4_enable_all_log = 0;
#endif


static struct sigaction old_int, old_ill, old_abrt, old_term, old_segv,
old_fpe, old_bus, old_pipe;
static void show_regs(struct sigcontext *regs)
{
    char comm[20] = { '\0' };

    prctl(PR_GET_NAME, comm);
    printf("=============================reg=============================\n");
    printf("Pid: %d, comm: %20s\n", (int)getpid(), comm);
    printf("pc : [<%08lx>]   sp : %08lx\n", regs->arm_pc, regs->arm_sp);
    printf("fp: %08lx   lr : %08lx\n", regs->arm_fp, regs->arm_lr);
    printf("ip: %08lx   cpsr : %08lx\n", regs->arm_ip, regs->arm_cpsr);
    printf("fault_address: %08lx   \n", regs->fault_address);
    printf("error_code: %08lx   \n", regs->error_code);
    printf("r0: %08lx   \n", regs->arm_r0);
    printf("r1: %08lx   \n", regs->arm_r1);
    printf("r2: %08lx   \n", regs->arm_r2);
    printf("r3: %08lx   \n", regs->arm_r3);
    printf("r4: %08lx   \n", regs->arm_r4);
    printf("r5: %08lx   \n", regs->arm_r5);
    printf("r6: %08lx   \n", regs->arm_r6);
    printf("r7: %08lx   \n", regs->arm_r7);
    printf("r8: %08lx   \n", regs->arm_r8);
    printf("r9: %08lx   \n", regs->arm_r9);
    printf("r10: %08lx   \n", regs->arm_r10);

    printf("==========================================================\n");

}

static void dump_stack(void)
{
    void *trace[128];
    char **messages;
    int i, trace_size;
    printf("=============================backtrace=============================\n");

    trace_size = backtrace(trace, 128);
    printf("obtained %d stack frames:\n",trace_size);

    messages = backtrace_symbols(trace, trace_size);

    if (messages != NULL)
    {

        printf("backtrace:\n");


        for (i = 0; i < trace_size; i++)
            printf("[%d]%s\n", i,messages[i]);

        free(messages);
    }
    printf("=============================================================\n");
}

static void sighandler(int signo, siginfo_t *info, void *context)
{
    ucontext_t *uc = (ucontext_t *)context;

    switch (signo)
    {

    case SIGSEGV:
    case SIGABRT:
        printf("=========================SIGSEGV=SIGABRT=============================\n");
        printf("Invalid memory reference %d", signo);
        printf("\tpc: 0x%x, addr: 0x%x\n", (unsigned int)(void *)(uc->uc_mcontext.arm_pc), (unsigned int)info->si_addr);
        show_regs(&uc->uc_mcontext);
        dump_stack();
        printf("Using default signal handler.\n");

        //while (1) pause();
        usleep(50*1000);
        sigaction(SIGSEGV, &old_segv, NULL);
        //exit(0);

    break;

    case SIGTERM:
        printf("==========================SIGTERM===================================\n");
        printf("Termination signal SIGTERM");
        printf("Using default signal handler.\n");

        sigaction(SIGTERM, &old_term, NULL);
        exit(0);
    break;


    case SIGPIPE:
        printf("==========================SIGPIPE===================================\n");
        printf("SIGPIPE signal SIGPIPE");
        printf("Using default signal handler.\n");

        sigaction(SIGPIPE, &old_pipe, NULL);

    break;

    default:
        printf("%d Unexpected signal", signo);

        show_regs(&uc->uc_mcontext);

        dump_stack();

        while (1) pause();

        break;
    }
}
#endif

int main(int argc, char **argv)
{
    printf("Mtk_bt_service server start:\n");
    struct sched_param param;
    int i4_ret = 0;
#if defined(MTK_BT_COREDUMP_BACKTRACE)
    struct sigaction sa;
    int ret;

#if defined(MTK_BT_SYS_LOG)
    /*init output log type*/
    if (0 == access("/data/log_all", 0))
    {
        printf("enable all ouput in btservice!!\n");
        ui4_enable_all_log = 1;
    }

#endif

    printf("sighandler\n");
    //system("ulimit -c unlimited");
    //system("echo core.%e.%p > /proc/sys/kernel/core_pattern");

    sa.sa_sigaction = &sighandler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_SIGINFO;

    if ((ret = sigaction(SIGINT, &sa, &old_int)) != 0) return ret;
    if ((ret = sigaction(SIGILL, &sa, &old_ill)) != 0) return ret;
    if ((ret = sigaction(SIGABRT, &sa, &old_abrt)) != 0) return ret;
    if ((ret = sigaction(SIGTERM, &sa, &old_term)) != 0) return ret;
    if ((ret = sigaction(SIGFPE, &sa, &old_fpe)) != 0) return ret;
    if ((ret = sigaction(SIGBUS, &sa, &old_bus)) != 0) return ret;
    sa.sa_flags |= SA_ONSTACK;
    if ((ret = sigaction(SIGSEGV, &sa, &old_segv)) != 0) return ret;
    sa.sa_handler = SIG_IGN;
    if ((ret = sigaction(SIGPIPE, &sa, &old_pipe)) != 0) return ret;
#endif
    param.sched_priority = 95;
    printf("increase the priority of bluedroid.");

    i4_ret = sched_setscheduler(0, SCHED_RR, &param);
    printf("i4_ret:%d @ %s\n", i4_ret, __FUNCTION__);
    if (-1 == i4_ret)
    {
        printf("bluedroid sched_setscheduler error\n");
    }
    else
    {
        printf("set bluedroid priority thread done\n");
    }

    c_rpc_init_mtk_bt_service_server();
    bt_rpcu_tl_log_start();
    c_rpc_start_mtk_bt_service_server();

    pause();

    return 0;
}
