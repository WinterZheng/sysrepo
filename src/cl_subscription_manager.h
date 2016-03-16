/**
 * @file cl_subscription_manager.h
 * @author Rastislav Szabo <raszabo@cisco.com>, Lukas Macko <lmacko@cisco.com>
 * @brief Client Library's Subscription Manager API.
 *
 * @copyright
 * Copyright 2016 Cisco Systems, Inc.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef CL_SUBSCRIPTION_MANAGER_H_
#define CL_SUBSCRIPTION_MANAGER_H_

#include <pthread.h>

#include "sysrepo.h"
#include "sr_common.h"

/**
 * @defgroup cl_sm Client Library's Subscription Manager
 * @{
 *
 * @brief Internally tracks all subscriptions and provides the notification
 * communication channel between sysepo engine and client library.
 */

/**
 * @brief Client Subscription manager context.
 */
typedef struct cl_sm_ctx_s cl_sm_ctx_t;

/**
 * @brief Sysrepo subscription context.
 */
typedef struct sr_subscription_ctx_s {
    uint32_t id;                                 /**< Library-local subscription identifier. */
    Sr__NotificationEvent event_type;            /**< Type of the notification event subscribed to. */
    union {
        sr_feature_enable_cb feature_enable_cb;  /**< Callback to be called by feature enable/disable event. */
        sr_module_install_cb module_install_cb;  /**< Callback to be called by module (un)install event. */
    } callback;
    cl_sm_ctx_t *sm_ctx;                         /**< Associated Subscription Manager context. */
    void *private_ctx;                           /**< Private context pointer, opaque to sysrepo. */
} sr_subscription_ctx_t;

/**
 * @brief Initializes a Subscription Manager instance.
 *
 * @param[out] sm_ctx Subscription Manager context that can be used in subsequent SM API calls.
 *
 * @return Error code (SR_ERR_OK on success).
 */
int cl_sm_init(cl_sm_ctx_t **sm_ctx);

/**
 * @brief Cleans up the Subscription Manager.
 *
 * @param[in] sm_ctx Subscription Manager context acquired by ::cl_sm_init call.
 */
void cl_sm_cleanup(cl_sm_ctx_t *sm_ctx);

/**
 * @brief Initializes a new subscription.
 *
 * @param[in] sm_ctx Subscription Manager context acquired by ::cl_sm_init call.
 * @param[out] destination Pointer to a string that contains destination address
 * (socket path) where notification should be delivered. Do not free.
 * @param[out] subscription Allocated subscription context. Release by
 * ::cl_sm_subscription_cleanup call.
 *
 * @return Error code (SR_ERR_OK on success).
 */
int cl_sm_subscription_init(cl_sm_ctx_t *sm_ctx, char **destination, sr_subscription_ctx_t **subscription);

/**
 * @brief Cleans up a subscription.
 *
 * @param[in] subscription Subscription context acquired by ::cl_sm_subscription_init call.
 */
void cl_sm_subscription_cleanup(sr_subscription_ctx_t *subscription);

/**@} cl_sm */

#endif /* CL_SUBSCRIPTION_MANAGER_H_ */