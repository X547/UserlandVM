#define SYSCALL_COUNT 286

#ifndef _ASSEMBLER

#include <TypeConstants.h>

const int kSyscallCount = SYSCALL_COUNT;

const extended_syscall_info kExtendedSyscallInfos[] = {
	{
		"_kern_is_computer_on", 0,
		{ 4, 8, B_INT32_TYPE },
		{
		}
	},
	{
		"_kern_generic_syscall", 4,
		{ 4, 8, B_INT32_TYPE },
		{
			{ 0, 8, 8, B_STRING_TYPE },
			{ 8, 4, 8, B_INT32_TYPE },
			{ 16, 8, 8, B_POINTER_TYPE },
			{ 24, 8, 8, B_INT64_TYPE },
		}
	},
	{
		"_kern_getrlimit", 2,
		{ 4, 8, B_INT32_TYPE },
		{
			{ 0, 4, 8, B_INT32_TYPE },
			{ 8, 8, 8, B_POINTER_TYPE },
		}
	},
	{
		"_kern_setrlimit", 2,
		{ 4, 8, B_INT32_TYPE },
		{
			{ 0, 4, 8, B_INT32_TYPE },
			{ 8, 8, 8, B_POINTER_TYPE },
		}
	},
	{
		"_kern_shutdown", 1,
		{ 4, 8, B_INT32_TYPE },
		{
			{ 0, 1, 8, B_INT8_TYPE },
		}
	},
	{
		"_kern_get_safemode_option", 3,
		{ 4, 8, B_INT32_TYPE },
		{
			{ 0, 8, 8, B_STRING_TYPE },
			{ 8, 8, 8, B_POINTER_TYPE },
			{ 16, 8, 8, B_POINTER_TYPE },
		}
	},
	{
		"_kern_wait_for_objects", 4,
		{ 8, 8, B_INT64_TYPE },
		{
			{ 0, 8, 8, B_POINTER_TYPE },
			{ 8, 4, 8, B_INT32_TYPE },
			{ 16, 4, 8, B_INT32_TYPE },
			{ 24, 8, 8, B_INT64_TYPE },
		}
	},
	{
		"_kern_event_queue_create", 1,
		{ 4, 8, B_INT32_TYPE },
		{
			{ 0, 4, 8, B_INT32_TYPE },
		}
	},
	{
		"_kern_event_queue_select", 3,
		{ 4, 8, B_INT32_TYPE },
		{
			{ 0, 4, 8, B_INT32_TYPE },
			{ 8, 8, 8, B_POINTER_TYPE },
			{ 16, 4, 8, B_INT32_TYPE },
		}
	},
	{
		"_kern_event_queue_wait", 5,
		{ 8, 8, B_INT64_TYPE },
		{
			{ 0, 4, 8, B_INT32_TYPE },
			{ 8, 8, 8, B_POINTER_TYPE },
			{ 16, 4, 8, B_INT32_TYPE },
			{ 24, 4, 8, B_INT32_TYPE },
			{ 32, 8, 8, B_INT64_TYPE },
		}
	},
	{
		"_kern_mutex_lock", 4,
		{ 4, 8, B_INT32_TYPE },
		{
			{ 0, 8, 8, B_POINTER_TYPE },
			{ 8, 8, 8, B_STRING_TYPE },
			{ 16, 4, 8, B_INT32_TYPE },
			{ 24, 8, 8, B_INT64_TYPE },
		}
	},
	{
		"_kern_mutex_unblock", 2,
		{ 4, 8, B_INT32_TYPE },
		{
			{ 0, 8, 8, B_POINTER_TYPE },
			{ 8, 4, 8, B_INT32_TYPE },
		}
	},
	{
		"_kern_mutex_switch_lock", 6,
		{ 4, 8, B_INT32_TYPE },
		{
			{ 0, 8, 8, B_POINTER_TYPE },
			{ 8, 4, 8, B_INT32_TYPE },
			{ 16, 8, 8, B_POINTER_TYPE },
			{ 24, 8, 8, B_STRING_TYPE },
			{ 32, 4, 8, B_INT32_TYPE },
			{ 40, 8, 8, B_INT64_TYPE },
		}
	},
	{
		"_kern_mutex_sem_acquire", 4,
		{ 4, 8, B_INT32_TYPE },
		{
			{ 0, 8, 8, B_POINTER_TYPE },
			{ 8, 8, 8, B_STRING_TYPE },
			{ 16, 4, 8, B_INT32_TYPE },
			{ 24, 8, 8, B_INT64_TYPE },
		}
	},
	{
		"_kern_mutex_sem_release", 2,
		{ 4, 8, B_INT32_TYPE },
		{
			{ 0, 8, 8, B_POINTER_TYPE },
			{ 8, 4, 8, B_INT32_TYPE },
		}
	},
	{
		"_kern_create_sem", 2,
		{ 4, 8, B_INT32_TYPE },
		{
			{ 0, 4, 8, B_INT32_TYPE },
			{ 8, 8, 8, B_STRING_TYPE },
		}
	},
	{
		"_kern_delete_sem", 1,
		{ 4, 8, B_INT32_TYPE },
		{
			{ 0, 4, 8, B_INT32_TYPE },
		}
	},
	{
		"_kern_switch_sem", 2,
		{ 4, 8, B_INT32_TYPE },
		{
			{ 0, 4, 8, B_INT32_TYPE },
			{ 8, 4, 8, B_INT32_TYPE },
		}
	},
	{
		"_kern_switch_sem_etc", 5,
		{ 4, 8, B_INT32_TYPE },
		{
			{ 0, 4, 8, B_INT32_TYPE },
			{ 8, 4, 8, B_INT32_TYPE },
			{ 16, 4, 8, B_INT32_TYPE },
			{ 24, 4, 8, B_INT32_TYPE },
			{ 32, 8, 8, B_INT64_TYPE },
		}
	},
	{
		"_kern_acquire_sem", 1,
		{ 4, 8, B_INT32_TYPE },
		{
			{ 0, 4, 8, B_INT32_TYPE },
		}
	},
	{
		"_kern_acquire_sem_etc", 4,
		{ 4, 8, B_INT32_TYPE },
		{
			{ 0, 4, 8, B_INT32_TYPE },
			{ 8, 4, 8, B_INT32_TYPE },
			{ 16, 4, 8, B_INT32_TYPE },
			{ 24, 8, 8, B_INT64_TYPE },
		}
	},
	{
		"_kern_release_sem", 1,
		{ 4, 8, B_INT32_TYPE },
		{
			{ 0, 4, 8, B_INT32_TYPE },
		}
	},
	{
		"_kern_release_sem_etc", 3,
		{ 4, 8, B_INT32_TYPE },
		{
			{ 0, 4, 8, B_INT32_TYPE },
			{ 8, 4, 8, B_INT32_TYPE },
			{ 16, 4, 8, B_INT32_TYPE },
		}
	},
	{
		"_kern_get_sem_count", 2,
		{ 4, 8, B_INT32_TYPE },
		{
			{ 0, 4, 8, B_INT32_TYPE },
			{ 8, 8, 8, B_POINTER_TYPE },
		}
	},
	{
		"_kern_get_sem_info", 3,
		{ 4, 8, B_INT32_TYPE },
		{
			{ 0, 4, 8, B_INT32_TYPE },
			{ 8, 8, 8, B_POINTER_TYPE },
			{ 16, 8, 8, B_INT64_TYPE },
		}
	},
	{
		"_kern_get_next_sem_info", 4,
		{ 4, 8, B_INT32_TYPE },
		{
			{ 0, 4, 8, B_INT32_TYPE },
			{ 8, 8, 8, B_POINTER_TYPE },
			{ 16, 8, 8, B_POINTER_TYPE },
			{ 24, 8, 8, B_INT64_TYPE },
		}
	},
	{
		"_kern_set_sem_owner", 2,
		{ 4, 8, B_INT32_TYPE },
		{
			{ 0, 4, 8, B_INT32_TYPE },
			{ 8, 4, 8, B_INT32_TYPE },
		}
	},
	{
		"_kern_realtime_sem_open", 6,
		{ 4, 8, B_INT32_TYPE },
		{
			{ 0, 8, 8, B_STRING_TYPE },
			{ 8, 4, 8, B_INT32_TYPE },
			{ 16, 4, 8, B_INT32_TYPE },
			{ 24, 4, 8, B_INT32_TYPE },
			{ 32, 8, 8, B_POINTER_TYPE },
			{ 40, 8, 8, B_POINTER_TYPE },
		}
	},
	{
		"_kern_realtime_sem_close", 2,
		{ 4, 8, B_INT32_TYPE },
		{
			{ 0, 4, 8, B_INT32_TYPE },
			{ 8, 8, 8, B_POINTER_TYPE },
		}
	},
	{
		"_kern_realtime_sem_unlink", 1,
		{ 4, 8, B_INT32_TYPE },
		{
			{ 0, 8, 8, B_STRING_TYPE },
		}
	},
	{
		"_kern_realtime_sem_get_value", 2,
		{ 4, 8, B_INT32_TYPE },
		{
			{ 0, 4, 8, B_INT32_TYPE },
			{ 8, 8, 8, B_POINTER_TYPE },
		}
	},
	{
		"_kern_realtime_sem_post", 1,
		{ 4, 8, B_INT32_TYPE },
		{
			{ 0, 4, 8, B_INT32_TYPE },
		}
	},
	{
		"_kern_realtime_sem_wait", 3,
		{ 4, 8, B_INT32_TYPE },
		{
			{ 0, 4, 8, B_INT32_TYPE },
			{ 8, 4, 8, B_INT32_TYPE },
			{ 16, 8, 8, B_INT64_TYPE },
		}
	},
	{
		"_kern_xsi_semget", 3,
		{ 4, 8, B_INT32_TYPE },
		{
			{ 0, 4, 8, B_INT32_TYPE },
			{ 8, 4, 8, B_INT32_TYPE },
			{ 16, 4, 8, B_INT32_TYPE },
		}
	},
	{
		"_kern_xsi_semctl", 4,
		{ 4, 8, B_INT32_TYPE },
		{
			{ 0, 4, 8, B_INT32_TYPE },
			{ 8, 4, 8, B_INT32_TYPE },
			{ 16, 4, 8, B_INT32_TYPE },
			{ 24, 8, 8, B_POINTER_TYPE },
		}
	},
	{
		"_kern_xsi_semop", 3,
		{ 4, 8, B_INT32_TYPE },
		{
			{ 0, 4, 8, B_INT32_TYPE },
			{ 8, 8, 8, B_POINTER_TYPE },
			{ 16, 8, 8, B_INT64_TYPE },
		}
	},
	{
		"_kern_xsi_msgctl", 3,
		{ 4, 8, B_INT32_TYPE },
		{
			{ 0, 4, 8, B_INT32_TYPE },
			{ 8, 4, 8, B_INT32_TYPE },
			{ 16, 8, 8, B_POINTER_TYPE },
		}
	},
	{
		"_kern_xsi_msgget", 2,
		{ 4, 8, B_INT32_TYPE },
		{
			{ 0, 4, 8, B_INT32_TYPE },
			{ 8, 4, 8, B_INT32_TYPE },
		}
	},
	{
		"_kern_xsi_msgrcv", 5,
		{ 8, 8, B_INT64_TYPE },
		{
			{ 0, 4, 8, B_INT32_TYPE },
			{ 8, 8, 8, B_POINTER_TYPE },
			{ 16, 8, 8, B_INT64_TYPE },
			{ 24, 8, 8, B_INT64_TYPE },
			{ 32, 4, 8, B_INT32_TYPE },
		}
	},
	{
		"_kern_xsi_msgsnd", 4,
		{ 4, 8, B_INT32_TYPE },
		{
			{ 0, 4, 8, B_INT32_TYPE },
			{ 8, 8, 8, B_POINTER_TYPE },
			{ 16, 8, 8, B_INT64_TYPE },
			{ 24, 4, 8, B_INT32_TYPE },
		}
	},
	{
		"_kern_load_image", 8,
		{ 4, 8, B_INT32_TYPE },
		{
			{ 0, 8, 8, B_POINTER_TYPE },
			{ 8, 8, 8, B_INT64_TYPE },
			{ 16, 4, 8, B_INT32_TYPE },
			{ 24, 4, 8, B_INT32_TYPE },
			{ 32, 4, 8, B_INT32_TYPE },
			{ 40, 4, 8, B_INT32_TYPE },
			{ 48, 4, 8, B_INT32_TYPE },
			{ 56, 4, 8, B_INT32_TYPE },
		}
	},
	{
		"_kern_exit_team", 1,
		{ 0, 0, B_RAW_TYPE },
		{
			{ 0, 4, 8, B_INT32_TYPE },
		}
	},
	{
		"_kern_kill_team", 1,
		{ 4, 8, B_INT32_TYPE },
		{
			{ 0, 4, 8, B_INT32_TYPE },
		}
	},
	{
		"_kern_get_current_team", 0,
		{ 4, 8, B_INT32_TYPE },
		{
		}
	},
	{
		"_kern_wait_for_team", 2,
		{ 4, 8, B_INT32_TYPE },
		{
			{ 0, 4, 8, B_INT32_TYPE },
			{ 8, 8, 8, B_POINTER_TYPE },
		}
	},
	{
		"_kern_wait_for_child", 4,
		{ 4, 8, B_INT32_TYPE },
		{
			{ 0, 4, 8, B_INT32_TYPE },
			{ 8, 4, 8, B_INT32_TYPE },
			{ 16, 8, 8, B_POINTER_TYPE },
			{ 24, 8, 8, B_POINTER_TYPE },
		}
	},
	{
		"_kern_exec", 6,
		{ 4, 8, B_INT32_TYPE },
		{
			{ 0, 8, 8, B_STRING_TYPE },
			{ 8, 8, 8, B_POINTER_TYPE },
			{ 16, 8, 8, B_INT64_TYPE },
			{ 24, 4, 8, B_INT32_TYPE },
			{ 32, 4, 8, B_INT32_TYPE },
			{ 40, 4, 8, B_INT32_TYPE },
		}
	},
	{
		"_kern_fork", 0,
		{ 4, 8, B_INT32_TYPE },
		{
		}
	},
	{
		"_kern_process_info", 2,
		{ 4, 8, B_INT32_TYPE },
		{
			{ 0, 4, 8, B_INT32_TYPE },
			{ 8, 4, 8, B_INT32_TYPE },
		}
	},
	{
		"_kern_setpgid", 2,
		{ 4, 8, B_INT32_TYPE },
		{
			{ 0, 4, 8, B_INT32_TYPE },
			{ 8, 4, 8, B_INT32_TYPE },
		}
	},
	{
		"_kern_setsid", 0,
		{ 4, 8, B_INT32_TYPE },
		{
		}
	},
	{
		"_kern_change_root", 1,
		{ 4, 8, B_INT32_TYPE },
		{
			{ 0, 8, 8, B_STRING_TYPE },
		}
	},
	{
		"_kern_spawn_thread", 1,
		{ 4, 8, B_INT32_TYPE },
		{
			{ 0, 8, 8, B_POINTER_TYPE },
		}
	},
	{
		"_kern_find_thread", 1,
		{ 4, 8, B_INT32_TYPE },
		{
			{ 0, 8, 8, B_STRING_TYPE },
		}
	},
	{
		"_kern_suspend_thread", 1,
		{ 4, 8, B_INT32_TYPE },
		{
			{ 0, 4, 8, B_INT32_TYPE },
		}
	},
	{
		"_kern_resume_thread", 1,
		{ 4, 8, B_INT32_TYPE },
		{
			{ 0, 4, 8, B_INT32_TYPE },
		}
	},
	{
		"_kern_rename_thread", 2,
		{ 4, 8, B_INT32_TYPE },
		{
			{ 0, 4, 8, B_INT32_TYPE },
			{ 8, 8, 8, B_STRING_TYPE },
		}
	},
	{
		"_kern_set_thread_priority", 2,
		{ 4, 8, B_INT32_TYPE },
		{
			{ 0, 4, 8, B_INT32_TYPE },
			{ 8, 4, 8, B_INT32_TYPE },
		}
	},
	{
		"_kern_kill_thread", 1,
		{ 4, 8, B_INT32_TYPE },
		{
			{ 0, 4, 8, B_INT32_TYPE },
		}
	},
	{
		"_kern_exit_thread", 1,
		{ 0, 0, B_RAW_TYPE },
		{
			{ 0, 4, 8, B_INT32_TYPE },
		}
	},
	{
		"_kern_cancel_thread", 2,
		{ 4, 8, B_INT32_TYPE },
		{
			{ 0, 4, 8, B_INT32_TYPE },
			{ 8, 8, 8, B_POINTER_TYPE },
		}
	},
	{
		"_kern_thread_yield", 0,
		{ 0, 0, B_RAW_TYPE },
		{
		}
	},
	{
		"_kern_wait_for_thread", 2,
		{ 4, 8, B_INT32_TYPE },
		{
			{ 0, 4, 8, B_INT32_TYPE },
			{ 8, 8, 8, B_POINTER_TYPE },
		}
	},
	{
		"_kern_wait_for_thread_etc", 4,
		{ 4, 8, B_INT32_TYPE },
		{
			{ 0, 4, 8, B_INT32_TYPE },
			{ 8, 4, 8, B_INT32_TYPE },
			{ 16, 8, 8, B_INT64_TYPE },
			{ 24, 8, 8, B_POINTER_TYPE },
		}
	},
	{
		"_kern_has_data", 1,
		{ 1, 8, B_INT8_TYPE },
		{
			{ 0, 4, 8, B_INT32_TYPE },
		}
	},
	{
		"_kern_send_data", 4,
		{ 4, 8, B_INT32_TYPE },
		{
			{ 0, 4, 8, B_INT32_TYPE },
			{ 8, 4, 8, B_INT32_TYPE },
			{ 16, 8, 8, B_POINTER_TYPE },
			{ 24, 8, 8, B_INT64_TYPE },
		}
	},
	{
		"_kern_receive_data", 3,
		{ 4, 8, B_INT32_TYPE },
		{
			{ 0, 8, 8, B_POINTER_TYPE },
			{ 8, 8, 8, B_POINTER_TYPE },
			{ 16, 8, 8, B_INT64_TYPE },
		}
	},
	{
		"_kern_restore_signal_frame", 1,
		{ 8, 8, B_INT64_TYPE },
		{
			{ 0, 8, 8, B_POINTER_TYPE },
		}
	},
	{
		"_kern_get_thread_info", 2,
		{ 4, 8, B_INT32_TYPE },
		{
			{ 0, 4, 8, B_INT32_TYPE },
			{ 8, 8, 8, B_POINTER_TYPE },
		}
	},
	{
		"_kern_get_next_thread_info", 3,
		{ 4, 8, B_INT32_TYPE },
		{
			{ 0, 4, 8, B_INT32_TYPE },
			{ 8, 8, 8, B_POINTER_TYPE },
			{ 16, 8, 8, B_POINTER_TYPE },
		}
	},
	{
		"_kern_get_team_info", 3,
		{ 4, 8, B_INT32_TYPE },
		{
			{ 0, 4, 8, B_INT32_TYPE },
			{ 8, 8, 8, B_POINTER_TYPE },
			{ 16, 8, 8, B_INT64_TYPE },
		}
	},
	{
		"_kern_get_next_team_info", 3,
		{ 4, 8, B_INT32_TYPE },
		{
			{ 0, 8, 8, B_POINTER_TYPE },
			{ 8, 8, 8, B_POINTER_TYPE },
			{ 16, 8, 8, B_INT64_TYPE },
		}
	},
	{
		"_kern_get_team_usage_info", 4,
		{ 4, 8, B_INT32_TYPE },
		{
			{ 0, 4, 8, B_INT32_TYPE },
			{ 8, 4, 8, B_INT32_TYPE },
			{ 16, 8, 8, B_POINTER_TYPE },
			{ 24, 8, 8, B_INT64_TYPE },
		}
	},
	{
		"_kern_get_extended_team_info", 5,
		{ 4, 8, B_INT32_TYPE },
		{
			{ 0, 4, 8, B_INT32_TYPE },
			{ 8, 4, 8, B_INT32_TYPE },
			{ 16, 8, 8, B_POINTER_TYPE },
			{ 24, 8, 8, B_INT64_TYPE },
			{ 32, 8, 8, B_POINTER_TYPE },
		}
	},
	{
		"_kern_get_cpu", 0,
		{ 4, 8, B_INT32_TYPE },
		{
		}
	},
	{
		"_kern_get_thread_affinity", 3,
		{ 4, 8, B_INT32_TYPE },
		{
			{ 0, 4, 8, B_INT32_TYPE },
			{ 8, 8, 8, B_POINTER_TYPE },
			{ 16, 8, 8, B_INT64_TYPE },
		}
	},
	{
		"_kern_set_thread_affinity", 3,
		{ 4, 8, B_INT32_TYPE },
		{
			{ 0, 4, 8, B_INT32_TYPE },
			{ 8, 8, 8, B_POINTER_TYPE },
			{ 16, 8, 8, B_INT64_TYPE },
		}
	},
	{
		"_kern_start_watching_system", 4,
		{ 4, 8, B_INT32_TYPE },
		{
			{ 0, 4, 8, B_INT32_TYPE },
			{ 8, 4, 8, B_INT32_TYPE },
			{ 16, 4, 8, B_INT32_TYPE },
			{ 24, 4, 8, B_INT32_TYPE },
		}
	},
	{
		"_kern_stop_watching_system", 4,
		{ 4, 8, B_INT32_TYPE },
		{
			{ 0, 4, 8, B_INT32_TYPE },
			{ 8, 4, 8, B_INT32_TYPE },
			{ 16, 4, 8, B_INT32_TYPE },
			{ 24, 4, 8, B_INT32_TYPE },
		}
	},
	{
		"_kern_block_thread", 2,
		{ 4, 8, B_INT32_TYPE },
		{
			{ 0, 4, 8, B_INT32_TYPE },
			{ 8, 8, 8, B_INT64_TYPE },
		}
	},
	{
		"_kern_unblock_thread", 2,
		{ 4, 8, B_INT32_TYPE },
		{
			{ 0, 4, 8, B_INT32_TYPE },
			{ 8, 4, 8, B_INT32_TYPE },
		}
	},
	{
		"_kern_unblock_threads", 3,
		{ 4, 8, B_INT32_TYPE },
		{
			{ 0, 8, 8, B_POINTER_TYPE },
			{ 8, 4, 8, B_INT32_TYPE },
			{ 16, 4, 8, B_INT32_TYPE },
		}
	},
	{
		"_kern_estimate_max_scheduling_latency", 1,
		{ 8, 8, B_INT64_TYPE },
		{
			{ 0, 4, 8, B_INT32_TYPE },
		}
	},
	{
		"_kern_set_scheduler_mode", 1,
		{ 4, 8, B_INT32_TYPE },
		{
			{ 0, 4, 8, B_INT32_TYPE },
		}
	},
	{
		"_kern_get_scheduler_mode", 0,
		{ 4, 8, B_INT32_TYPE },
		{
		}
	},
	{
		"_kern_getgid", 1,
		{ 4, 8, B_INT32_TYPE },
		{
			{ 0, 1, 8, B_INT8_TYPE },
		}
	},
	{
		"_kern_getuid", 1,
		{ 4, 8, B_INT32_TYPE },
		{
			{ 0, 1, 8, B_INT8_TYPE },
		}
	},
	{
		"_kern_setregid", 3,
		{ 4, 8, B_INT32_TYPE },
		{
			{ 0, 4, 8, B_INT32_TYPE },
			{ 8, 4, 8, B_INT32_TYPE },
			{ 16, 1, 8, B_INT8_TYPE },
		}
	},
	{
		"_kern_setreuid", 3,
		{ 4, 8, B_INT32_TYPE },
		{
			{ 0, 4, 8, B_INT32_TYPE },
			{ 8, 4, 8, B_INT32_TYPE },
			{ 16, 1, 8, B_INT8_TYPE },
		}
	},
	{
		"_kern_getgroups", 2,
		{ 8, 8, B_INT64_TYPE },
		{
			{ 0, 4, 8, B_INT32_TYPE },
			{ 8, 8, 8, B_POINTER_TYPE },
		}
	},
	{
		"_kern_setgroups", 2,
		{ 4, 8, B_INT32_TYPE },
		{
			{ 0, 4, 8, B_INT32_TYPE },
			{ 8, 8, 8, B_POINTER_TYPE },
		}
	},
	{
		"_kern_send_signal", 4,
		{ 4, 8, B_INT32_TYPE },
		{
			{ 0, 4, 8, B_INT32_TYPE },
			{ 8, 4, 8, B_INT32_TYPE },
			{ 16, 8, 8, B_POINTER_TYPE },
			{ 24, 4, 8, B_INT32_TYPE },
		}
	},
	{
		"_kern_set_signal_mask", 3,
		{ 4, 8, B_INT32_TYPE },
		{
			{ 0, 4, 8, B_INT32_TYPE },
			{ 8, 8, 8, B_POINTER_TYPE },
			{ 16, 8, 8, B_POINTER_TYPE },
		}
	},
	{
		"_kern_sigaction", 3,
		{ 4, 8, B_INT32_TYPE },
		{
			{ 0, 4, 8, B_INT32_TYPE },
			{ 8, 8, 8, B_POINTER_TYPE },
			{ 16, 8, 8, B_POINTER_TYPE },
		}
	},
	{
		"_kern_sigwait", 4,
		{ 4, 8, B_INT32_TYPE },
		{
			{ 0, 8, 8, B_POINTER_TYPE },
			{ 8, 8, 8, B_POINTER_TYPE },
			{ 16, 4, 8, B_INT32_TYPE },
			{ 24, 8, 8, B_INT64_TYPE },
		}
	},
	{
		"_kern_sigsuspend", 1,
		{ 4, 8, B_INT32_TYPE },
		{
			{ 0, 8, 8, B_POINTER_TYPE },
		}
	},
	{
		"_kern_sigpending", 1,
		{ 4, 8, B_INT32_TYPE },
		{
			{ 0, 8, 8, B_POINTER_TYPE },
		}
	},
	{
		"_kern_set_signal_stack", 2,
		{ 4, 8, B_INT32_TYPE },
		{
			{ 0, 8, 8, B_POINTER_TYPE },
			{ 8, 8, 8, B_POINTER_TYPE },
		}
	},
	{
		"_kern_register_image", 2,
		{ 4, 8, B_INT32_TYPE },
		{
			{ 0, 8, 8, B_POINTER_TYPE },
			{ 8, 8, 8, B_INT64_TYPE },
		}
	},
	{
		"_kern_unregister_image", 1,
		{ 4, 8, B_INT32_TYPE },
		{
			{ 0, 4, 8, B_INT32_TYPE },
		}
	},
	{
		"_kern_image_relocated", 1,
		{ 0, 0, B_RAW_TYPE },
		{
			{ 0, 4, 8, B_INT32_TYPE },
		}
	},
	{
		"_kern_loading_app_failed", 1,
		{ 0, 0, B_RAW_TYPE },
		{
			{ 0, 4, 8, B_INT32_TYPE },
		}
	},
	{
		"_kern_get_image_info", 3,
		{ 4, 8, B_INT32_TYPE },
		{
			{ 0, 4, 8, B_INT32_TYPE },
			{ 8, 8, 8, B_POINTER_TYPE },
			{ 16, 8, 8, B_INT64_TYPE },
		}
	},
	{
		"_kern_get_next_image_info", 4,
		{ 4, 8, B_INT32_TYPE },
		{
			{ 0, 4, 8, B_INT32_TYPE },
			{ 8, 8, 8, B_POINTER_TYPE },
			{ 16, 8, 8, B_POINTER_TYPE },
			{ 24, 8, 8, B_INT64_TYPE },
		}
	},
	{
		"_kern_read_kernel_image_symbols", 6,
		{ 4, 8, B_INT32_TYPE },
		{
			{ 0, 4, 8, B_INT32_TYPE },
			{ 8, 8, 8, B_POINTER_TYPE },
			{ 16, 8, 8, B_POINTER_TYPE },
			{ 24, 8, 8, B_POINTER_TYPE },
			{ 32, 8, 8, B_POINTER_TYPE },
			{ 40, 8, 8, B_POINTER_TYPE },
		}
	},
	{
		"_kern_mount", 6,
		{ 4, 8, B_INT32_TYPE },
		{
			{ 0, 8, 8, B_STRING_TYPE },
			{ 8, 8, 8, B_STRING_TYPE },
			{ 16, 8, 8, B_STRING_TYPE },
			{ 24, 4, 8, B_INT32_TYPE },
			{ 32, 8, 8, B_STRING_TYPE },
			{ 40, 8, 8, B_INT64_TYPE },
		}
	},
	{
		"_kern_unmount", 2,
		{ 4, 8, B_INT32_TYPE },
		{
			{ 0, 8, 8, B_STRING_TYPE },
			{ 8, 4, 8, B_INT32_TYPE },
		}
	},
	{
		"_kern_read_fs_info", 2,
		{ 4, 8, B_INT32_TYPE },
		{
			{ 0, 4, 8, B_INT32_TYPE },
			{ 8, 8, 8, B_POINTER_TYPE },
		}
	},
	{
		"_kern_write_fs_info", 3,
		{ 4, 8, B_INT32_TYPE },
		{
			{ 0, 4, 8, B_INT32_TYPE },
			{ 8, 8, 8, B_POINTER_TYPE },
			{ 16, 4, 8, B_INT32_TYPE },
		}
	},
	{
		"_kern_next_device", 1,
		{ 4, 8, B_INT32_TYPE },
		{
			{ 0, 8, 8, B_POINTER_TYPE },
		}
	},
	{
		"_kern_sync", 0,
		{ 4, 8, B_INT32_TYPE },
		{
		}
	},
	{
		"_kern_entry_ref_to_path", 5,
		{ 4, 8, B_INT32_TYPE },
		{
			{ 0, 4, 8, B_INT32_TYPE },
			{ 8, 8, 8, B_INT64_TYPE },
			{ 16, 8, 8, B_STRING_TYPE },
			{ 24, 8, 8, B_POINTER_TYPE },
			{ 32, 8, 8, B_INT64_TYPE },
		}
	},
	{
		"_kern_normalize_path", 3,
		{ 4, 8, B_INT32_TYPE },
		{
			{ 0, 8, 8, B_STRING_TYPE },
			{ 8, 1, 8, B_INT8_TYPE },
			{ 16, 8, 8, B_POINTER_TYPE },
		}
	},
	{
		"_kern_open_entry_ref", 5,
		{ 4, 8, B_INT32_TYPE },
		{
			{ 0, 4, 8, B_INT32_TYPE },
			{ 8, 8, 8, B_INT64_TYPE },
			{ 16, 8, 8, B_STRING_TYPE },
			{ 24, 4, 8, B_INT32_TYPE },
			{ 32, 4, 8, B_INT32_TYPE },
		}
	},
	{
		"_kern_open", 4,
		{ 4, 8, B_INT32_TYPE },
		{
			{ 0, 4, 8, B_INT32_TYPE },
			{ 8, 8, 8, B_STRING_TYPE },
			{ 16, 4, 8, B_INT32_TYPE },
			{ 24, 4, 8, B_INT32_TYPE },
		}
	},
	{
		"_kern_open_dir_entry_ref", 3,
		{ 4, 8, B_INT32_TYPE },
		{
			{ 0, 4, 8, B_INT32_TYPE },
			{ 8, 8, 8, B_INT64_TYPE },
			{ 16, 8, 8, B_STRING_TYPE },
		}
	},
	{
		"_kern_open_dir", 2,
		{ 4, 8, B_INT32_TYPE },
		{
			{ 0, 4, 8, B_INT32_TYPE },
			{ 8, 8, 8, B_STRING_TYPE },
		}
	},
	{
		"_kern_open_parent_dir", 3,
		{ 4, 8, B_INT32_TYPE },
		{
			{ 0, 4, 8, B_INT32_TYPE },
			{ 8, 8, 8, B_POINTER_TYPE },
			{ 16, 8, 8, B_INT64_TYPE },
		}
	},
	{
		"_kern_fcntl", 3,
		{ 4, 8, B_INT32_TYPE },
		{
			{ 0, 4, 8, B_INT32_TYPE },
			{ 8, 4, 8, B_INT32_TYPE },
			{ 16, 8, 8, B_INT64_TYPE },
		}
	},
	{
		"_kern_fsync", 1,
		{ 4, 8, B_INT32_TYPE },
		{
			{ 0, 4, 8, B_INT32_TYPE },
		}
	},
	{
		"_kern_flock", 2,
		{ 4, 8, B_INT32_TYPE },
		{
			{ 0, 4, 8, B_INT32_TYPE },
			{ 8, 4, 8, B_INT32_TYPE },
		}
	},
	{
		"_kern_seek", 3,
		{ 8, 8, B_INT64_TYPE },
		{
			{ 0, 4, 8, B_INT32_TYPE },
			{ 8, 8, 8, B_INT64_TYPE },
			{ 16, 4, 8, B_INT32_TYPE },
		}
	},
	{
		"_kern_create_dir_entry_ref", 4,
		{ 4, 8, B_INT32_TYPE },
		{
			{ 0, 4, 8, B_INT32_TYPE },
			{ 8, 8, 8, B_INT64_TYPE },
			{ 16, 8, 8, B_STRING_TYPE },
			{ 24, 4, 8, B_INT32_TYPE },
		}
	},
	{
		"_kern_create_dir", 3,
		{ 4, 8, B_INT32_TYPE },
		{
			{ 0, 4, 8, B_INT32_TYPE },
			{ 8, 8, 8, B_STRING_TYPE },
			{ 16, 4, 8, B_INT32_TYPE },
		}
	},
	{
		"_kern_remove_dir", 2,
		{ 4, 8, B_INT32_TYPE },
		{
			{ 0, 4, 8, B_INT32_TYPE },
			{ 8, 8, 8, B_STRING_TYPE },
		}
	},
	{
		"_kern_read_link", 4,
		{ 4, 8, B_INT32_TYPE },
		{
			{ 0, 4, 8, B_INT32_TYPE },
			{ 8, 8, 8, B_STRING_TYPE },
			{ 16, 8, 8, B_POINTER_TYPE },
			{ 24, 8, 8, B_POINTER_TYPE },
		}
	},
	{
		"_kern_create_symlink", 4,
		{ 4, 8, B_INT32_TYPE },
		{
			{ 0, 4, 8, B_INT32_TYPE },
			{ 8, 8, 8, B_STRING_TYPE },
			{ 16, 8, 8, B_STRING_TYPE },
			{ 24, 4, 8, B_INT32_TYPE },
		}
	},
	{
		"_kern_create_link", 5,
		{ 4, 8, B_INT32_TYPE },
		{
			{ 0, 4, 8, B_INT32_TYPE },
			{ 8, 8, 8, B_STRING_TYPE },
			{ 16, 4, 8, B_INT32_TYPE },
			{ 24, 8, 8, B_STRING_TYPE },
			{ 32, 1, 8, B_INT8_TYPE },
		}
	},
	{
		"_kern_unlink", 2,
		{ 4, 8, B_INT32_TYPE },
		{
			{ 0, 4, 8, B_INT32_TYPE },
			{ 8, 8, 8, B_STRING_TYPE },
		}
	},
	{
		"_kern_rename", 4,
		{ 4, 8, B_INT32_TYPE },
		{
			{ 0, 4, 8, B_INT32_TYPE },
			{ 8, 8, 8, B_STRING_TYPE },
			{ 16, 4, 8, B_INT32_TYPE },
			{ 24, 8, 8, B_STRING_TYPE },
		}
	},
	{
		"_kern_create_fifo", 3,
		{ 4, 8, B_INT32_TYPE },
		{
			{ 0, 4, 8, B_INT32_TYPE },
			{ 8, 8, 8, B_STRING_TYPE },
			{ 16, 4, 8, B_INT32_TYPE },
		}
	},
	{
		"_kern_create_pipe", 1,
		{ 4, 8, B_INT32_TYPE },
		{
			{ 0, 8, 8, B_POINTER_TYPE },
		}
	},
	{
		"_kern_access", 4,
		{ 4, 8, B_INT32_TYPE },
		{
			{ 0, 4, 8, B_INT32_TYPE },
			{ 8, 8, 8, B_STRING_TYPE },
			{ 16, 4, 8, B_INT32_TYPE },
			{ 24, 1, 8, B_INT8_TYPE },
		}
	},
	{
		"_kern_select", 6,
		{ 8, 8, B_INT64_TYPE },
		{
			{ 0, 4, 8, B_INT32_TYPE },
			{ 8, 8, 8, B_POINTER_TYPE },
			{ 16, 8, 8, B_POINTER_TYPE },
			{ 24, 8, 8, B_POINTER_TYPE },
			{ 32, 8, 8, B_INT64_TYPE },
			{ 40, 8, 8, B_POINTER_TYPE },
		}
	},
	{
		"_kern_poll", 4,
		{ 8, 8, B_INT64_TYPE },
		{
			{ 0, 8, 8, B_POINTER_TYPE },
			{ 8, 4, 8, B_INT32_TYPE },
			{ 16, 8, 8, B_INT64_TYPE },
			{ 24, 8, 8, B_POINTER_TYPE },
		}
	},
	{
		"_kern_open_attr_dir", 3,
		{ 4, 8, B_INT32_TYPE },
		{
			{ 0, 4, 8, B_INT32_TYPE },
			{ 8, 8, 8, B_STRING_TYPE },
			{ 16, 1, 8, B_INT8_TYPE },
		}
	},
	{
		"_kern_read_attr", 5,
		{ 8, 8, B_INT64_TYPE },
		{
			{ 0, 4, 8, B_INT32_TYPE },
			{ 8, 8, 8, B_STRING_TYPE },
			{ 16, 8, 8, B_INT64_TYPE },
			{ 24, 8, 8, B_POINTER_TYPE },
			{ 32, 8, 8, B_INT64_TYPE },
		}
	},
	{
		"_kern_write_attr", 6,
		{ 8, 8, B_INT64_TYPE },
		{
			{ 0, 4, 8, B_INT32_TYPE },
			{ 8, 8, 8, B_STRING_TYPE },
			{ 16, 4, 8, B_INT32_TYPE },
			{ 24, 8, 8, B_INT64_TYPE },
			{ 32, 8, 8, B_POINTER_TYPE },
			{ 40, 8, 8, B_INT64_TYPE },
		}
	},
	{
		"_kern_stat_attr", 3,
		{ 4, 8, B_INT32_TYPE },
		{
			{ 0, 4, 8, B_INT32_TYPE },
			{ 8, 8, 8, B_STRING_TYPE },
			{ 16, 8, 8, B_POINTER_TYPE },
		}
	},
	{
		"_kern_open_attr", 5,
		{ 4, 8, B_INT32_TYPE },
		{
			{ 0, 4, 8, B_INT32_TYPE },
			{ 8, 8, 8, B_STRING_TYPE },
			{ 16, 8, 8, B_STRING_TYPE },
			{ 24, 4, 8, B_INT32_TYPE },
			{ 32, 4, 8, B_INT32_TYPE },
		}
	},
	{
		"_kern_remove_attr", 2,
		{ 4, 8, B_INT32_TYPE },
		{
			{ 0, 4, 8, B_INT32_TYPE },
			{ 8, 8, 8, B_STRING_TYPE },
		}
	},
	{
		"_kern_rename_attr", 4,
		{ 4, 8, B_INT32_TYPE },
		{
			{ 0, 4, 8, B_INT32_TYPE },
			{ 8, 8, 8, B_STRING_TYPE },
			{ 16, 4, 8, B_INT32_TYPE },
			{ 24, 8, 8, B_STRING_TYPE },
		}
	},
	{
		"_kern_open_index_dir", 1,
		{ 4, 8, B_INT32_TYPE },
		{
			{ 0, 4, 8, B_INT32_TYPE },
		}
	},
	{
		"_kern_create_index", 4,
		{ 4, 8, B_INT32_TYPE },
		{
			{ 0, 4, 8, B_INT32_TYPE },
			{ 8, 8, 8, B_STRING_TYPE },
			{ 16, 4, 8, B_INT32_TYPE },
			{ 24, 4, 8, B_INT32_TYPE },
		}
	},
	{
		"_kern_read_index_stat", 3,
		{ 4, 8, B_INT32_TYPE },
		{
			{ 0, 4, 8, B_INT32_TYPE },
			{ 8, 8, 8, B_STRING_TYPE },
			{ 16, 8, 8, B_POINTER_TYPE },
		}
	},
	{
		"_kern_remove_index", 2,
		{ 4, 8, B_INT32_TYPE },
		{
			{ 0, 4, 8, B_INT32_TYPE },
			{ 8, 8, 8, B_STRING_TYPE },
		}
	},
	{
		"_kern_getcwd", 2,
		{ 4, 8, B_INT32_TYPE },
		{
			{ 0, 8, 8, B_POINTER_TYPE },
			{ 8, 8, 8, B_INT64_TYPE },
		}
	},
	{
		"_kern_setcwd", 2,
		{ 4, 8, B_INT32_TYPE },
		{
			{ 0, 4, 8, B_INT32_TYPE },
			{ 8, 8, 8, B_STRING_TYPE },
		}
	},
	{
		"_kern_open_query", 6,
		{ 4, 8, B_INT32_TYPE },
		{
			{ 0, 4, 8, B_INT32_TYPE },
			{ 8, 8, 8, B_STRING_TYPE },
			{ 16, 8, 8, B_INT64_TYPE },
			{ 24, 4, 8, B_INT32_TYPE },
			{ 32, 4, 8, B_INT32_TYPE },
			{ 40, 4, 8, B_INT32_TYPE },
		}
	},
	{
		"_kern_read", 4,
		{ 8, 8, B_INT64_TYPE },
		{
			{ 0, 4, 8, B_INT32_TYPE },
			{ 8, 8, 8, B_INT64_TYPE },
			{ 16, 8, 8, B_POINTER_TYPE },
			{ 24, 8, 8, B_INT64_TYPE },
		}
	},
	{
		"_kern_readv", 4,
		{ 8, 8, B_INT64_TYPE },
		{
			{ 0, 4, 8, B_INT32_TYPE },
			{ 8, 8, 8, B_INT64_TYPE },
			{ 16, 8, 8, B_POINTER_TYPE },
			{ 24, 8, 8, B_INT64_TYPE },
		}
	},
	{
		"_kern_write", 4,
		{ 8, 8, B_INT64_TYPE },
		{
			{ 0, 4, 8, B_INT32_TYPE },
			{ 8, 8, 8, B_INT64_TYPE },
			{ 16, 8, 8, B_POINTER_TYPE },
			{ 24, 8, 8, B_INT64_TYPE },
		}
	},
	{
		"_kern_writev", 4,
		{ 8, 8, B_INT64_TYPE },
		{
			{ 0, 4, 8, B_INT32_TYPE },
			{ 8, 8, 8, B_INT64_TYPE },
			{ 16, 8, 8, B_POINTER_TYPE },
			{ 24, 8, 8, B_INT64_TYPE },
		}
	},
	{
		"_kern_ioctl", 4,
		{ 4, 8, B_INT32_TYPE },
		{
			{ 0, 4, 8, B_INT32_TYPE },
			{ 8, 4, 8, B_INT32_TYPE },
			{ 16, 8, 8, B_POINTER_TYPE },
			{ 24, 8, 8, B_INT64_TYPE },
		}
	},
	{
		"_kern_read_dir", 4,
		{ 8, 8, B_INT64_TYPE },
		{
			{ 0, 4, 8, B_INT32_TYPE },
			{ 8, 8, 8, B_POINTER_TYPE },
			{ 16, 8, 8, B_INT64_TYPE },
			{ 24, 4, 8, B_INT32_TYPE },
		}
	},
	{
		"_kern_rewind_dir", 1,
		{ 4, 8, B_INT32_TYPE },
		{
			{ 0, 4, 8, B_INT32_TYPE },
		}
	},
	{
		"_kern_read_stat", 5,
		{ 4, 8, B_INT32_TYPE },
		{
			{ 0, 4, 8, B_INT32_TYPE },
			{ 8, 8, 8, B_STRING_TYPE },
			{ 16, 1, 8, B_INT8_TYPE },
			{ 24, 8, 8, B_POINTER_TYPE },
			{ 32, 8, 8, B_INT64_TYPE },
		}
	},
	{
		"_kern_write_stat", 6,
		{ 4, 8, B_INT32_TYPE },
		{
			{ 0, 4, 8, B_INT32_TYPE },
			{ 8, 8, 8, B_STRING_TYPE },
			{ 16, 1, 8, B_INT8_TYPE },
			{ 24, 8, 8, B_POINTER_TYPE },
			{ 32, 8, 8, B_INT64_TYPE },
			{ 40, 4, 8, B_INT32_TYPE },
		}
	},
	{
		"_kern_close", 1,
		{ 4, 8, B_INT32_TYPE },
		{
			{ 0, 4, 8, B_INT32_TYPE },
		}
	},
	{
		"_kern_dup", 1,
		{ 4, 8, B_INT32_TYPE },
		{
			{ 0, 4, 8, B_INT32_TYPE },
		}
	},
	{
		"_kern_dup2", 2,
		{ 4, 8, B_INT32_TYPE },
		{
			{ 0, 4, 8, B_INT32_TYPE },
			{ 8, 4, 8, B_INT32_TYPE },
		}
	},
	{
		"_kern_lock_node", 1,
		{ 4, 8, B_INT32_TYPE },
		{
			{ 0, 4, 8, B_INT32_TYPE },
		}
	},
	{
		"_kern_unlock_node", 1,
		{ 4, 8, B_INT32_TYPE },
		{
			{ 0, 4, 8, B_INT32_TYPE },
		}
	},
	{
		"_kern_get_next_fd_info", 4,
		{ 4, 8, B_INT32_TYPE },
		{
			{ 0, 4, 8, B_INT32_TYPE },
			{ 8, 8, 8, B_POINTER_TYPE },
			{ 16, 8, 8, B_POINTER_TYPE },
			{ 24, 8, 8, B_INT64_TYPE },
		}
	},
	{
		"_kern_preallocate", 3,
		{ 4, 8, B_INT32_TYPE },
		{
			{ 0, 4, 8, B_INT32_TYPE },
			{ 8, 8, 8, B_INT64_TYPE },
			{ 16, 8, 8, B_INT64_TYPE },
		}
	},
	{
		"_kern_socket", 3,
		{ 4, 8, B_INT32_TYPE },
		{
			{ 0, 4, 8, B_INT32_TYPE },
			{ 8, 4, 8, B_INT32_TYPE },
			{ 16, 4, 8, B_INT32_TYPE },
		}
	},
	{
		"_kern_bind", 3,
		{ 4, 8, B_INT32_TYPE },
		{
			{ 0, 4, 8, B_INT32_TYPE },
			{ 8, 8, 8, B_POINTER_TYPE },
			{ 16, 4, 8, B_INT32_TYPE },
		}
	},
	{
		"_kern_shutdown_socket", 2,
		{ 4, 8, B_INT32_TYPE },
		{
			{ 0, 4, 8, B_INT32_TYPE },
			{ 8, 4, 8, B_INT32_TYPE },
		}
	},
	{
		"_kern_connect", 3,
		{ 4, 8, B_INT32_TYPE },
		{
			{ 0, 4, 8, B_INT32_TYPE },
			{ 8, 8, 8, B_POINTER_TYPE },
			{ 16, 4, 8, B_INT32_TYPE },
		}
	},
	{
		"_kern_listen", 2,
		{ 4, 8, B_INT32_TYPE },
		{
			{ 0, 4, 8, B_INT32_TYPE },
			{ 8, 4, 8, B_INT32_TYPE },
		}
	},
	{
		"_kern_accept", 3,
		{ 4, 8, B_INT32_TYPE },
		{
			{ 0, 4, 8, B_INT32_TYPE },
			{ 8, 8, 8, B_POINTER_TYPE },
			{ 16, 8, 8, B_POINTER_TYPE },
		}
	},
	{
		"_kern_recv", 4,
		{ 8, 8, B_INT64_TYPE },
		{
			{ 0, 4, 8, B_INT32_TYPE },
			{ 8, 8, 8, B_POINTER_TYPE },
			{ 16, 8, 8, B_INT64_TYPE },
			{ 24, 4, 8, B_INT32_TYPE },
		}
	},
	{
		"_kern_recvfrom", 6,
		{ 8, 8, B_INT64_TYPE },
		{
			{ 0, 4, 8, B_INT32_TYPE },
			{ 8, 8, 8, B_POINTER_TYPE },
			{ 16, 8, 8, B_INT64_TYPE },
			{ 24, 4, 8, B_INT32_TYPE },
			{ 32, 8, 8, B_POINTER_TYPE },
			{ 40, 8, 8, B_POINTER_TYPE },
		}
	},
	{
		"_kern_recvmsg", 3,
		{ 8, 8, B_INT64_TYPE },
		{
			{ 0, 4, 8, B_INT32_TYPE },
			{ 8, 8, 8, B_POINTER_TYPE },
			{ 16, 4, 8, B_INT32_TYPE },
		}
	},
	{
		"_kern_send", 4,
		{ 8, 8, B_INT64_TYPE },
		{
			{ 0, 4, 8, B_INT32_TYPE },
			{ 8, 8, 8, B_POINTER_TYPE },
			{ 16, 8, 8, B_INT64_TYPE },
			{ 24, 4, 8, B_INT32_TYPE },
		}
	},
	{
		"_kern_sendto", 6,
		{ 8, 8, B_INT64_TYPE },
		{
			{ 0, 4, 8, B_INT32_TYPE },
			{ 8, 8, 8, B_POINTER_TYPE },
			{ 16, 8, 8, B_INT64_TYPE },
			{ 24, 4, 8, B_INT32_TYPE },
			{ 32, 8, 8, B_POINTER_TYPE },
			{ 40, 4, 8, B_INT32_TYPE },
		}
	},
	{
		"_kern_sendmsg", 3,
		{ 8, 8, B_INT64_TYPE },
		{
			{ 0, 4, 8, B_INT32_TYPE },
			{ 8, 8, 8, B_POINTER_TYPE },
			{ 16, 4, 8, B_INT32_TYPE },
		}
	},
	{
		"_kern_getsockopt", 5,
		{ 4, 8, B_INT32_TYPE },
		{
			{ 0, 4, 8, B_INT32_TYPE },
			{ 8, 4, 8, B_INT32_TYPE },
			{ 16, 4, 8, B_INT32_TYPE },
			{ 24, 8, 8, B_POINTER_TYPE },
			{ 32, 8, 8, B_POINTER_TYPE },
		}
	},
	{
		"_kern_setsockopt", 5,
		{ 4, 8, B_INT32_TYPE },
		{
			{ 0, 4, 8, B_INT32_TYPE },
			{ 8, 4, 8, B_INT32_TYPE },
			{ 16, 4, 8, B_INT32_TYPE },
			{ 24, 8, 8, B_POINTER_TYPE },
			{ 32, 4, 8, B_INT32_TYPE },
		}
	},
	{
		"_kern_getpeername", 3,
		{ 4, 8, B_INT32_TYPE },
		{
			{ 0, 4, 8, B_INT32_TYPE },
			{ 8, 8, 8, B_POINTER_TYPE },
			{ 16, 8, 8, B_POINTER_TYPE },
		}
	},
	{
		"_kern_getsockname", 3,
		{ 4, 8, B_INT32_TYPE },
		{
			{ 0, 4, 8, B_INT32_TYPE },
			{ 8, 8, 8, B_POINTER_TYPE },
			{ 16, 8, 8, B_POINTER_TYPE },
		}
	},
	{
		"_kern_sockatmark", 1,
		{ 4, 8, B_INT32_TYPE },
		{
			{ 0, 4, 8, B_INT32_TYPE },
		}
	},
	{
		"_kern_socketpair", 4,
		{ 4, 8, B_INT32_TYPE },
		{
			{ 0, 4, 8, B_INT32_TYPE },
			{ 8, 4, 8, B_INT32_TYPE },
			{ 16, 4, 8, B_INT32_TYPE },
			{ 24, 8, 8, B_POINTER_TYPE },
		}
	},
	{
		"_kern_get_next_socket_stat", 3,
		{ 4, 8, B_INT32_TYPE },
		{
			{ 0, 4, 8, B_INT32_TYPE },
			{ 8, 8, 8, B_POINTER_TYPE },
			{ 16, 8, 8, B_POINTER_TYPE },
		}
	},
	{
		"_kern_stop_notifying", 2,
		{ 4, 8, B_INT32_TYPE },
		{
			{ 0, 4, 8, B_INT32_TYPE },
			{ 8, 4, 8, B_INT32_TYPE },
		}
	},
	{
		"_kern_start_watching", 5,
		{ 4, 8, B_INT32_TYPE },
		{
			{ 0, 4, 8, B_INT32_TYPE },
			{ 8, 8, 8, B_INT64_TYPE },
			{ 16, 4, 8, B_INT32_TYPE },
			{ 24, 4, 8, B_INT32_TYPE },
			{ 32, 4, 8, B_INT32_TYPE },
		}
	},
	{
		"_kern_stop_watching", 4,
		{ 4, 8, B_INT32_TYPE },
		{
			{ 0, 4, 8, B_INT32_TYPE },
			{ 8, 8, 8, B_INT64_TYPE },
			{ 16, 4, 8, B_INT32_TYPE },
			{ 24, 4, 8, B_INT32_TYPE },
		}
	},
	{
		"_kern_set_real_time_clock", 1,
		{ 4, 8, B_INT32_TYPE },
		{
			{ 0, 8, 8, B_INT64_TYPE },
		}
	},
	{
		"_kern_set_timezone", 3,
		{ 4, 8, B_INT32_TYPE },
		{
			{ 0, 4, 8, B_INT32_TYPE },
			{ 8, 8, 8, B_STRING_TYPE },
			{ 16, 8, 8, B_INT64_TYPE },
		}
	},
	{
		"_kern_get_timezone", 3,
		{ 4, 8, B_INT32_TYPE },
		{
			{ 0, 8, 8, B_POINTER_TYPE },
			{ 8, 8, 8, B_POINTER_TYPE },
			{ 16, 8, 8, B_INT64_TYPE },
		}
	},
	{
		"_kern_set_real_time_clock_is_gmt", 1,
		{ 4, 8, B_INT32_TYPE },
		{
			{ 0, 1, 8, B_INT8_TYPE },
		}
	},
	{
		"_kern_get_real_time_clock_is_gmt", 1,
		{ 4, 8, B_INT32_TYPE },
		{
			{ 0, 8, 8, B_POINTER_TYPE },
		}
	},
	{
		"_kern_get_clock", 2,
		{ 4, 8, B_INT32_TYPE },
		{
			{ 0, 4, 8, B_INT32_TYPE },
			{ 8, 8, 8, B_POINTER_TYPE },
		}
	},
	{
		"_kern_set_clock", 2,
		{ 4, 8, B_INT32_TYPE },
		{
			{ 0, 4, 8, B_INT32_TYPE },
			{ 8, 8, 8, B_INT64_TYPE },
		}
	},
	{
		"_kern_system_time", 0,
		{ 8, 8, B_INT64_TYPE },
		{
		}
	},
	{
		"_kern_snooze_etc", 4,
		{ 4, 8, B_INT32_TYPE },
		{
			{ 0, 8, 8, B_INT64_TYPE },
			{ 8, 4, 8, B_INT32_TYPE },
			{ 16, 4, 8, B_INT32_TYPE },
			{ 24, 8, 8, B_POINTER_TYPE },
		}
	},
	{
		"_kern_create_timer", 5,
		{ 4, 8, B_INT32_TYPE },
		{
			{ 0, 4, 8, B_INT32_TYPE },
			{ 8, 4, 8, B_INT32_TYPE },
			{ 16, 4, 8, B_INT32_TYPE },
			{ 24, 8, 8, B_POINTER_TYPE },
			{ 32, 8, 8, B_POINTER_TYPE },
		}
	},
	{
		"_kern_delete_timer", 2,
		{ 4, 8, B_INT32_TYPE },
		{
			{ 0, 4, 8, B_INT32_TYPE },
			{ 8, 4, 8, B_INT32_TYPE },
		}
	},
	{
		"_kern_get_timer", 3,
		{ 4, 8, B_INT32_TYPE },
		{
			{ 0, 4, 8, B_INT32_TYPE },
			{ 8, 4, 8, B_INT32_TYPE },
			{ 16, 8, 8, B_POINTER_TYPE },
		}
	},
	{
		"_kern_set_timer", 6,
		{ 4, 8, B_INT32_TYPE },
		{
			{ 0, 4, 8, B_INT32_TYPE },
			{ 8, 4, 8, B_INT32_TYPE },
			{ 16, 8, 8, B_INT64_TYPE },
			{ 24, 8, 8, B_INT64_TYPE },
			{ 32, 4, 8, B_INT32_TYPE },
			{ 40, 8, 8, B_POINTER_TYPE },
		}
	},
	{
		"_kern_create_area", 6,
		{ 4, 8, B_INT32_TYPE },
		{
			{ 0, 8, 8, B_STRING_TYPE },
			{ 8, 8, 8, B_POINTER_TYPE },
			{ 16, 4, 8, B_INT32_TYPE },
			{ 24, 8, 8, B_INT64_TYPE },
			{ 32, 4, 8, B_INT32_TYPE },
			{ 40, 4, 8, B_INT32_TYPE },
		}
	},
	{
		"_kern_delete_area", 1,
		{ 4, 8, B_INT32_TYPE },
		{
			{ 0, 4, 8, B_INT32_TYPE },
		}
	},
	{
		"_kern_area_for", 1,
		{ 4, 8, B_INT32_TYPE },
		{
			{ 0, 8, 8, B_POINTER_TYPE },
		}
	},
	{
		"_kern_find_area", 1,
		{ 4, 8, B_INT32_TYPE },
		{
			{ 0, 8, 8, B_STRING_TYPE },
		}
	},
	{
		"_kern_get_area_info", 2,
		{ 4, 8, B_INT32_TYPE },
		{
			{ 0, 4, 8, B_INT32_TYPE },
			{ 8, 8, 8, B_POINTER_TYPE },
		}
	},
	{
		"_kern_get_next_area_info", 3,
		{ 4, 8, B_INT32_TYPE },
		{
			{ 0, 4, 8, B_INT32_TYPE },
			{ 8, 8, 8, B_POINTER_TYPE },
			{ 16, 8, 8, B_POINTER_TYPE },
		}
	},
	{
		"_kern_resize_area", 2,
		{ 4, 8, B_INT32_TYPE },
		{
			{ 0, 4, 8, B_INT32_TYPE },
			{ 8, 8, 8, B_INT64_TYPE },
		}
	},
	{
		"_kern_transfer_area", 4,
		{ 4, 8, B_INT32_TYPE },
		{
			{ 0, 4, 8, B_INT32_TYPE },
			{ 8, 8, 8, B_POINTER_TYPE },
			{ 16, 4, 8, B_INT32_TYPE },
			{ 24, 4, 8, B_INT32_TYPE },
		}
	},
	{
		"_kern_set_area_protection", 2,
		{ 4, 8, B_INT32_TYPE },
		{
			{ 0, 4, 8, B_INT32_TYPE },
			{ 8, 4, 8, B_INT32_TYPE },
		}
	},
	{
		"_kern_clone_area", 5,
		{ 4, 8, B_INT32_TYPE },
		{
			{ 0, 8, 8, B_STRING_TYPE },
			{ 8, 8, 8, B_POINTER_TYPE },
			{ 16, 4, 8, B_INT32_TYPE },
			{ 24, 4, 8, B_INT32_TYPE },
			{ 32, 4, 8, B_INT32_TYPE },
		}
	},
	{
		"_kern_reserve_address_range", 3,
		{ 4, 8, B_INT32_TYPE },
		{
			{ 0, 8, 8, B_POINTER_TYPE },
			{ 8, 4, 8, B_INT32_TYPE },
			{ 16, 8, 8, B_INT64_TYPE },
		}
	},
	{
		"_kern_unreserve_address_range", 2,
		{ 4, 8, B_INT32_TYPE },
		{
			{ 0, 8, 8, B_INT64_TYPE },
			{ 8, 8, 8, B_INT64_TYPE },
		}
	},
	{
		"_kern_map_file", 9,
		{ 4, 8, B_INT32_TYPE },
		{
			{ 0, 8, 8, B_STRING_TYPE },
			{ 8, 8, 8, B_POINTER_TYPE },
			{ 16, 4, 8, B_INT32_TYPE },
			{ 24, 8, 8, B_INT64_TYPE },
			{ 32, 4, 8, B_INT32_TYPE },
			{ 40, 4, 8, B_INT32_TYPE },
			{ 48, 1, 8, B_INT8_TYPE },
			{ 56, 4, 8, B_INT32_TYPE },
			{ 64, 8, 8, B_INT64_TYPE },
		}
	},
	{
		"_kern_unmap_memory", 2,
		{ 4, 8, B_INT32_TYPE },
		{
			{ 0, 8, 8, B_POINTER_TYPE },
			{ 8, 8, 8, B_INT64_TYPE },
		}
	},
	{
		"_kern_set_memory_protection", 3,
		{ 4, 8, B_INT32_TYPE },
		{
			{ 0, 8, 8, B_POINTER_TYPE },
			{ 8, 8, 8, B_INT64_TYPE },
			{ 16, 4, 8, B_INT32_TYPE },
		}
	},
	{
		"_kern_sync_memory", 3,
		{ 4, 8, B_INT32_TYPE },
		{
			{ 0, 8, 8, B_POINTER_TYPE },
			{ 8, 8, 8, B_INT64_TYPE },
			{ 16, 4, 8, B_INT32_TYPE },
		}
	},
	{
		"_kern_memory_advice", 3,
		{ 4, 8, B_INT32_TYPE },
		{
			{ 0, 8, 8, B_POINTER_TYPE },
			{ 8, 8, 8, B_INT64_TYPE },
			{ 16, 4, 8, B_INT32_TYPE },
		}
	},
	{
		"_kern_get_memory_properties", 4,
		{ 4, 8, B_INT32_TYPE },
		{
			{ 0, 4, 8, B_INT32_TYPE },
			{ 8, 8, 8, B_POINTER_TYPE },
			{ 16, 8, 8, B_POINTER_TYPE },
			{ 24, 8, 8, B_POINTER_TYPE },
		}
	},
	{
		"_kern_mlock", 2,
		{ 4, 8, B_INT32_TYPE },
		{
			{ 0, 8, 8, B_POINTER_TYPE },
			{ 8, 8, 8, B_INT64_TYPE },
		}
	},
	{
		"_kern_munlock", 2,
		{ 4, 8, B_INT32_TYPE },
		{
			{ 0, 8, 8, B_POINTER_TYPE },
			{ 8, 8, 8, B_INT64_TYPE },
		}
	},
	{
		"_kern_create_port", 2,
		{ 4, 8, B_INT32_TYPE },
		{
			{ 0, 4, 8, B_INT32_TYPE },
			{ 8, 8, 8, B_STRING_TYPE },
		}
	},
	{
		"_kern_close_port", 1,
		{ 4, 8, B_INT32_TYPE },
		{
			{ 0, 4, 8, B_INT32_TYPE },
		}
	},
	{
		"_kern_delete_port", 1,
		{ 4, 8, B_INT32_TYPE },
		{
			{ 0, 4, 8, B_INT32_TYPE },
		}
	},
	{
		"_kern_find_port", 1,
		{ 4, 8, B_INT32_TYPE },
		{
			{ 0, 8, 8, B_STRING_TYPE },
		}
	},
	{
		"_kern_get_port_info", 2,
		{ 4, 8, B_INT32_TYPE },
		{
			{ 0, 4, 8, B_INT32_TYPE },
			{ 8, 8, 8, B_POINTER_TYPE },
		}
	},
	{
		"_kern_get_next_port_info", 3,
		{ 4, 8, B_INT32_TYPE },
		{
			{ 0, 4, 8, B_INT32_TYPE },
			{ 8, 8, 8, B_POINTER_TYPE },
			{ 16, 8, 8, B_POINTER_TYPE },
		}
	},
	{
		"_kern_port_buffer_size_etc", 3,
		{ 8, 8, B_INT64_TYPE },
		{
			{ 0, 4, 8, B_INT32_TYPE },
			{ 8, 4, 8, B_INT32_TYPE },
			{ 16, 8, 8, B_INT64_TYPE },
		}
	},
	{
		"_kern_port_count", 1,
		{ 4, 8, B_INT32_TYPE },
		{
			{ 0, 4, 8, B_INT32_TYPE },
		}
	},
	{
		"_kern_read_port_etc", 6,
		{ 8, 8, B_INT64_TYPE },
		{
			{ 0, 4, 8, B_INT32_TYPE },
			{ 8, 8, 8, B_POINTER_TYPE },
			{ 16, 8, 8, B_POINTER_TYPE },
			{ 24, 8, 8, B_INT64_TYPE },
			{ 32, 4, 8, B_INT32_TYPE },
			{ 40, 8, 8, B_INT64_TYPE },
		}
	},
	{
		"_kern_set_port_owner", 2,
		{ 4, 8, B_INT32_TYPE },
		{
			{ 0, 4, 8, B_INT32_TYPE },
			{ 8, 4, 8, B_INT32_TYPE },
		}
	},
	{
		"_kern_write_port_etc", 6,
		{ 4, 8, B_INT32_TYPE },
		{
			{ 0, 4, 8, B_INT32_TYPE },
			{ 8, 4, 8, B_INT32_TYPE },
			{ 16, 8, 8, B_POINTER_TYPE },
			{ 24, 8, 8, B_INT64_TYPE },
			{ 32, 4, 8, B_INT32_TYPE },
			{ 40, 8, 8, B_INT64_TYPE },
		}
	},
	{
		"_kern_writev_port_etc", 7,
		{ 4, 8, B_INT32_TYPE },
		{
			{ 0, 4, 8, B_INT32_TYPE },
			{ 8, 4, 8, B_INT32_TYPE },
			{ 16, 8, 8, B_POINTER_TYPE },
			{ 24, 8, 8, B_INT64_TYPE },
			{ 32, 8, 8, B_INT64_TYPE },
			{ 40, 4, 8, B_INT32_TYPE },
			{ 48, 8, 8, B_INT64_TYPE },
		}
	},
	{
		"_kern_get_port_message_info_etc", 5,
		{ 4, 8, B_INT32_TYPE },
		{
			{ 0, 4, 8, B_INT32_TYPE },
			{ 8, 8, 8, B_POINTER_TYPE },
			{ 16, 8, 8, B_INT64_TYPE },
			{ 24, 4, 8, B_INT32_TYPE },
			{ 32, 8, 8, B_INT64_TYPE },
		}
	},
	{
		"_kern_kernel_debugger", 1,
		{ 4, 8, B_INT32_TYPE },
		{
			{ 0, 8, 8, B_STRING_TYPE },
		}
	},
	{
		"_kern_register_syslog_daemon", 1,
		{ 0, 0, B_RAW_TYPE },
		{
			{ 0, 4, 8, B_INT32_TYPE },
		}
	},
	{
		"_kern_debugger", 1,
		{ 0, 0, B_RAW_TYPE },
		{
			{ 0, 8, 8, B_STRING_TYPE },
		}
	},
	{
		"_kern_disable_debugger", 1,
		{ 4, 8, B_INT32_TYPE },
		{
			{ 0, 4, 8, B_INT32_TYPE },
		}
	},
	{
		"_kern_install_default_debugger", 1,
		{ 4, 8, B_INT32_TYPE },
		{
			{ 0, 4, 8, B_INT32_TYPE },
		}
	},
	{
		"_kern_install_team_debugger", 2,
		{ 4, 8, B_INT32_TYPE },
		{
			{ 0, 4, 8, B_INT32_TYPE },
			{ 8, 4, 8, B_INT32_TYPE },
		}
	},
	{
		"_kern_remove_team_debugger", 1,
		{ 4, 8, B_INT32_TYPE },
		{
			{ 0, 4, 8, B_INT32_TYPE },
		}
	},
	{
		"_kern_debug_thread", 1,
		{ 4, 8, B_INT32_TYPE },
		{
			{ 0, 4, 8, B_INT32_TYPE },
		}
	},
	{
		"_kern_wait_for_debugger", 0,
		{ 0, 0, B_RAW_TYPE },
		{
		}
	},
	{
		"_kern_set_debugger_breakpoint", 4,
		{ 4, 8, B_INT32_TYPE },
		{
			{ 0, 8, 8, B_POINTER_TYPE },
			{ 8, 4, 8, B_INT32_TYPE },
			{ 16, 4, 8, B_INT32_TYPE },
			{ 24, 1, 8, B_INT8_TYPE },
		}
	},
	{
		"_kern_clear_debugger_breakpoint", 2,
		{ 4, 8, B_INT32_TYPE },
		{
			{ 0, 8, 8, B_POINTER_TYPE },
			{ 8, 1, 8, B_INT8_TYPE },
		}
	},
	{
		"_kern_system_profiler_start", 1,
		{ 4, 8, B_INT32_TYPE },
		{
			{ 0, 8, 8, B_POINTER_TYPE },
		}
	},
	{
		"_kern_system_profiler_next_buffer", 2,
		{ 4, 8, B_INT32_TYPE },
		{
			{ 0, 8, 8, B_INT64_TYPE },
			{ 8, 8, 8, B_POINTER_TYPE },
		}
	},
	{
		"_kern_system_profiler_stop", 0,
		{ 4, 8, B_INT32_TYPE },
		{
		}
	},
	{
		"_kern_system_profiler_recorded", 1,
		{ 4, 8, B_INT32_TYPE },
		{
			{ 0, 8, 8, B_POINTER_TYPE },
		}
	},
	{
		"_kern_get_system_info", 1,
		{ 4, 8, B_INT32_TYPE },
		{
			{ 0, 8, 8, B_POINTER_TYPE },
		}
	},
	{
		"_kern_get_cpu_info", 3,
		{ 4, 8, B_INT32_TYPE },
		{
			{ 0, 4, 8, B_INT32_TYPE },
			{ 8, 4, 8, B_INT32_TYPE },
			{ 16, 8, 8, B_POINTER_TYPE },
		}
	},
	{
		"_kern_get_cpu_topology_info", 2,
		{ 4, 8, B_INT32_TYPE },
		{
			{ 0, 8, 8, B_POINTER_TYPE },
			{ 8, 8, 8, B_POINTER_TYPE },
		}
	},
	{
		"_kern_analyze_scheduling", 5,
		{ 4, 8, B_INT32_TYPE },
		{
			{ 0, 8, 8, B_INT64_TYPE },
			{ 8, 8, 8, B_INT64_TYPE },
			{ 16, 8, 8, B_POINTER_TYPE },
			{ 24, 8, 8, B_INT64_TYPE },
			{ 32, 8, 8, B_POINTER_TYPE },
		}
	},
	{
		"_kern_debug_output", 1,
		{ 0, 0, B_RAW_TYPE },
		{
			{ 0, 8, 8, B_STRING_TYPE },
		}
	},
	{
		"_kern_ktrace_output", 1,
		{ 0, 0, B_RAW_TYPE },
		{
			{ 0, 8, 8, B_STRING_TYPE },
		}
	},
	{
		"_kern_frame_buffer_update", 5,
		{ 4, 8, B_INT32_TYPE },
		{
			{ 0, 8, 8, B_INT64_TYPE },
			{ 8, 4, 8, B_INT32_TYPE },
			{ 16, 4, 8, B_INT32_TYPE },
			{ 24, 4, 8, B_INT32_TYPE },
			{ 32, 4, 8, B_INT32_TYPE },
		}
	},
	{
		"_kern_register_messaging_service", 2,
		{ 4, 8, B_INT32_TYPE },
		{
			{ 0, 4, 8, B_INT32_TYPE },
			{ 8, 4, 8, B_INT32_TYPE },
		}
	},
	{
		"_kern_unregister_messaging_service", 0,
		{ 4, 8, B_INT32_TYPE },
		{
		}
	},
	{
		"_kern_clear_caches", 3,
		{ 0, 0, B_RAW_TYPE },
		{
			{ 0, 8, 8, B_POINTER_TYPE },
			{ 8, 8, 8, B_INT64_TYPE },
			{ 16, 4, 8, B_INT32_TYPE },
		}
	},
	{
		"_kern_cpu_enabled", 1,
		{ 1, 8, B_INT8_TYPE },
		{
			{ 0, 4, 8, B_INT32_TYPE },
		}
	},
	{
		"_kern_set_cpu_enabled", 2,
		{ 4, 8, B_INT32_TYPE },
		{
			{ 0, 4, 8, B_INT32_TYPE },
			{ 8, 1, 8, B_INT8_TYPE },
		}
	},
	{
		"_kern_get_next_disk_device_id", 2,
		{ 4, 8, B_INT32_TYPE },
		{
			{ 0, 8, 8, B_POINTER_TYPE },
			{ 8, 8, 8, B_POINTER_TYPE },
		}
	},
	{
		"_kern_find_disk_device", 2,
		{ 4, 8, B_INT32_TYPE },
		{
			{ 0, 8, 8, B_STRING_TYPE },
			{ 8, 8, 8, B_POINTER_TYPE },
		}
	},
	{
		"_kern_find_partition", 2,
		{ 4, 8, B_INT32_TYPE },
		{
			{ 0, 8, 8, B_STRING_TYPE },
			{ 8, 8, 8, B_POINTER_TYPE },
		}
	},
	{
		"_kern_find_file_disk_device", 2,
		{ 4, 8, B_INT32_TYPE },
		{
			{ 0, 8, 8, B_STRING_TYPE },
			{ 8, 8, 8, B_POINTER_TYPE },
		}
	},
	{
		"_kern_get_disk_device_data", 5,
		{ 4, 8, B_INT32_TYPE },
		{
			{ 0, 4, 8, B_INT32_TYPE },
			{ 8, 1, 8, B_INT8_TYPE },
			{ 16, 8, 8, B_POINTER_TYPE },
			{ 24, 8, 8, B_INT64_TYPE },
			{ 32, 8, 8, B_POINTER_TYPE },
		}
	},
	{
		"_kern_register_file_device", 1,
		{ 4, 8, B_INT32_TYPE },
		{
			{ 0, 8, 8, B_STRING_TYPE },
		}
	},
	{
		"_kern_unregister_file_device", 2,
		{ 4, 8, B_INT32_TYPE },
		{
			{ 0, 4, 8, B_INT32_TYPE },
			{ 8, 8, 8, B_STRING_TYPE },
		}
	},
	{
		"_kern_get_file_disk_device_path", 3,
		{ 4, 8, B_INT32_TYPE },
		{
			{ 0, 4, 8, B_INT32_TYPE },
			{ 8, 8, 8, B_POINTER_TYPE },
			{ 16, 8, 8, B_INT64_TYPE },
		}
	},
	{
		"_kern_get_disk_system_info", 2,
		{ 4, 8, B_INT32_TYPE },
		{
			{ 0, 4, 8, B_INT32_TYPE },
			{ 8, 8, 8, B_POINTER_TYPE },
		}
	},
	{
		"_kern_get_next_disk_system_info", 2,
		{ 4, 8, B_INT32_TYPE },
		{
			{ 0, 8, 8, B_POINTER_TYPE },
			{ 8, 8, 8, B_POINTER_TYPE },
		}
	},
	{
		"_kern_find_disk_system", 2,
		{ 4, 8, B_INT32_TYPE },
		{
			{ 0, 8, 8, B_STRING_TYPE },
			{ 8, 8, 8, B_POINTER_TYPE },
		}
	},
	{
		"_kern_defragment_partition", 2,
		{ 4, 8, B_INT32_TYPE },
		{
			{ 0, 4, 8, B_INT32_TYPE },
			{ 8, 8, 8, B_POINTER_TYPE },
		}
	},
	{
		"_kern_repair_partition", 3,
		{ 4, 8, B_INT32_TYPE },
		{
			{ 0, 4, 8, B_INT32_TYPE },
			{ 8, 8, 8, B_POINTER_TYPE },
			{ 16, 1, 8, B_INT8_TYPE },
		}
	},
	{
		"_kern_resize_partition", 6,
		{ 4, 8, B_INT32_TYPE },
		{
			{ 0, 4, 8, B_INT32_TYPE },
			{ 8, 8, 8, B_POINTER_TYPE },
			{ 16, 4, 8, B_INT32_TYPE },
			{ 24, 8, 8, B_POINTER_TYPE },
			{ 32, 8, 8, B_INT64_TYPE },
			{ 40, 8, 8, B_INT64_TYPE },
		}
	},
	{
		"_kern_move_partition", 8,
		{ 4, 8, B_INT32_TYPE },
		{
			{ 0, 4, 8, B_INT32_TYPE },
			{ 8, 8, 8, B_POINTER_TYPE },
			{ 16, 4, 8, B_INT32_TYPE },
			{ 24, 8, 8, B_POINTER_TYPE },
			{ 32, 8, 8, B_INT64_TYPE },
			{ 40, 8, 8, B_POINTER_TYPE },
			{ 48, 8, 8, B_POINTER_TYPE },
			{ 56, 4, 8, B_INT32_TYPE },
		}
	},
	{
		"_kern_set_partition_name", 5,
		{ 4, 8, B_INT32_TYPE },
		{
			{ 0, 4, 8, B_INT32_TYPE },
			{ 8, 8, 8, B_POINTER_TYPE },
			{ 16, 4, 8, B_INT32_TYPE },
			{ 24, 8, 8, B_POINTER_TYPE },
			{ 32, 8, 8, B_STRING_TYPE },
		}
	},
	{
		"_kern_set_partition_content_name", 3,
		{ 4, 8, B_INT32_TYPE },
		{
			{ 0, 4, 8, B_INT32_TYPE },
			{ 8, 8, 8, B_POINTER_TYPE },
			{ 16, 8, 8, B_STRING_TYPE },
		}
	},
	{
		"_kern_set_partition_type", 5,
		{ 4, 8, B_INT32_TYPE },
		{
			{ 0, 4, 8, B_INT32_TYPE },
			{ 8, 8, 8, B_POINTER_TYPE },
			{ 16, 4, 8, B_INT32_TYPE },
			{ 24, 8, 8, B_POINTER_TYPE },
			{ 32, 8, 8, B_STRING_TYPE },
		}
	},
	{
		"_kern_set_partition_parameters", 5,
		{ 4, 8, B_INT32_TYPE },
		{
			{ 0, 4, 8, B_INT32_TYPE },
			{ 8, 8, 8, B_POINTER_TYPE },
			{ 16, 4, 8, B_INT32_TYPE },
			{ 24, 8, 8, B_POINTER_TYPE },
			{ 32, 8, 8, B_STRING_TYPE },
		}
	},
	{
		"_kern_set_partition_content_parameters", 3,
		{ 4, 8, B_INT32_TYPE },
		{
			{ 0, 4, 8, B_INT32_TYPE },
			{ 8, 8, 8, B_POINTER_TYPE },
			{ 16, 8, 8, B_STRING_TYPE },
		}
	},
	{
		"_kern_initialize_partition", 5,
		{ 4, 8, B_INT32_TYPE },
		{
			{ 0, 4, 8, B_INT32_TYPE },
			{ 8, 8, 8, B_POINTER_TYPE },
			{ 16, 8, 8, B_STRING_TYPE },
			{ 24, 8, 8, B_STRING_TYPE },
			{ 32, 8, 8, B_STRING_TYPE },
		}
	},
	{
		"_kern_uninitialize_partition", 4,
		{ 4, 8, B_INT32_TYPE },
		{
			{ 0, 4, 8, B_INT32_TYPE },
			{ 8, 8, 8, B_POINTER_TYPE },
			{ 16, 4, 8, B_INT32_TYPE },
			{ 24, 8, 8, B_POINTER_TYPE },
		}
	},
	{
		"_kern_create_child_partition", 9,
		{ 4, 8, B_INT32_TYPE },
		{
			{ 0, 4, 8, B_INT32_TYPE },
			{ 8, 8, 8, B_POINTER_TYPE },
			{ 16, 8, 8, B_INT64_TYPE },
			{ 24, 8, 8, B_INT64_TYPE },
			{ 32, 8, 8, B_STRING_TYPE },
			{ 40, 8, 8, B_STRING_TYPE },
			{ 48, 8, 8, B_STRING_TYPE },
			{ 56, 8, 8, B_POINTER_TYPE },
			{ 64, 8, 8, B_POINTER_TYPE },
		}
	},
	{
		"_kern_delete_child_partition", 4,
		{ 4, 8, B_INT32_TYPE },
		{
			{ 0, 4, 8, B_INT32_TYPE },
			{ 8, 8, 8, B_POINTER_TYPE },
			{ 16, 4, 8, B_INT32_TYPE },
			{ 24, 4, 8, B_INT32_TYPE },
		}
	},
	{
		"_kern_start_watching_disks", 3,
		{ 4, 8, B_INT32_TYPE },
		{
			{ 0, 4, 8, B_INT32_TYPE },
			{ 8, 4, 8, B_INT32_TYPE },
			{ 16, 4, 8, B_INT32_TYPE },
		}
	},
	{
		"_kern_stop_watching_disks", 2,
		{ 4, 8, B_INT32_TYPE },
		{
			{ 0, 4, 8, B_INT32_TYPE },
			{ 8, 4, 8, B_INT32_TYPE },
		}
	},
};
#endif	// _ASSEMBLER
