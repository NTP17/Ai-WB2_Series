// Copyright 2015-2018 Axkressif Systems (Shanghai) PTE LTD
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at

//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#ifndef _AXK_TRANSPORT_UTILS_H_
#define _AXK_TRANSPORT_UTILS_H_
#include <sys/time.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief      Utility macro to be used for NULL ptr check after malloc
 *
 */
#define AXK_TRANSPORT_MEM_CHECK(TAG, a, action) if (!(a)) {                                         \
        blog_error("%s(%d): %s", __FUNCTION__, __LINE__, "Memory exhausted");                     \
        action;                                                                                     \
        }

/**
 * @brief      Utility macro for checking the error code of axk_err_t
 */
#define AXK_TRANSPORT_ERR_OK_CHECK(TAG, err, action) \
        {                                                     \
            axk_err_t _axk_transport_err_to_check = err;      \
            if (_axk_transport_err_to_check != AXK_OK) {      \
                blog_error("%s(%d): Expected AXK_OK; reported: %d", __FUNCTION__, __LINE__, _axk_transport_err_to_check); \
                action;                                       \
            }                                                 \
        }

/**
 * @brief      Convert milliseconds to timeval struct for valid timeouts, otherwise
 *             (if "wait forever" requested by timeout_ms=-1) timeval structure is not updated and NULL returned
 *
 * @param[in]  timeout_ms  The timeout value in milliseconds or -1 to waiting forever
 * @param[out] tv          Pointer to timeval struct
 *
 * @return
 * - NULL if timeout_ms=-1 (wait forever)
 * - pointer to the updated timeval structure (provided as "tv" argument) with recalculated timeout value
 */
struct timeval* axk_transport_utils_ms_to_timeval(int timeout_ms, struct timeval *tv);


#ifdef __cplusplus
}
#endif
#endif /* _AXK_TRANSPORT_UTILS_H_ */
