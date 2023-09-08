/****************************************************************************
 * @brief Everything between extern "C" { and } is compiled as C code.
 ***************************************************************************/
#pragma once

#include <stdint.h>
#include <stdbool.h>


/****************************************************************************
 * @brief Everything between extern "C" { and } is compiled as C code.
 ***************************************************************************/
#ifdef __cplusplus
extern "C" {
#endif

#define SEMAPHORE_ID_MIN 0
#define SEMAPHORE_ID_MAX 31

/****************************************************************************
 * @brief Reserves semaphore with specified ID.
 * 
 * @param sem_id
 *        The ID of semaphore to reserve.
 * 
 * @return True after reservation, false if an invalid ID is passed. 
 ***************************************************************************/
bool semaphore_reserve(const uint8_t sem_id);

/****************************************************************************
 * @brief Releases semaphore with specified ID.
 * 
 * @param sem_id
 *        The ID of semaphore to release.
 * 
 * @return True after release, false if an invalid ID is passed. 
 ***************************************************************************/
bool semaphore_release(const uint8_t sem_id);

#ifdef __cplusplus
} /* extern C */
#endif