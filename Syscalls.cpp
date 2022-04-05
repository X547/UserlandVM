#include "Syscalls.h"
#include <private/system/syscalls.h>
#include <stdlib.h>

thread_id vm_spawn_thread(struct thread_creation_attributes* attributes);
void vm_exit_thread(status_t returnValue);


void DispatchSyscall(uint32 op, uint64 *args, uint64 *_returnValue)
{
	switch (op) {
		case 0:
			*_returnValue = _kern_is_computer_on();
			break;
		case 1:
			*_returnValue = _kern_generic_syscall(*(const char **)args, (uint32)*(long*)((char*)args + 8), *(void **)((char*)args + 16), *(size_t*)((char*)args + 24));
			break;
		case 2:
			*_returnValue = _kern_getrlimit((int)*(long*)args, *(struct rlimit **)((char*)args + 8));
			break;
		case 3:
			*_returnValue = _kern_setrlimit((int)*(long*)args, *(const struct rlimit **)((char*)args + 8));
			break;
		case 4:
			*_returnValue = _kern_shutdown((bool)*(long*)args);
			break;
		case 5:
			*_returnValue = _kern_get_safemode_option(*(const char **)args, *(char **)((char*)args + 8), *(size_t **)((char*)args + 16));
			break;
		case 6:
			*_returnValue = _kern_wait_for_objects(*(object_wait_info **)args, (int)*(long*)((char*)args + 8), (uint32)*(long*)((char*)args + 16), *(bigtime_t*)((char*)args + 24));
			break;
		case 7:
			*_returnValue = _kern_mutex_lock(*(int32 **)args, *(const char **)((char*)args + 8), (uint32)*(long*)((char*)args + 16), *(bigtime_t*)((char*)args + 24));
			break;
		case 8:
			*_returnValue = _kern_mutex_unlock(*(int32 **)args, (uint32)*(long*)((char*)args + 8));
			break;
		case 9:
			*_returnValue = _kern_mutex_switch_lock(*(int32 **)args, *(int32 **)((char*)args + 8), *(const char **)((char*)args + 16), (uint32)*(long*)((char*)args + 24), *(bigtime_t*)((char*)args + 32));
			break;
		case 10:
			*_returnValue = _kern_mutex_sem_acquire(*(int32 **)args, *(const char **)((char*)args + 8), (uint32)*(long*)((char*)args + 16), *(bigtime_t*)((char*)args + 24));
			break;
		case 11:
			*_returnValue = _kern_mutex_sem_release(*(int32 **)args);
			break;
		case 12:
			*_returnValue = _kern_create_sem((int)*(long*)args, *(const char **)((char*)args + 8));
			break;
		case 13:
			*_returnValue = _kern_delete_sem((sem_id)*(long*)args);
			break;
		case 14:
			*_returnValue = _kern_switch_sem((sem_id)*(long*)args, (sem_id)*(long*)((char*)args + 8));
			break;
		case 15:
			*_returnValue = _kern_switch_sem_etc((sem_id)*(long*)args, (sem_id)*(long*)((char*)args + 8), (uint32)*(long*)((char*)args + 16), (uint32)*(long*)((char*)args + 24), *(bigtime_t*)((char*)args + 32));
			break;
		case 16:
			*_returnValue = _kern_acquire_sem((sem_id)*(long*)args);
			break;
		case 17:
			*_returnValue = _kern_acquire_sem_etc((sem_id)*(long*)args, (uint32)*(long*)((char*)args + 8), (uint32)*(long*)((char*)args + 16), *(bigtime_t*)((char*)args + 24));
			break;
		case 18:
			*_returnValue = _kern_release_sem((sem_id)*(long*)args);
			break;
		case 19:
			*_returnValue = _kern_release_sem_etc((sem_id)*(long*)args, (uint32)*(long*)((char*)args + 8), (uint32)*(long*)((char*)args + 16));
			break;
		case 20:
			*_returnValue = _kern_get_sem_count((sem_id)*(long*)args, *(int32 **)((char*)args + 8));
			break;
		case 21:
			*_returnValue = _kern_get_sem_info((sem_id)*(long*)args, *(struct sem_info **)((char*)args + 8), *(size_t*)((char*)args + 16));
			break;
		case 22:
			*_returnValue = _kern_get_next_sem_info((team_id)*(long*)args, *(int32 **)((char*)args + 8), *(struct sem_info **)((char*)args + 16), *(size_t*)((char*)args + 24));
			break;
		case 23:
			*_returnValue = _kern_set_sem_owner((sem_id)*(long*)args, (team_id)*(long*)((char*)args + 8));
			break;
		case 24:
			*_returnValue = _kern_realtime_sem_open(*(const char **)args, (int)*(long*)((char*)args + 8), (mode_t)*(long*)((char*)args + 16), (uint32)*(long*)((char*)args + 24), *(struct _sem_t **)((char*)args + 32), *(struct _sem_t * **)((char*)args + 40));
			break;
		case 25:
			*_returnValue = _kern_realtime_sem_close((sem_id)*(long*)args, *(struct _sem_t * **)((char*)args + 8));
			break;
		case 26:
			*_returnValue = _kern_realtime_sem_unlink(*(const char **)args);
			break;
		case 27:
			*_returnValue = _kern_realtime_sem_get_value((sem_id)*(long*)args, *(int **)((char*)args + 8));
			break;
		case 28:
			*_returnValue = _kern_realtime_sem_post((sem_id)*(long*)args);
			break;
		case 29:
			*_returnValue = _kern_realtime_sem_wait((sem_id)*(long*)args, (uint32)*(long*)((char*)args + 8), *(bigtime_t*)((char*)args + 16));
			break;
		case 30:
			*_returnValue = _kern_xsi_semget((key_t)*(long*)args, (int)*(long*)((char*)args + 8), (int)*(long*)((char*)args + 16));
			break;
		case 31:
			*_returnValue = _kern_xsi_semctl((int)*(long*)args, (int)*(long*)((char*)args + 8), (int)*(long*)((char*)args + 16), *(union semun **)((char*)args + 24));
			break;
		case 32:
			*_returnValue = _kern_xsi_semop((int)*(long*)args, *(struct sembuf **)((char*)args + 8), *(size_t*)((char*)args + 16));
			break;
		case 33:
			*_returnValue = _kern_xsi_msgctl((int)*(long*)args, (int)*(long*)((char*)args + 8), *(struct msqid_ds **)((char*)args + 16));
			break;
		case 34:
			*_returnValue = _kern_xsi_msgget((key_t)*(long*)args, (int)*(long*)((char*)args + 8));
			break;
		case 35:
			*_returnValue = _kern_xsi_msgrcv((int)*(long*)args, *(void **)((char*)args + 8), *(size_t*)((char*)args + 16), *(long*)((char*)args + 24), (int)*(long*)((char*)args + 32));
			break;
		case 36:
			*_returnValue = _kern_xsi_msgsnd((int)*(long*)args, *(const void **)((char*)args + 8), *(size_t*)((char*)args + 16), (int)*(long*)((char*)args + 24));
			break;
		case 37:
			*_returnValue = _kern_load_image(*(const char * const **)args, *(size_t*)((char*)args + 8), (int32)*(long*)((char*)args + 16), (int32)*(long*)((char*)args + 24), (int32)*(long*)((char*)args + 32), (uint32)*(long*)((char*)args + 40), (port_id)*(long*)((char*)args + 48), (uint32)*(long*)((char*)args + 56));
			break;
		case 38:
			 _kern_exit_team((status_t)*(long*)args);
			break;
		case 39:
			*_returnValue = _kern_kill_team((team_id)*(long*)args);
			break;
		case 40:
			*_returnValue = _kern_get_current_team();
			break;
		case 41:
			*_returnValue = _kern_wait_for_team((team_id)*(long*)args, *(status_t **)((char*)args + 8));
			break;
		case 42:
			*_returnValue = _kern_wait_for_child((thread_id)*(long*)args, (uint32)*(long*)((char*)args + 8), *(siginfo_t **)((char*)args + 16), *(team_usage_info **)((char*)args + 24));
			break;
		case 43:
			*_returnValue = _kern_exec(*(const char **)args, *(const char * const **)((char*)args + 8), *(size_t*)((char*)args + 16), (int32)*(long*)((char*)args + 24), (int32)*(long*)((char*)args + 32), (mode_t)*(long*)((char*)args + 40));
			break;
		case 44:
			*_returnValue = _kern_fork();
			break;
		case 45:
			*_returnValue = _kern_process_info((pid_t)*(long*)args, (int32)*(long*)((char*)args + 8));
			break;
		case 46:
			*_returnValue = _kern_setpgid((pid_t)*(long*)args, (pid_t)*(long*)((char*)args + 8));
			break;
		case 47:
			*_returnValue = _kern_setsid();
			break;
		case 48:
			*_returnValue = _kern_change_root(*(const char **)args);
			break;
		case 49:
			*_returnValue = vm_spawn_thread(*(struct thread_creation_attributes **)args);
			break;
		case 50:
			*_returnValue = _kern_find_thread(*(const char **)args);
			break;
		case 51:
			*_returnValue = _kern_suspend_thread((thread_id)*(long*)args);
			break;
		case 52:
			*_returnValue = _kern_resume_thread((thread_id)*(long*)args);
			break;
		case 53:
			*_returnValue = _kern_rename_thread((thread_id)*(long*)args, *(const char **)((char*)args + 8));
			break;
		case 54:
			*_returnValue = _kern_set_thread_priority((thread_id)*(long*)args, (int32)*(long*)((char*)args + 8));
			break;
		case 55:
			*_returnValue = _kern_kill_thread((thread_id)*(long*)args);
			break;
		case 56:
			 vm_exit_thread((status_t)*(long*)args);
			break;
		case 57:
			*_returnValue = _kern_cancel_thread((thread_id)*(long*)args, *(void ( * *) ( int ))((char*)args + 8));
			break;
		case 58:
			 _kern_thread_yield();
			break;
		case 59:
			*_returnValue = _kern_wait_for_thread((thread_id)*(long*)args, *(status_t **)((char*)args + 8));
			break;
		case 60:
			*_returnValue = _kern_has_data((thread_id)*(long*)args);
			break;
		case 61:
			*_returnValue = _kern_send_data((thread_id)*(long*)args, (int32)*(long*)((char*)args + 8), *(const void **)((char*)args + 16), *(size_t*)((char*)args + 24));
			break;
		case 62:
			*_returnValue = _kern_receive_data(*(thread_id **)args, *(void **)((char*)args + 8), *(size_t*)((char*)args + 16));
			break;
		case 63:
			*_returnValue = _kern_restore_signal_frame(*(struct signal_frame_data **)args);
			break;
		case 64:
			*_returnValue = _kern_get_thread_info((thread_id)*(long*)args, *(thread_info **)((char*)args + 8));
			break;
		case 65:
			*_returnValue = _kern_get_next_thread_info((team_id)*(long*)args, *(int32 **)((char*)args + 8), *(thread_info **)((char*)args + 16));
			break;
		case 66:
			*_returnValue = _kern_get_team_info((team_id)*(long*)args, *(team_info **)((char*)args + 8));
			break;
		case 67:
			*_returnValue = _kern_get_next_team_info(*(int32 **)args, *(team_info **)((char*)args + 8));
			break;
		case 68:
			*_returnValue = _kern_get_team_usage_info((team_id)*(long*)args, (int32)*(long*)((char*)args + 8), *(team_usage_info **)((char*)args + 16), *(size_t*)((char*)args + 24));
			break;
		case 69:
			*_returnValue = _kern_get_extended_team_info((team_id)*(long*)args, (uint32)*(long*)((char*)args + 8), *(void **)((char*)args + 16), *(size_t*)((char*)args + 24), *(size_t **)((char*)args + 32));
			break;
		case 70:
			*_returnValue = _kern_start_watching_system((int32)*(long*)args, (uint32)*(long*)((char*)args + 8), (port_id)*(long*)((char*)args + 16), (int32)*(long*)((char*)args + 24));
			break;
		case 71:
			*_returnValue = _kern_stop_watching_system((int32)*(long*)args, (uint32)*(long*)((char*)args + 8), (port_id)*(long*)((char*)args + 16), (int32)*(long*)((char*)args + 24));
			break;
		case 72:
			*_returnValue = _kern_block_thread((uint32)*(long*)args, *(bigtime_t*)((char*)args + 8));
			break;
		case 73:
			*_returnValue = _kern_unblock_thread((thread_id)*(long*)args, (status_t)*(long*)((char*)args + 8));
			break;
		case 74:
			*_returnValue = _kern_unblock_threads(*(thread_id **)args, (uint32)*(long*)((char*)args + 8), (status_t)*(long*)((char*)args + 16));
			break;
		case 75:
			*_returnValue = _kern_estimate_max_scheduling_latency((thread_id)*(long*)args);
			break;
		case 76:
			*_returnValue = _kern_set_scheduler_mode((int32)*(long*)args);
			break;
		case 77:
			*_returnValue = _kern_get_scheduler_mode();
			break;
		case 78:
			*_returnValue = _kern_getgid((bool)*(long*)args);
			break;
		case 79:
			*_returnValue = _kern_getuid((bool)*(long*)args);
			break;
		case 80:
			*_returnValue = _kern_setregid((gid_t)*(long*)args, (gid_t)*(long*)((char*)args + 8), (bool)*(long*)((char*)args + 16));
			break;
		case 81:
			*_returnValue = _kern_setreuid((uid_t)*(long*)args, (uid_t)*(long*)((char*)args + 8), (bool)*(long*)((char*)args + 16));
			break;
		case 82:
			*_returnValue = _kern_getgroups((int)*(long*)args, *(gid_t **)((char*)args + 8));
			break;
		case 83:
			*_returnValue = _kern_setgroups((int)*(long*)args, *(const gid_t **)((char*)args + 8));
			break;
		case 84:
			*_returnValue = _kern_send_signal((int32)*(long*)args, (uint32)*(long*)((char*)args + 8), *(const union sigval **)((char*)args + 16), (uint32)*(long*)((char*)args + 24));
			break;
		case 85:
			*_returnValue = _kern_set_signal_mask((int)*(long*)args, *(const sigset_t **)((char*)args + 8), *(sigset_t **)((char*)args + 16));
			break;
		case 86:
			*_returnValue = _kern_sigaction((int)*(long*)args, *(const struct sigaction **)((char*)args + 8), *(struct sigaction **)((char*)args + 16));
			break;
		case 87:
			*_returnValue = _kern_sigwait(*(const sigset_t **)args, *(siginfo_t **)((char*)args + 8), (uint32)*(long*)((char*)args + 16), *(bigtime_t*)((char*)args + 24));
			break;
		case 88:
			*_returnValue = _kern_sigsuspend(*(const sigset_t **)args);
			break;
		case 89:
			*_returnValue = _kern_sigpending(*(sigset_t **)args);
			break;
		case 90:
			*_returnValue = _kern_set_signal_stack(*(const stack_t **)args, *(stack_t **)((char*)args + 8));
			break;
		case 91:
			*_returnValue = _kern_register_image(*(extended_image_info **)args, *(size_t*)((char*)args + 8));
			break;
		case 92:
			*_returnValue = _kern_unregister_image((image_id)*(long*)args);
			break;
		case 93:
			 _kern_image_relocated((image_id)*(long*)args);
			break;
		case 94:
			 _kern_loading_app_failed((status_t)*(long*)args);
			break;
		case 95:
			*_returnValue = _kern_get_image_info((image_id)*(long*)args, *(image_info **)((char*)args + 8), *(size_t*)((char*)args + 16));
			break;
		case 96:
			*_returnValue = _kern_get_next_image_info((team_id)*(long*)args, *(int32 **)((char*)args + 8), *(image_info **)((char*)args + 16), *(size_t*)((char*)args + 24));
			break;
		case 97:
			*_returnValue = _kern_read_kernel_image_symbols((image_id)*(long*)args, *(elf_sym **)((char*)args + 8), *(int32 **)((char*)args + 16), *(char **)((char*)args + 24), *(size_t **)((char*)args + 32), *(addr_t **)((char*)args + 40));
			break;
		case 98:
			*_returnValue = _kern_mount(*(const char **)args, *(const char **)((char*)args + 8), *(const char **)((char*)args + 16), (uint32)*(long*)((char*)args + 24), *(const char **)((char*)args + 32), *(size_t*)((char*)args + 40));
			break;
		case 99:
			*_returnValue = _kern_unmount(*(const char **)args, (uint32)*(long*)((char*)args + 8));
			break;
		case 100:
			*_returnValue = _kern_read_fs_info((dev_t)*(long*)args, *(struct fs_info **)((char*)args + 8));
			break;
		case 101:
			*_returnValue = _kern_write_fs_info((dev_t)*(long*)args, *(const struct fs_info **)((char*)args + 8), (int)*(long*)((char*)args + 16));
			break;
		case 102:
			*_returnValue = _kern_next_device(*(int32 **)args);
			break;
		case 103:
			*_returnValue = _kern_sync();
			break;
		case 104:
			*_returnValue = _kern_entry_ref_to_path((dev_t)*(long*)args, *(ino_t*)((char*)args + 8), *(const char **)((char*)args + 16), *(char **)((char*)args + 24), *(size_t*)((char*)args + 32));
			break;
		case 105:
			*_returnValue = _kern_normalize_path(*(const char **)args, (bool)*(long*)((char*)args + 8), *(char **)((char*)args + 16));
			break;
		case 106:
			*_returnValue = _kern_open_entry_ref((dev_t)*(long*)args, *(ino_t*)((char*)args + 8), *(const char **)((char*)args + 16), (int)*(long*)((char*)args + 24), (int)*(long*)((char*)args + 32));
			break;
		case 107:
			*_returnValue = _kern_open((int)*(long*)args, *(const char **)((char*)args + 8), (int)*(long*)((char*)args + 16), (int)*(long*)((char*)args + 24));
			break;
		case 108:
			*_returnValue = _kern_open_dir_entry_ref((dev_t)*(long*)args, *(ino_t*)((char*)args + 8), *(const char **)((char*)args + 16));
			break;
		case 109:
			*_returnValue = _kern_open_dir((int)*(long*)args, *(const char **)((char*)args + 8));
			break;
		case 110:
			*_returnValue = _kern_open_parent_dir((int)*(long*)args, *(char **)((char*)args + 8), *(size_t*)((char*)args + 16));
			break;
		case 111:
			*_returnValue = _kern_fcntl((int)*(long*)args, (int)*(long*)((char*)args + 8), *(size_t*)((char*)args + 16));
			break;
		case 112:
			*_returnValue = _kern_fsync((int)*(long*)args);
			break;
		case 113:
			*_returnValue = _kern_flock((int)*(long*)args, (int)*(long*)((char*)args + 8));
			break;
		case 114:
			*_returnValue = _kern_seek((int)*(long*)args, *(off_t*)((char*)args + 8), (int)*(long*)((char*)args + 16));
			break;
		case 115:
			*_returnValue = _kern_create_dir_entry_ref((dev_t)*(long*)args, *(ino_t*)((char*)args + 8), *(const char **)((char*)args + 16), (int)*(long*)((char*)args + 24));
			break;
		case 116:
			*_returnValue = _kern_create_dir((int)*(long*)args, *(const char **)((char*)args + 8), (int)*(long*)((char*)args + 16));
			break;
		case 117:
			*_returnValue = _kern_remove_dir((int)*(long*)args, *(const char **)((char*)args + 8));
			break;
		case 118:
			*_returnValue = _kern_read_link((int)*(long*)args, *(const char **)((char*)args + 8), *(char **)((char*)args + 16), *(size_t **)((char*)args + 24));
			break;
		case 119:
			*_returnValue = _kern_create_symlink((int)*(long*)args, *(const char **)((char*)args + 8), *(const char **)((char*)args + 16), (int)*(long*)((char*)args + 24));
			break;
		case 120:
			*_returnValue = _kern_create_link((int)*(long*)args, *(const char **)((char*)args + 8), (int)*(long*)((char*)args + 16), *(const char **)((char*)args + 24), (bool)*(long*)((char*)args + 32));
			break;
		case 121:
			*_returnValue = _kern_unlink((int)*(long*)args, *(const char **)((char*)args + 8));
			break;
		case 122:
			*_returnValue = _kern_rename((int)*(long*)args, *(const char **)((char*)args + 8), (int)*(long*)((char*)args + 16), *(const char **)((char*)args + 24));
			break;
		case 123:
			*_returnValue = _kern_create_fifo((int)*(long*)args, *(const char **)((char*)args + 8), (mode_t)*(long*)((char*)args + 16));
			break;
		case 124:
			*_returnValue = _kern_create_pipe(*(int **)args);
			break;
		case 125:
			*_returnValue = _kern_access((int)*(long*)args, *(const char **)((char*)args + 8), (int)*(long*)((char*)args + 16), (bool)*(long*)((char*)args + 24));
			break;
		case 126:
			*_returnValue = _kern_select((int)*(long*)args, *(struct fd_set **)((char*)args + 8), *(struct fd_set **)((char*)args + 16), *(struct fd_set **)((char*)args + 24), *(bigtime_t*)((char*)args + 32), *(const sigset_t **)((char*)args + 40));
			break;
		case 127:
			*_returnValue = _kern_poll(*(struct pollfd **)args, (int)*(long*)((char*)args + 8), *(bigtime_t*)((char*)args + 16), *(const sigset_t **)((char*)args + 24));
			break;
		case 128:
			*_returnValue = _kern_open_attr_dir((int)*(long*)args, *(const char **)((char*)args + 8), (bool)*(long*)((char*)args + 16));
			break;
		case 129:
			*_returnValue = _kern_read_attr((int)*(long*)args, *(const char **)((char*)args + 8), *(off_t*)((char*)args + 16), *(void **)((char*)args + 24), *(size_t*)((char*)args + 32));
			break;
		case 130:
			*_returnValue = _kern_write_attr((int)*(long*)args, *(const char **)((char*)args + 8), (uint32)*(long*)((char*)args + 16), *(off_t*)((char*)args + 24), *(const void **)((char*)args + 32), *(size_t*)((char*)args + 40));
			break;
		case 131:
			*_returnValue = _kern_stat_attr((int)*(long*)args, *(const char **)((char*)args + 8), *(struct attr_info **)((char*)args + 16));
			break;
		case 132:
			*_returnValue = _kern_open_attr((int)*(long*)args, *(const char **)((char*)args + 8), *(const char **)((char*)args + 16), (uint32)*(long*)((char*)args + 24), (int)*(long*)((char*)args + 32));
			break;
		case 133:
			*_returnValue = _kern_remove_attr((int)*(long*)args, *(const char **)((char*)args + 8));
			break;
		case 134:
			*_returnValue = _kern_rename_attr((int)*(long*)args, *(const char **)((char*)args + 8), (int)*(long*)((char*)args + 16), *(const char **)((char*)args + 24));
			break;
		case 135:
			*_returnValue = _kern_open_index_dir((dev_t)*(long*)args);
			break;
		case 136:
			*_returnValue = _kern_create_index((dev_t)*(long*)args, *(const char **)((char*)args + 8), (uint32)*(long*)((char*)args + 16), (uint32)*(long*)((char*)args + 24));
			break;
		case 137:
			*_returnValue = _kern_read_index_stat((dev_t)*(long*)args, *(const char **)((char*)args + 8), *(struct stat **)((char*)args + 16));
			break;
		case 138:
			*_returnValue = _kern_remove_index((dev_t)*(long*)args, *(const char **)((char*)args + 8));
			break;
		case 139:
			*_returnValue = _kern_getcwd(*(char **)args, *(size_t*)((char*)args + 8));
			break;
		case 140:
			*_returnValue = _kern_setcwd((int)*(long*)args, *(const char **)((char*)args + 8));
			break;
		case 141:
			*_returnValue = _kern_open_query((dev_t)*(long*)args, *(const char **)((char*)args + 8), *(size_t*)((char*)args + 16), (uint32)*(long*)((char*)args + 24), (port_id)*(long*)((char*)args + 32), (int32)*(long*)((char*)args + 40));
			break;
		case 142:
			*_returnValue = _kern_read((int)*(long*)args, *(off_t*)((char*)args + 8), *(void **)((char*)args + 16), *(size_t*)((char*)args + 24));
			break;
		case 143:
			*_returnValue = _kern_readv((int)*(long*)args, *(off_t*)((char*)args + 8), *(const struct iovec **)((char*)args + 16), *(size_t*)((char*)args + 24));
			break;
		case 144:
			*_returnValue = _kern_write((int)*(long*)args, *(off_t*)((char*)args + 8), *(const void **)((char*)args + 16), *(size_t*)((char*)args + 24));
			break;
		case 145:
			*_returnValue = _kern_writev((int)*(long*)args, *(off_t*)((char*)args + 8), *(const struct iovec **)((char*)args + 16), *(size_t*)((char*)args + 24));
			break;
		case 146:
			*_returnValue = _kern_ioctl((int)*(long*)args, (uint32)*(long*)((char*)args + 8), *(void **)((char*)args + 16), *(size_t*)((char*)args + 24));
			break;
		case 147:
			*_returnValue = _kern_read_dir((int)*(long*)args, *(struct dirent **)((char*)args + 8), *(size_t*)((char*)args + 16), (uint32)*(long*)((char*)args + 24));
			break;
		case 148:
			*_returnValue = _kern_rewind_dir((int)*(long*)args);
			break;
		case 149:
			*_returnValue = _kern_read_stat((int)*(long*)args, *(const char **)((char*)args + 8), (bool)*(long*)((char*)args + 16), *(struct stat **)((char*)args + 24), *(size_t*)((char*)args + 32));
			break;
		case 150:
			*_returnValue = _kern_write_stat((int)*(long*)args, *(const char **)((char*)args + 8), (bool)*(long*)((char*)args + 16), *(const struct stat **)((char*)args + 24), *(size_t*)((char*)args + 32), (int)*(long*)((char*)args + 40));
			break;
		case 151:
			*_returnValue = _kern_close((int)*(long*)args);
			break;
		case 152:
			*_returnValue = _kern_dup((int)*(long*)args);
			break;
		case 153:
			*_returnValue = _kern_dup2((int)*(long*)args, (int)*(long*)((char*)args + 8));
			break;
		case 154:
			*_returnValue = _kern_lock_node((int)*(long*)args);
			break;
		case 155:
			*_returnValue = _kern_unlock_node((int)*(long*)args);
			break;
		case 156:
			*_returnValue = _kern_get_next_fd_info((team_id)*(long*)args, *(uint32 **)((char*)args + 8), *(struct fd_info **)((char*)args + 16), *(size_t*)((char*)args + 24));
			break;
		case 157:
			*_returnValue = _kern_preallocate((int)*(long*)args, *(off_t*)((char*)args + 8), *(off_t*)((char*)args + 16));
			break;
		case 158:
			*_returnValue = _kern_socket((int)*(long*)args, (int)*(long*)((char*)args + 8), (int)*(long*)((char*)args + 16));
			break;
		case 159:
			*_returnValue = _kern_bind((int)*(long*)args, *(const struct sockaddr **)((char*)args + 8), (socklen_t)*(long*)((char*)args + 16));
			break;
		case 160:
			*_returnValue = _kern_shutdown_socket((int)*(long*)args, (int)*(long*)((char*)args + 8));
			break;
		case 161:
			*_returnValue = _kern_connect((int)*(long*)args, *(const struct sockaddr **)((char*)args + 8), (socklen_t)*(long*)((char*)args + 16));
			break;
		case 162:
			*_returnValue = _kern_listen((int)*(long*)args, (int)*(long*)((char*)args + 8));
			break;
		case 163:
			*_returnValue = _kern_accept((int)*(long*)args, *(struct sockaddr **)((char*)args + 8), *(socklen_t **)((char*)args + 16));
			break;
		case 164:
			*_returnValue = _kern_recv((int)*(long*)args, *(void **)((char*)args + 8), *(size_t*)((char*)args + 16), (int)*(long*)((char*)args + 24));
			break;
		case 165:
			*_returnValue = _kern_recvfrom((int)*(long*)args, *(void **)((char*)args + 8), *(size_t*)((char*)args + 16), (int)*(long*)((char*)args + 24), *(struct sockaddr **)((char*)args + 32), *(socklen_t **)((char*)args + 40));
			break;
		case 166:
			*_returnValue = _kern_recvmsg((int)*(long*)args, *(struct msghdr **)((char*)args + 8), (int)*(long*)((char*)args + 16));
			break;
		case 167:
			*_returnValue = _kern_send((int)*(long*)args, *(const void **)((char*)args + 8), *(size_t*)((char*)args + 16), (int)*(long*)((char*)args + 24));
			break;
		case 168:
			*_returnValue = _kern_sendto((int)*(long*)args, *(const void **)((char*)args + 8), *(size_t*)((char*)args + 16), (int)*(long*)((char*)args + 24), *(const struct sockaddr **)((char*)args + 32), (socklen_t)*(long*)((char*)args + 40));
			break;
		case 169:
			*_returnValue = _kern_sendmsg((int)*(long*)args, *(const struct msghdr **)((char*)args + 8), (int)*(long*)((char*)args + 16));
			break;
		case 170:
			*_returnValue = _kern_getsockopt((int)*(long*)args, (int)*(long*)((char*)args + 8), (int)*(long*)((char*)args + 16), *(void **)((char*)args + 24), *(socklen_t **)((char*)args + 32));
			break;
		case 171:
			*_returnValue = _kern_setsockopt((int)*(long*)args, (int)*(long*)((char*)args + 8), (int)*(long*)((char*)args + 16), *(const void **)((char*)args + 24), (socklen_t)*(long*)((char*)args + 32));
			break;
		case 172:
			*_returnValue = _kern_getpeername((int)*(long*)args, *(struct sockaddr **)((char*)args + 8), *(socklen_t **)((char*)args + 16));
			break;
		case 173:
			*_returnValue = _kern_getsockname((int)*(long*)args, *(struct sockaddr **)((char*)args + 8), *(socklen_t **)((char*)args + 16));
			break;
		case 174:
			*_returnValue = _kern_sockatmark((int)*(long*)args);
			break;
		case 175:
			*_returnValue = _kern_socketpair((int)*(long*)args, (int)*(long*)((char*)args + 8), (int)*(long*)((char*)args + 16), *(int **)((char*)args + 24));
			break;
		case 176:
			*_returnValue = _kern_get_next_socket_stat((int)*(long*)args, *(uint32 **)((char*)args + 8), *(struct net_stat **)((char*)args + 16));
			break;
		case 177:
			*_returnValue = _kern_stop_notifying((port_id)*(long*)args, (uint32)*(long*)((char*)args + 8));
			break;
		case 178:
			*_returnValue = _kern_start_watching((dev_t)*(long*)args, *(ino_t*)((char*)args + 8), (uint32)*(long*)((char*)args + 16), (port_id)*(long*)((char*)args + 24), (uint32)*(long*)((char*)args + 32));
			break;
		case 179:
			*_returnValue = _kern_stop_watching((dev_t)*(long*)args, *(ino_t*)((char*)args + 8), (port_id)*(long*)((char*)args + 16), (uint32)*(long*)((char*)args + 24));
			break;
		case 180:
			*_returnValue = _kern_set_real_time_clock(*(bigtime_t*)args);
			break;
		case 181:
			*_returnValue = _kern_set_timezone((int32)*(long*)args, *(const char **)((char*)args + 8), *(size_t*)((char*)args + 16));
			break;
		case 182:
			*_returnValue = _kern_get_timezone(*(int32 **)args, *(char **)((char*)args + 8), *(size_t*)((char*)args + 16));
			break;
		case 183:
			*_returnValue = _kern_set_real_time_clock_is_gmt((bool)*(long*)args);
			break;
		case 184:
			*_returnValue = _kern_get_real_time_clock_is_gmt(*(bool **)args);
			break;
		case 185:
			*_returnValue = _kern_get_clock((clockid_t)*(long*)args, *(bigtime_t **)((char*)args + 8));
			break;
		case 186:
			*_returnValue = _kern_set_clock((clockid_t)*(long*)args, *(bigtime_t*)((char*)args + 8));
			break;
		case 187:
			*_returnValue = _kern_system_time();
			break;
		case 188:
			*_returnValue = _kern_snooze_etc(*(bigtime_t*)args, (int)*(long*)((char*)args + 8), (int32)*(long*)((char*)args + 16), *(bigtime_t **)((char*)args + 24));
			break;
		case 189:
			*_returnValue = _kern_create_timer((clockid_t)*(long*)args, (thread_id)*(long*)((char*)args + 8), (uint32)*(long*)((char*)args + 16), *(const struct sigevent **)((char*)args + 24), *(const struct thread_creation_attributes **)((char*)args + 32));
			break;
		case 190:
			*_returnValue = _kern_delete_timer((int32)*(long*)args, (thread_id)*(long*)((char*)args + 8));
			break;
		case 191:
			*_returnValue = _kern_get_timer((int32)*(long*)args, (thread_id)*(long*)((char*)args + 8), *(struct user_timer_info **)((char*)args + 16));
			break;
		case 192:
			*_returnValue = _kern_set_timer((int32)*(long*)args, (thread_id)*(long*)((char*)args + 8), *(bigtime_t*)((char*)args + 16), *(bigtime_t*)((char*)args + 24), (uint32)*(long*)((char*)args + 32), *(struct user_timer_info **)((char*)args + 40));
			break;
		case 193:
			*_returnValue = _kern_create_area(*(const char **)args, *(void * **)((char*)args + 8), (uint32)*(long*)((char*)args + 16), *(size_t*)((char*)args + 24), (uint32)*(long*)((char*)args + 32), (uint32)*(long*)((char*)args + 40));
			break;
		case 194:
			*_returnValue = _kern_delete_area((area_id)*(long*)args);
			break;
		case 195:
			*_returnValue = _kern_area_for(*(void **)args);
			break;
		case 196:
			*_returnValue = _kern_find_area(*(const char **)args);
			break;
		case 197:
			*_returnValue = _kern_get_area_info((area_id)*(long*)args, *(area_info **)((char*)args + 8));
			break;
		case 198:
			*_returnValue = _kern_get_next_area_info((team_id)*(long*)args, *(ssize_t **)((char*)args + 8), *(area_info **)((char*)args + 16));
			break;
		case 199:
			*_returnValue = _kern_resize_area((area_id)*(long*)args, *(size_t*)((char*)args + 8));
			break;
		case 200:
			*_returnValue = _kern_transfer_area((area_id)*(long*)args, *(void * **)((char*)args + 8), (uint32)*(long*)((char*)args + 16), (team_id)*(long*)((char*)args + 24));
			break;
		case 201:
			*_returnValue = _kern_set_area_protection((area_id)*(long*)args, (uint32)*(long*)((char*)args + 8));
			break;
		case 202:
			*_returnValue = _kern_clone_area(*(const char **)args, *(void * **)((char*)args + 8), (uint32)*(long*)((char*)args + 16), (uint32)*(long*)((char*)args + 24), (area_id)*(long*)((char*)args + 32));
			break;
		case 203:
			*_returnValue = _kern_reserve_address_range(*(addr_t **)args, (uint32)*(long*)((char*)args + 8), *(addr_t*)((char*)args + 16));
			break;
		case 204:
			*_returnValue = _kern_unreserve_address_range(*(addr_t*)args, *(addr_t*)((char*)args + 8));
			break;
		case 205:
			*_returnValue = _kern_map_file(*(const char **)args, *(void * **)((char*)args + 8), (uint32)*(long*)((char*)args + 16), *(size_t*)((char*)args + 24), (uint32)*(long*)((char*)args + 32), (uint32)*(long*)((char*)args + 40), (bool)*(long*)((char*)args + 48), (int)*(long*)((char*)args + 56), *(off_t*)((char*)args + 64));
			break;
		case 206:
			*_returnValue = _kern_unmap_memory(*(void **)args, *(size_t*)((char*)args + 8));
			break;
		case 207:
			*_returnValue = _kern_set_memory_protection(*(void **)args, *(size_t*)((char*)args + 8), (uint32)*(long*)((char*)args + 16));
			break;
		case 208:
			*_returnValue = _kern_sync_memory(*(void **)args, *(size_t*)((char*)args + 8), (int)*(long*)((char*)args + 16));
			break;
		case 209:
			*_returnValue = _kern_memory_advice(*(void **)args, *(size_t*)((char*)args + 8), (uint32)*(long*)((char*)args + 16));
			break;
		case 210:
			*_returnValue = _kern_get_memory_properties((team_id)*(long*)args, *(const void **)((char*)args + 8), *(uint32 **)((char*)args + 16), *(uint32 **)((char*)args + 24));
			break;
		case 211:
			*_returnValue = _kern_mlock(*(const void **)args, *(size_t*)((char*)args + 8));
			break;
		case 212:
			*_returnValue = _kern_munlock(*(const void **)args, *(size_t*)((char*)args + 8));
			break;
		case 213:
			*_returnValue = _kern_create_port((int32)*(long*)args, *(const char **)((char*)args + 8));
			break;
		case 214:
			*_returnValue = _kern_close_port((port_id)*(long*)args);
			break;
		case 215:
			*_returnValue = _kern_delete_port((port_id)*(long*)args);
			break;
		case 216:
			*_returnValue = _kern_find_port(*(const char **)args);
			break;
		case 217:
			*_returnValue = _kern_get_port_info((port_id)*(long*)args, *(struct port_info **)((char*)args + 8));
			break;
		case 218:
			*_returnValue = _kern_get_next_port_info((team_id)*(long*)args, *(int32 **)((char*)args + 8), *(struct port_info **)((char*)args + 16));
			break;
		case 219:
			*_returnValue = _kern_port_buffer_size_etc((port_id)*(long*)args, (uint32)*(long*)((char*)args + 8), *(bigtime_t*)((char*)args + 16));
			break;
		case 220:
			*_returnValue = _kern_port_count((port_id)*(long*)args);
			break;
		case 221:
			*_returnValue = _kern_read_port_etc((port_id)*(long*)args, *(int32 **)((char*)args + 8), *(void **)((char*)args + 16), *(size_t*)((char*)args + 24), (uint32)*(long*)((char*)args + 32), *(bigtime_t*)((char*)args + 40));
			break;
		case 222:
			*_returnValue = _kern_set_port_owner((port_id)*(long*)args, (team_id)*(long*)((char*)args + 8));
			break;
		case 223:
			*_returnValue = _kern_write_port_etc((port_id)*(long*)args, (int32)*(long*)((char*)args + 8), *(const void **)((char*)args + 16), *(size_t*)((char*)args + 24), (uint32)*(long*)((char*)args + 32), *(bigtime_t*)((char*)args + 40));
			break;
		case 224:
			*_returnValue = _kern_writev_port_etc((port_id)*(long*)args, (int32)*(long*)((char*)args + 8), *(const struct iovec **)((char*)args + 16), *(size_t*)((char*)args + 24), *(size_t*)((char*)args + 32), (uint32)*(long*)((char*)args + 40), *(bigtime_t*)((char*)args + 48));
			break;
		case 225:
			*_returnValue = _kern_get_port_message_info_etc((port_id)*(long*)args, *(port_message_info **)((char*)args + 8), *(size_t*)((char*)args + 16), (uint32)*(long*)((char*)args + 24), *(bigtime_t*)((char*)args + 32));
			break;
		case 226:
			*_returnValue = _kern_kernel_debugger(*(const char **)args);
			break;
		case 227:
			 _kern_register_syslog_daemon((port_id)*(long*)args);
			break;
		case 228:
			 _kern_debugger(*(const char **)args);
			break;
		case 229:
			*_returnValue = _kern_disable_debugger((int)*(long*)args);
			break;
		case 230:
			*_returnValue = _kern_install_default_debugger((port_id)*(long*)args);
			break;
		case 231:
			*_returnValue = _kern_install_team_debugger((team_id)*(long*)args, (port_id)*(long*)((char*)args + 8));
			break;
		case 232:
			*_returnValue = _kern_remove_team_debugger((team_id)*(long*)args);
			break;
		case 233:
			*_returnValue = _kern_debug_thread((thread_id)*(long*)args);
			break;
		case 234:
			_kern_wait_for_debugger();
			break;
		case 235:
			*_returnValue = _kern_set_debugger_breakpoint(*(void **)args, (uint32)*(long*)((char*)args + 8), (int32)*(long*)((char*)args + 16), (bool)*(long*)((char*)args + 24));
			break;
		case 236:
			*_returnValue = _kern_clear_debugger_breakpoint(*(void **)args, (bool)*(long*)((char*)args + 8));
			break;
		case 237:
			*_returnValue = _kern_system_profiler_start(*(struct system_profiler_parameters **)args);
			break;
		case 238:
			*_returnValue = _kern_system_profiler_next_buffer(*(size_t*)args, *(uint64 **)((char*)args + 8));
			break;
		case 239:
			*_returnValue = _kern_system_profiler_stop();
			break;
		case 240:
			*_returnValue = _kern_system_profiler_recorded(*(struct system_profiler_parameters **)args);
			break;
		case 241:
			*_returnValue = _kern_get_system_info(*(system_info **)args);
			break;
		case 242:
			*_returnValue = _kern_get_cpu_info((uint32)*(long*)args, (uint32)*(long*)((char*)args + 8), *(cpu_info **)((char*)args + 16));
			break;
		case 243:
			*_returnValue = _kern_get_cpu_topology_info(*(cpu_topology_node_info **)args, *(uint32 **)((char*)args + 8));
			break;
		case 244:
			*_returnValue = _kern_analyze_scheduling(*(bigtime_t*)args, *(bigtime_t*)((char*)args + 8), *(void **)((char*)args + 16), *(size_t*)((char*)args + 24), *(struct scheduling_analysis **)((char*)args + 32));
			break;
		case 245:
			_kern_debug_output(*(const char **)args);
			break;
		case 246:
			_kern_ktrace_output(*(const char **)args);
			break;
		case 247:
			*_returnValue = _kern_frame_buffer_update(*(addr_t*)args, (int32)*(long*)((char*)args + 8), (int32)*(long*)((char*)args + 16), (int32)*(long*)((char*)args + 24), (int32)*(long*)((char*)args + 32));
			break;
		case 248:
			*_returnValue = _kern_register_messaging_service((sem_id)*(long*)args, (sem_id)*(long*)((char*)args + 8));
			break;
		case 249:
			*_returnValue = _kern_unregister_messaging_service();
			break;
		case 250:
			_kern_clear_caches(*(void **)args, *(size_t*)((char*)args + 8), (uint32)*(long*)((char*)args + 16));
			break;
		case 251:
			*_returnValue = _kern_cpu_enabled((int32)*(long*)args);
			break;
		case 252:
			*_returnValue = _kern_set_cpu_enabled((int32)*(long*)args, (bool)*(long*)((char*)args + 8));
			break;
		case 253:
			*_returnValue = _kern_get_next_disk_device_id(*(int32 **)args, *(size_t **)((char*)args + 8));
			break;
		case 254:
			*_returnValue = _kern_find_disk_device(*(const char **)args, *(size_t **)((char*)args + 8));
			break;
		case 255:
			*_returnValue = _kern_find_partition(*(const char **)args, *(size_t **)((char*)args + 8));
			break;
		case 256:
			*_returnValue = _kern_find_file_disk_device(*(const char **)args, *(size_t **)((char*)args + 8));
			break;
		case 257:
			*_returnValue = _kern_get_disk_device_data((partition_id)*(long*)args, (bool)*(long*)((char*)args + 8), *(struct user_disk_device_data **)((char*)args + 16), *(size_t*)((char*)args + 24), *(size_t **)((char*)args + 32));
			break;
		case 258:
			*_returnValue = _kern_register_file_device(*(const char **)args);
			break;
		case 259:
			*_returnValue = _kern_unregister_file_device((partition_id)*(long*)args, *(const char **)((char*)args + 8));
			break;
		case 260:
			*_returnValue = _kern_get_file_disk_device_path((partition_id)*(long*)args, *(char **)((char*)args + 8), *(size_t*)((char*)args + 16));
			break;
		case 261:
			*_returnValue = _kern_get_disk_system_info((disk_system_id)*(long*)args, *(struct user_disk_system_info **)((char*)args + 8));
			break;
		case 262:
			*_returnValue = _kern_get_next_disk_system_info(*(int32 **)args, *(struct user_disk_system_info **)((char*)args + 8));
			break;
		case 263:
			*_returnValue = _kern_find_disk_system(*(const char **)args, *(struct user_disk_system_info **)((char*)args + 8));
			break;
		case 264:
			*_returnValue = _kern_defragment_partition((partition_id)*(long*)args, *(int32 **)((char*)args + 8));
			break;
		case 265:
			*_returnValue = _kern_repair_partition((partition_id)*(long*)args, *(int32 **)((char*)args + 8), (bool)*(long*)((char*)args + 16));
			break;
		case 266:
			*_returnValue = _kern_resize_partition((partition_id)*(long*)args, *(int32 **)((char*)args + 8), (partition_id)*(long*)((char*)args + 16), *(int32 **)((char*)args + 24), *(off_t*)((char*)args + 32), *(off_t*)((char*)args + 40));
			break;
		case 267:
			*_returnValue = _kern_move_partition((partition_id)*(long*)args, *(int32 **)((char*)args + 8), (partition_id)*(long*)((char*)args + 16), *(int32 **)((char*)args + 24), *(off_t*)((char*)args + 32), *(partition_id **)((char*)args + 40), *(int32 **)((char*)args + 48), (int32)*(long*)((char*)args + 56));
			break;
		case 268:
			*_returnValue = _kern_set_partition_name((partition_id)*(long*)args, *(int32 **)((char*)args + 8), (partition_id)*(long*)((char*)args + 16), *(int32 **)((char*)args + 24), *(const char **)((char*)args + 32));
			break;
		case 269:
			*_returnValue = _kern_set_partition_content_name((partition_id)*(long*)args, *(int32 **)((char*)args + 8), *(const char **)((char*)args + 16));
			break;
		case 270:
			*_returnValue = _kern_set_partition_type((partition_id)*(long*)args, *(int32 **)((char*)args + 8), (partition_id)*(long*)((char*)args + 16), *(int32 **)((char*)args + 24), *(const char **)((char*)args + 32));
			break;
		case 271:
			*_returnValue = _kern_set_partition_parameters((partition_id)*(long*)args, *(int32 **)((char*)args + 8), (partition_id)*(long*)((char*)args + 16), *(int32 **)((char*)args + 24), *(const char **)((char*)args + 32));
			break;
		case 272:
			*_returnValue = _kern_set_partition_content_parameters((partition_id)*(long*)args, *(int32 **)((char*)args + 8), *(const char **)((char*)args + 16));
			break;
		case 273:
			*_returnValue = _kern_initialize_partition((partition_id)*(long*)args, *(int32 **)((char*)args + 8), *(const char **)((char*)args + 16), *(const char **)((char*)args + 24), *(const char **)((char*)args + 32));
			break;
		case 274:
			*_returnValue = _kern_uninitialize_partition((partition_id)*(long*)args, *(int32 **)((char*)args + 8), (partition_id)*(long*)((char*)args + 16), *(int32 **)((char*)args + 24));
			break;
		case 275:
			*_returnValue = _kern_create_child_partition((partition_id)*(long*)args, *(int32 **)((char*)args + 8), *(off_t*)((char*)args + 16), *(off_t*)((char*)args + 24), *(const char **)((char*)args + 32), *(const char **)((char*)args + 40), *(const char **)((char*)args + 48), *(partition_id **)((char*)args + 56), *(int32 **)((char*)args + 64));
			break;
		case 276:
			*_returnValue = _kern_delete_child_partition((partition_id)*(long*)args, *(int32 **)((char*)args + 8), (partition_id)*(long*)((char*)args + 16), (int32)*(long*)((char*)args + 24));
			break;
		case 277:
			*_returnValue = _kern_start_watching_disks((uint32)*(long*)args, (port_id)*(long*)((char*)args + 8), (int32)*(long*)((char*)args + 16));
			break;
		case 278:
			*_returnValue = _kern_stop_watching_disks((port_id)*(long*)args, (int32)*(long*)((char*)args + 8));
			break;
	}
}
