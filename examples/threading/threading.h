#include <stdbool.h>
#include <pthread.h>

/**
 * This structure should be dynamically allocated and passed as
 * an argument to your thread using pthread_create.
 * It should be returned by your thread so it can be freed by
 * the joiner thread.
 */
struct thread_data {
    /*
     * Values needed by the thread to manage its execution.
     */
    pthread_mutex_t *mutex;          // Mutex to be obtained and released
    int wait_to_obtain_ms;           // Time to wait before obtaining the mutex (in milliseconds)
    int wait_to_release_ms;          // Time to wait before releasing the mutex (in milliseconds)

    /**
     * Set to true if the thread completed with success, false
     * if an error occurred.
     */
    bool thread_complete_success;
};

/**
* Start a thread which sleeps @param wait_to_obtain_ms number of milliseconds, then obtains the
* mutex in @param mutex, then holds for @param wait_to_release_ms milliseconds, then releases.
* The start_thread_obtaining_mutex function should only start the thread and should not block
* for the thread to complete.
* The start_thread_obtaining_mutex function should use dynamic memory allocation for thread_data
* structure passed into the thread.  The number of threads active should be limited only by the
* amount of available memory.
* The thread started should return a pointer to the thread_data structure when it exits, which can be used
* to free memory as well as to check thread_complete_success for successful exit.
* If a thread was started succesfully @param thread should be filled with the pthread_create thread ID
* coresponding to the thread which was started.
* @return true if the thread could be started, false if a failure occurred.
*/
bool start_thread_obtaining_mutex(pthread_t *thread, pthread_mutex_t *mutex, int wait_to_obtain_ms, int wait_to_release_ms);
