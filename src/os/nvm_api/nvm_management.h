/*
 * Copyright (c) 2018, Intel Corporation.
 * SPDX-License-Identifier: BSD-3-Clause
 */

/**
 * @file nvm_management.h
 * @brief The file describes the entry points of the Native Management API.
 * It is intended to be used by clients of the Native Management API
 * in order to perform management actions.
 *
 * @mainpage Apache Pass Software Management API
 *
 * @section Introduction
 * The native management API is provided as a convenience for the developers of management utilities.
 * The library serves as an abstraction layer above the underlying driver and operating system.
 * The intent of the abstraction is to simplify the interface, unify the API across operating systems
 * and drivers and to reduce programming errors in the applications utilizing the library.
 *
 * @subsection Compiling
 * The following header files are required to compile applications using the native management library:
 *
 *      - nvm_management.h: The native management API interface definition.
 *      - nvm_types.h: Common types used by the native management API.
 *
 * Please be sure to link with the -lixpdimm option when compiling.
 *
 * @subsection Versioning
 * The Management Library is versioned in two ways.  First, standard shared library versioning techniques are used so that the OS run-time linkers can combine applications with the appropriate version of the library if possible.  Second, C macros are provided to allow an application to determine and react to different versions of the library in different run-time environments.
 * The version is formatted as MM.mm.hh.bbbb where MM is the 2-digit major version (00-99), mm is the 2-digit minor version (00-99), hh is the 2-digit hot fix number (00-99), and bbbb is the 4-digit build number (0000-9999).
 * The following C macros and interfaces are provided to retrieve the native API version information.
 *
 * <table>
 * <tr><td>Synopsis</td><td><strong>int nvm_get_major_version</strong>();</td></tr>
 * <tr><td>Description</td><td>Retrieve the native API library major version number (00-99).</td></tr>
 * <tr><td>Arguments</td><td>None</td></tr>
 * <tr><td>Conditions</td><td>No limiting conditions apply to this function.</td></tr>
 * <tr><td>Remarks</td><td>Applications and the native API library are not compatible if they were written against different major versions.&nbsp; For this reason, it is recommended that every application that uses the native API library performs the following check:
 * if (nvm_get_major_version() != NVM_VERSION_MAJOR)
 * // The application cannot continue with this version of the library
 * </td></tr>
 * <tr><td>Returns</td><td>Returns the major version number.</td></tr>
 * </table>
 *
 * <table>
 * <tr><td>Synopsis</td><td><strong>int nvm_get_minor_version</strong>();</td></tr>
 * <tr><td>Description</td><td>Retrieve the native API library minor version number (00-99).</td></tr>
 * <tr><td>Arguments</td><td>None</td></tr>
 * <tr><td>Conditions</td><td>No limiting conditions apply to this function.</td></tr>
 * <tr><td>Remarks</td><td>Unless otherwise stated, every data structure, function, and description described in this document has existed with those exact semantics since version 1.0 of the native API library.  In cases where functions have been added, the appropriate section in this document will describe the version that introduced the new feature.  Applications wishing to check for features that were added may do so by comparing the return value from nvm_get_minor_version() against the minor number in this specification associated with the introduction of the new feature.
 * if (nvm_get_minor_version() != NVM_VERSION_MINOR)
 * // Specific APIs may not be supported
 * </td></tr>
 * <tr><td>Returns</td><td>Returns the minor version number.</td></tr>
 * </table>
 *
 * <table>
 * <tr><td>Synopsis</td><td><strong>int nvm_get_hotfix_number</strong>();</td></tr>
 * <tr><td>Description</td><td>Retrieve the native API library hot fix version number (00-99).</td></tr>
 * <tr><td>Arguments</td><td>None</td></tr>
 * <tr><td>Conditions</td><td>No limiting conditions apply to this function.</td></tr>
 * <tr><td>Remarks</td><td>The hotfix number is used when reporting incidents but has no significance with respect to library compatibility.
 * </td></tr>
 * <tr><td>Returns</td><td>Returns the hot fix version number.</td></tr>
 * </table>
 *
 * <table>
 * <tr><td>Synopsis</td><td><strong>int nvm_get_build_number</strong>();</td></tr>
 * <tr><td>Description</td><td>Retrieve the native API library build version number (0000-9999).</td></tr>
 * <tr><td>Arguments</td><td>None</td></tr>
 * <tr><td>Conditions</td><td>No limiting conditions apply to this function.</td></tr>
 * <tr><td>Remarks</td><td>The build number is used when reporting incidents but has no significance with respect to library compatibility.
 * </td></tr>
 * <tr><td>Returns</td><td>Returns the build version number.</td></tr>
 * </table>
 *
 * @subsection Caller Privileges
 * Unless otherwise specified, all interfaces require the caller to have administrative/root privileges. The library will return NVM_ERR_INVALIDPERMISSIONS if not.
 *
 * @subsection Return Codes
 * Each interface returns a code indicating the status of the operation as defined in ::return_code. Use nvm_get_error to convert the code into a textual description. Specific codes that may be returned by a particular interface are defined in the "Returns" section of each interface.
 *
 */

#ifndef _NVM_MANAGEMENT_H_
#define _NVM_MANAGEMENT_H_

#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <time.h>

#include "nvm_types.h"
#include "export_api.h"

#ifdef __cplusplus
extern "C"
{
#endif

#define NVM_VERSION_MAJOR   __VERSION_MAJOR__           // Major version number
#define NVM_VERSION_MINOR   __VERSION_MINOR__           // Minor version number
#define NVM_VERSION_HOTFIX   __VERSION_HOTFIX__         // Hot fix version number
#define NVM_VERSION_BUILDNUM    __VERSION_BUILDNUM__    // Build version number

/**
 * Convert an array of 8 unsigned chars into an unsigned 64 bit value
 * @remarks While it doesn't seem right to be casting 8 bit chars to unsigned long
 * long, this is an issue with gcc - see http:// gcc.gnu.org/bugzilla/show_bug.cgi?id=47821.
 */
#define NVM_8_BYTE_ARRAY_TO_64_BIT_VALUE(arr, val) \
	val = ((unsigned long long)(arr[7] & 0xFF) << 56) + \
	      ((unsigned long long)(arr[6] & 0xFF) << 48) + \
	      ((unsigned long long)(arr[5] & 0xFF) << 40) + \
	      ((unsigned long long)(arr[4] & 0xFF) << 32) + \
	      ((unsigned long long)(arr[3] & 0xFF) << 24) + \
	      ((unsigned long long)(arr[2] & 0xFF) << 16) + \
	      ((unsigned long long)(arr[1] & 0xFF) << 8) + \
	      (unsigned long long)(arr[0] & 0xFF);

/**
 * Convert an unsigned 64 bit integer to an array of 8 unsigned chars
 */
#define NVM_64_BIT_VALUE_TO_8_BYTE_ARRAY(val, arr) \
	arr[7] = (unsigned char)((val >> 56) & 0xFF); \
	arr[6] = (unsigned char)((val >> 48) & 0xFF); \
	arr[5] = (unsigned char)((val >> 40) & 0xFF); \
	arr[4] = (unsigned char)((val >> 32) & 0xFF); \
	arr[3] = (unsigned char)((val >> 24) & 0xFF); \
	arr[2] = (unsigned char)((val >> 16) & 0xFF); \
	arr[1] = (unsigned char)((val >> 8) & 0xFF); \
	arr[0] = (unsigned char)(val & 0xFF);

NVM_API int nvm_init();
NVM_API int nvm_run_cli(int argc, char *argv[]);
/**
 * Encode the temperature as a NVM_UINT32
 */
static inline NVM_UINT32 nvm_encode_temperature(NVM_REAL32 value)
{
	NVM_UINT32 result;

	result = (NVM_UINT32)(value * 10000);
	return result;
}

/**
 * Decode a NVM_UINT32 as a temperature
 */
static inline NVM_REAL32 nvm_decode_temperature(NVM_UINT32 value)
{
	NVM_REAL32 result;

	result = (NVM_REAL32)(value / 10000.0);
	return result;
}

/**
 * ****************************************************************************
 * ENUMS
 * ****************************************************************************
 */

/**
 * The operating system type.
 */
enum os_type {
	OS_TYPE_UNKNOWN = 0,    ///< The OS type can not be determined
	OS_TYPE_WINDOWS = 1,    ///< Windows
	OS_TYPE_LINUX	= 2,    ///< Linux
	OS_TYPE_ESX	= 3     // ESX
};

/**
 * Compatibility of the device, FW and configuration with the management software.
 */
enum manageability_state {
	MANAGEMENT_UNKNOWN		= 0,    // Device is not recognized or manageability cannot be determined.
	MANAGEMENT_VALIDCONFIG		= 1,    ///< Device is fully manageable.
	MANAGEMENT_INVALIDCONFIG	= 2,    ///< Device is recognized but cannot be managed.
	MANAGEMENT_NON_FUNCTIONAL	= 3     ///< Device is disabled per NFIT
};

/**
 * Security and Sanitize state of the AEP DIMM.
 */
enum lock_state {
	LOCK_STATE_UNKNOWN		= 0,    ///< Device lock state can not be determined.
	LOCK_STATE_DISABLED		= 1,    ///< Security is not enabled on the device.
	LOCK_STATE_UNLOCKED		= 2,    ///< Security is enabled and unlocked and un-frozen.
	LOCK_STATE_LOCKED		= 3,    ///< Security is enabled and locked and un-frozen.
	LOCK_STATE_FROZEN		= 4,    ///< Security is enabled, unlocked and frozen.
	LOCK_STATE_PASSPHRASE_LIMIT	= 5,    ///< The passphrase limit has been reached, reset required.
	LOCK_STATE_NOT_SUPPORTED	= 6     // Security is not supported
};

/**
 * The device type.
 */
enum memory_type {
	MEMORY_TYPE_UNKNOWN	= 0,    ///< The type of DIMM cannot be determined.
	MEMORY_TYPE_DDR4	= 1,    ///< DDR4.
	MEMORY_TYPE_NVMDIMM	= 2     // NGNVM.
};

/**
 * The device format factor.
 */
enum device_form_factor {
	DEVICE_FORM_FACTOR_UNKNOWN	= 0,    // The form factor cannot be determined.
	DEVICE_FORM_FACTOR_DIMM		= 8,    ///< DIMM.
	DEVICE_FORM_FACTOR_SODIMM	= 12,   ///< SODIMM.
};

/**
 * Overall health summary of the device
 */
enum device_health {
	DEVICE_HEALTH_UNKNOWN		= 0,    ///< The HEALTH can not be determined.
	DEVICE_HEALTH_NORMAL		= 5,    ///< Normal.
	DEVICE_HEALTH_NONCRITICAL	= 15,   ///< Maintenance required.
	DEVICE_HEALTH_CRITICAL		= 25,   ///< Features or performance degraded due to failure.
	DEVICE_HEALTH_FATAL		= 30,   ///< Data loss has occurred or is imminent.
};

/**
 * The address range scrub (ARS) operation status for the AEP DIMM
 */
enum device_ars_status {
	DEVICE_ARS_STATUS_UNKNOWN,
	DEVICE_ARS_STATUS_NOTSTARTED,
	DEVICE_ARS_STATUS_INPROGRESS,
	DEVICE_ARS_STATUS_COMPLETE,
	DEVICE_ARS_STATUS_ABORTED
};

/**
 * The overwrite DIMM operation status for the AEP DIMM
 */
enum device_overwritedimm_status {
	DEVICE_OVERWRITEDIMM_STATUS_UNKNOWN,
	DEVICE_OVERWRITEDIMM_STATUS_NOTSTARTED,
	DEVICE_OVERWRITEDIMM_STATUS_INPROGRESS,
	DEVICE_OVERWRITEDIMM_STATUS_COMPLETE
};

/**
 * The type of sensor.
 * @internal
 * These enums are also used as indexes in the device.sensors array.  It is important to
 * keep them in order and with valid values (0 - 17)
 * @endinternal
 */
enum sensor_type {
  SENSOR_HEALTH = 0,    ///< Apache Pass DIMM health as reported in the SMART log
  SENSOR_MEDIA_TEMPERATURE = 1,    ///< Device media temperature in degrees Celsius.
  SENSOR_CONTROLLER_TEMPERATURE = 2,    ///< Device media temperature in degrees Celsius.
  SENSOR_PERCENTAGE_REMAINING = 3,    ///< Amount of percentage remaining as a percentage.
  SENSOR_WEARLEVEL = 4,    ///< An estimate of the device life used as a percentage.
  SENSOR_DIRTYSHUTDOWNS = 5,    ///< Device shutdowns without notification.
  SENSOR_POWERONTIME = 6,    ///< Total power-on time over the lifetime of the device.
  SENSOR_UPTIME = 7,    ///< Total power-on time since the last power cycle of the device.
  SENSOR_POWERCYCLES = 8,    ///< Number of power cycles over the lifetime of the device.
  SENSOR_FWERRORLOGCOUNT = 9,    ///< The total number of firmware error log entries.
  SENSOR_POWERLIMITED = 10,   ///< Whether or not the AEP DIMM is power limited.
};

typedef NVM_UINT64 NVM_SENSOR_CATEGORY_BITMASK;

/*
 * The bitmask for sensor type.
 */
enum sensor_category {
	SENSOR_CAT_SMART_HEALTH = 0x1,
	SENSOR_CAT_POWER	= 0x2,
	SENSOR_CAT_FW_ERROR	= 0x4,
	SENSOR_CAT_ALL		= SENSOR_CAT_SMART_HEALTH | SENSOR_CAT_POWER | SENSOR_CAT_FW_ERROR
};

/**
 * The units of measurement for a sensor.
 */
enum sensor_units {
	UNIT_COUNT	= 1,    ///< In numbers of something (0,1,2 ... n).
	UNIT_CELSIUS	= 2,    ///< In units of Celsius degrees.
	UNIT_SECONDS	= 21,   ///< In seconds of time.
	UNIT_MINUTES	= 22,   ///< In minutes of time.
	UNIT_HOURS	= 23,   ///< In hours of time.
	UNIT_CYCLES	= 39,   ///< Cycles
	UNIT_PERCENT	= 65    ///< In units of percentage.
};

/**
 * The current status of a sensor
 */
enum sensor_status {
	SENSOR_NOT_INITIALIZED	= -1,   //no attempt to read sensor value yet.
	SENSOR_NORMAL		= 0,    ///< Current value of the sensor is in the normal range.
	SENSOR_NONCRITICAL	= 1,    ///< Current value of the sensor is in non critical range.
	SENSOR_CRITICAL		= 2,    ///< Current value of the sensor is in the critical error range.
	SENSOR_FATAL		= 3,    ///< Current value of the sensor is in the fatal error range.
	SENSOR_UNKNOWN		= 4,    ///< Sensor status cannot be determined.
};

/**
 *      The type of the event that occurred.  Can be used to filter subscriptions.
 */
enum event_type {
	EVENT_TYPE_ALL			= 0,    // Subscribe or filter on all event types
	EVENT_TYPE_CONFIG		= 1,    ///< Device configuration status
	EVENT_TYPE_HEALTH		= 2,    ///< Device health event.
	EVENT_TYPE_MGMT			= 3,    ///< Management software generated event.
	EVENT_TYPE_DIAG			= 4,    ///< Subscribe or filter on all diagnostic event types
	EVENT_TYPE_DIAG_QUICK		= 5,    ///< Quick diagnostic test event.
	EVENT_TYPE_DIAG_PLATFORM_CONFIG = 6,    ///< Platform config diagnostic test event.
	EVENT_TYPE_DIAG_SECURITY	= 7,    ///< Security diagnostic test event.
	EVENT_TYPE_DIAG_FW_CONSISTENCY	= 8     ///< FW consistency diagnostic test event.
};

/**
 * Perceived severity of the event
 */
enum event_severity {
	EVENT_SEVERITY_INFO	= 2,    ///< Informational event.
	EVENT_SEVERITY_WARN	= 3,    ///< Warning or degraded.
	EVENT_SEVERITY_CRITICAL = 6,    ///< Critical.
	EVENT_SEVERITY_FATAL	= 7     // Fatal or nonrecoverable.
};

enum diagnostic_result {
	DIAGNOSTIC_RESULT_UNKNOWN	= 0,
	DIAGNOSTIC_RESULT_OK		= 2,
	DIAGNOSTIC_RESULT_WARNING	= 3,
	DIAGNOSTIC_RESULT_FAILED	= 5,
	DIAGNOSTIC_RESULT_ABORTED	= 6
};

/**
 * Logging level used with the library logging functions.
 */
enum log_level {
	LOG_LEVEL_ERROR = 0,    ///< Error message
	LOG_LEVEL_WARN	= 1,    ///< Warning message
	LOG_LEVEL_INFO	= 2,    ///< Informational message
	LOG_LEVEL_DEBUG = 3     // Debug message
};

/**
 * Logging level used with the firmware logging functions.
 */
enum fw_log_level {
	FW_LOG_LEVEL_DISABLED	= 0,    ///< Logging Disabled
	FW_LOG_LEVEL_ERROR	= 1,    ///< Error message
	FW_LOG_LEVEL_WARN	= 2,    ///< Warning message
	FW_LOG_LEVEL_INFO	= 3,    ///< Informational message
	FW_LOG_LEVEL_DEBUG	= 4,    ///< Debug message
	FW_LOG_LEVEL_UNKNOWN	= 5     // Unknown fw log level setting
};

/**
 * Triggers to modify left shift value
 */
enum triggers_to_modify_shift_value {
	FATAL_ERROR_TRIGGER			= 2,
	SPARE_BLOCK_PERCENTAGE_TRIGGER		= 3,
	DIRTY_SHUTDOWN_TRIGGER			= 4,
};

/**
 * Injected error type - should match the #defines in types.h
 */
enum error_type {
	ERROR_TYPE_POISON		= 1,    ///< Inject a poison error.
	ERROR_TYPE_TEMPERATURE		= 2,    ///< Inject a media temperature error.
	ERROR_TYPE_PACKAGE_SPARING		= 3,    ///< Trigger or revert an artificial package sparing.
	ERROR_TYPE_SPARE_CAPACITY	= 4,    ///< Trigger or clear a percentage remaining threshold alarm.
	ERROR_TYPE_MEDIA_FATAL_ERROR	= 5,    ///< Inject or clear a fake media fatal error.
	ERROR_TYPE_DIRTY_SHUTDOWN	= 6,    ///< Inject or clear a dirty shutdown error.
};

/*
 * Inject a poison error at specific dpa
 */
enum poison_memory_type {
	POISON_MEMORY_TYPE_MEMORYMODE	= 1,    ///< currently allocated in Memory mode
	POISON_MEMORY_TYPE_APPDIRECT	= 2,    ///< currently allocated in AppDirect
	POISON_MEMORY_TYPE_PATROLSCRUB	= 4,    ///< simulating an error found during a patrol scrub operation
	// indifferent to how the memory is currently allocated
};

/**
 * Diagnostic test type
 */
enum diagnostic_test {
	DIAG_TYPE_QUICK			= 0,    ///< verifies manageable DIMM host mailbox is accessible and basic health
	DIAG_TYPE_PLATFORM_CONFIG	= 1,    ///< verifies BIOS config matches installed HW
	DIAG_TYPE_SECURITY		= 2,    ///< verifies all manageable DIMMS have consistent security state
	DIAG_TYPE_FW_CONSISTENCY	= 3     // verifies all DIMMS have consistent FW and attributes
};

/**
 * Diagnostic threshold type.
 */
typedef NVM_UINT64 diagnostic_threshold_type;

#define DIAG_THRESHOLD_QUICK_HEALTH                         (1 << 0)
#define DIAG_THRESHOLD_QUICK_MEDIA_TEMP                     (1 << 1)
#define DIAG_THRESHOLD_QUICK_CONTROLLER_TEMP                (1 << 2)
#define DIAG_THRESHOLD_QUICK_AVAIL_SPARE                    (1 << 3)
#define DIAG_THRESHOLD_QUICK_PERC_USED                      (1 << 4)
#define DIAG_THRESHOLD_QUICK_SPARE_DIE                      (1 << 5)
#define DIAG_THRESHOLD_QUICK_UNCORRECT_ERRORS               (1 << 6)
#define DIAG_THRESHOLD_QUICK_CORRECTED_ERRORS               (1 << 7)
#define DIAG_THRESHOLD_QUICK_ERASURE_CODED_CORRECTED_ERRORS (1 << 8)
#define DIAG_THRESHOLD_QUICK_VALID_VENDOR_ID                (1 << 9)
#define DIAG_THRESHOLD_QUICK_VALID_MANUFACTURER             (1 << 10)
#define DIAG_THRESHOLD_QUICK_VALID_PART_NUMBER              (1 << 11)
#define DIAG_THRESHOLD_QUICK_VIRAL                          (1 << 12)
#define DIAG_THRESHOLD_SECURITY_CONSISTENT                  (1 << 13)
#define DIAG_THRESHOLD_SECURITY_ALL_DISABLED                (1 << 14)
#define DIAG_THRESHOLD_SECURITY_ALL_NOTSUPPORTED            (1 << 15)
#define DIAG_THRESHOLD_FW_CONSISTENT                        (1 << 16)
#define DIAG_THRESHOLD_FW_MEDIA_TEMP                        (1 << 17)
#define DIAG_THRESHOLD_FW_CORE_TEMP                         (1 << 18)
#define DIAG_THRESHOLD_FW_SPARE                             (1 << 19)
#define DIAG_THRESHOLD_FW_POW_MGMT_POLICY                   (1 << 20)
#define DIAG_THRESHOLD_FW_PEAK_POW_BUDGET_MIN               (1 << 21)
#define DIAG_THRESHOLD_FW_PEAK_POW_BUDGET_MAX               (1 << 22)
#define DIAG_THRESHOLD_FW_AVG_POW_BUDGET_MIN                (1 << 23)
#define DIAG_THRESHOLD_FW_AVG_POW_BUDGET_MAX                (1 << 24)
#define DIAG_THRESHOLD_FW_DIE_SPARING_POLICY                (1 << 25)
#define DIAG_THRESHOLD_FW_DIE_SPARING_LEVEL                 (1 << 26)
#define DIAG_THRESHOLD_FW_TIME                              (1 << 27)
#define DIAG_THRESHOLD_FW_DEBUGLOG                          (1 << 28)
#define DIAG_THRESHOLD_PCONFIG_NFIT                         (1 << 29)
#define DIAG_THRESHOLD_PCONFIG_PCAT                         (1 << 30)
#define DIAG_THRESHOLD_PCONFIG_PCD                          (1llu << 31)
#define DIAG_THRESHOLD_PCONFIG_CURRENT_PCD                  (1llu << 32)
#define DIAG_THRESHOLD_PCONFIG_UNCONFIGURED                 (1llu << 33)
#define DIAG_THRESHOLD_PCONFIG_BROKEN_ISET                  (1llu << 34)
#define DIAG_THRESHOLD_PCONFIG_MAPPED_CAPACITY              (1llu << 35)
#define DIAG_THRESHOLD_PCONFIG_BEST_PRACTICES               (1llu << 36)

// The volatile memory mode currently selected by the BIOS.
enum volatile_mode {
	VOLATILE_MODE_1LM	= 0,    ///< 1LM Mode
	VOLATILE_MODE_MEMORY	= 1,    ///< Memory Mode
	VOLATILE_MODE_AUTO	= 2,    ///< Memory Mode if DDR4 + AEP DIMM present, 1LM otherwise
	VOLATILE_MODE_UNKNOWN	= 3,    ///< The current volatile memory mode cannot be determined.
};

// The App Direct mode currently selected by the BIOS.
enum app_direct_mode {
	APP_DIRECT_MODE_DISABLED	= 0,    ///< App Direct mode disabled.
	APP_DIRECT_MODE_ENABLED		= 1,    ///< App Direct mode enabled.
	APP_DIRECT_MODE_UNKNOWN		= 2,    ///< The current App Direct mode cannot be determined.
};

// Interface format code as reported by NFIT
enum nvm_format {
	FORMAT_NONE		= 0,
	FORMAT_BLOCK_STANDARD	= 0x201,
	FORMAT_BYTE_STANDARD	= 0x301
};

/**
 * Status of last AEP DIMM shutdown
 */
enum shutdown_status {
  SHUTDOWN_STATUS_UNKNOWN = 0,                ///< The last shutdown status cannot be determined.
  SHUTDOWN_STATUS_PM_ADR = 1 << 0,            ///< Async DIMM Refresh command received
  SHUTDOWN_STATUS_PM_S3 = 1 << 1,             ///< PM S3 received
  SHUTDOWN_STATUS_PM_S5 = 1 << 2,             ///< PM S5 received
  SHUTDOWN_STATUS_DDRT_POWER_FAIL = 1 << 3,   ///< DDRT power fail command received
  SHUTDOWN_STATUS_PMIC_POWER_LOSS = 1 << 4,   ///< PMIC Power Loss received
  SHUTDOWN_STATUS_WARM_RESET = 1 << 5,        ///< PM warm reset received
  SHUTDOWN_STATUS_FORCED_THERMAL = 1 << 6,    ///< Thermal shutdown received
  SHUTDOWN_STATUS_CLEAN = 1 << 7              ///< Denotes a proper clean shutdown
};

enum shutdown_status_extended {
	SHUTDOWN_STATUS_VIRAL_INT_RCVD			= 1 << 0,
	SHUTDOWN_STATUS_SURPRISE_CLK_STOP_INT_RCVD	= 1 << 1,
	SHUTDOWN_STATUS_WR_DATA_FLUSH_RCVD		= 1 << 2,
	SHUTDOWN_STATUS_S4_PWR_STATE_RCVD		= 1 << 3,
};

/**
 * Status of the device current configuration
 */
enum config_status {
	CONFIG_STATUS_NOT_CONFIGURED		= 0,    ///< The device is not configured.
	CONFIG_STATUS_VALID			= 1,    ///< The device has a valid configuration.
	CONFIG_STATUS_ERR_CORRUPT		= 2,    ///< The device configuration is corrupt.
	CONFIG_STATUS_ERR_BROKEN_INTERLEAVE	= 3,    ///< The interleave set is broken.
	CONFIG_STATUS_ERR_REVERTED		= 4,    ///< The configuration failed and was reverted.
	CONFIG_STATUS_ERR_NOT_SUPPORTED		= 5,    ///< The configuration is not supported by the BIOS.
	CONFIG_STATUS_UNKNOWN			= 6,    ///< The configuration status cannot be determined
};

/**
 * Status of current configuration goal
 */
enum config_goal_status {
	CONFIG_GOAL_STATUS_NO_GOAL_OR_SUCCESS		= 0,    ///< The configuration goal status cannot be determined.
	CONFIG_GOAL_STATUS_UNKNOWN			= 1,    ///< The configuration goal has not yet been applied.
	CONFIG_GOAL_STATUS_NEW				= 2,    ///< The configuration goal was applied successfully.
	CONFIG_GOAL_STATUS_ERR_BADREQUEST		= 3,    ///< The configuration goal was invalid.
	CONFIG_GOAL_STATUS_ERR_INSUFFICIENTRESOURCES	= 4,    ///< Not enough resources to apply the goal.
	CONFIG_GOAL_STATUS_ERR_FW			= 5,    ///< Failed to apply the goal due to a FW error.
	CONFIG_GOAL_STATUS_ERR_UNKNOWN			= 6,    ///< Failed to apply the goal for an unknown reason.
};

/**
 *  * Status of NVM jobs
 */
enum nvm_job_status {
	NVM_JOB_STATUS_UNKNOWN		= 0,
	NVM_JOB_STATUS_NOT_STARTED	= 1,
	NVM_JOB_STATUS_RUNNING		= 2,
	NVM_JOB_STATUS_COMPLETE		= 3
};

/**
 * Type of job
 */
enum nvm_job_type {
	NVM_JOB_TYPE_SANITIZE	= 0,
	NVM_JOB_TYPE_ARS	= 1
};

/**
 * firmware type
 */
enum device_fw_type {
	DEVICE_FW_TYPE_UNKNOWN		= 0, ///< fw image type cannot be determined
	DEVICE_FW_TYPE_PRODUCTION	= 1,
	DEVICE_FW_TYPE_DFX		= 2,
	DEVICE_FW_TYPE_DEBUG		= 3
};

/**
 * status of last firmware update operation
 */
enum fw_update_status {
	FW_UPDATE_UNKNOWN	= 0, ///< status of the last FW update cannot be retrieved
	FW_UPDATE_STAGED	= 1,
	FW_UPDATE_SUCCESS	= 2,
	FW_UPDATE_FAILED	= 3
};

/**
 * ****************************************************************************
 * STRUCTURES
 * ****************************************************************************
 */

/**
 * The host server that the native API library is running on.
 */
struct host {
	char		name[NVM_COMPUTERNAME_LEN];     ///<The host computer name.
	enum os_type	os_type;                        ///<OS type.
	char		os_name[NVM_OSNAME_LEN];        ///< OS name string.
	char		os_version[NVM_OSVERSION_LEN];  ///< OS version string.
	NVM_BOOL	mixed_sku;                      ///< One or more AEP DIMMs have different SKUs.
	NVM_BOOL	sku_violation;                  ///< Configuration of AEP DIMMs are unsupported due to a license issue.
};

/**
 * Software versions (one per server).
 */
struct sw_inventory {
	NVM_VERSION	mgmt_sw_revision;               ///< Host software version.
	NVM_VERSION	vendor_driver_revision;         ///< Vendor specific NVDIMM driver version.
	NVM_BOOL	vendor_driver_compatible;       ///< Is vendor driver compatible with MGMT SW?
};

/**
 * Structure that describes a memory device in the system.
 * This data is harvested from the SMBIOS table Type 17 structures.
 */
struct memory_topology {
	NVM_UINT16		physical_id;                            ///< Memory device's physical identifier (SMBIOS handle)
	enum memory_type	memory_type;                            ///< Type of memory device
	enum device_form_factor form_factor;                            ///< DEPRECATED; Form factor of the memory device
	NVM_UINT64		raw_capacity;                           ///< DEPRECATED; Raw capacity of the device in bytes
	NVM_UINT64		data_width;                             ///< DEPRECATED; Width in bits used to store user data
	NVM_UINT64		total_width;                            ///< DEPRECATED; Width in bits for data and error correction/data redundancy
	NVM_UINT64		speed;                                  ///< DEPRECATED; Speed in MHz
	char			part_number[NVM_PART_NUM_LEN];          ///< DEPRECATED; Part number assigned by the vendor
	char			device_locator[NVM_DEVICE_LOCATOR_LEN]; ///< Physically-labeled socket of device location
	char			bank_label[NVM_BANK_LABEL_LEN];         ///< Physically-labeled bank of device location
};

/**
 * Structure that describes the security capabilities of a device
 */
struct device_security_capabilities {
	NVM_BOOL	passphrase_capable;     ///< AEP DIMM supports the nvm_(set|remove)_passphrase command
	NVM_BOOL	unlock_device_capable;  ///< AEP DIMM supports the nvm_unlock_device command
	NVM_BOOL	erase_crypto_capable;   ///< AEP DIMM supports nvm_erase command with the CRYPTO
};

/**
 * Structure that describes the capabilities supported by an AEP DIMM
 */
struct device_capabilities {
	NVM_BOOL	package_sparing_capable;        ///< AEP DIMM supports package sparing
	NVM_BOOL	memory_mode_capable;            ///< AEP DIMM supports memory mode
	NVM_BOOL	storage_mode_capable;           ///< AEP DIMM supports storage mode
	NVM_BOOL	app_direct_mode_capable;        ///< AEP DIMM supports app direct mode
};

/**
 * The device_discovery structure describes an enterprise-level view of a device with
 * enough information to allow callers to uniquely identify a device and determine its status.
 * The UID in this structure is used for all other device management calls to uniquely
 * identify a device.  It is intended that this structure will not change over time to
 * allow the native API library to communicate with older and newer revisions of devices.
 * @internal
 * Keep this structure to data from the Identify DIMM command and calculated data.
 * @endinternal
 */
struct device_discovery {
	// Properties that are fast to access
	///////////////////////////////////////////////////////////////////////////
	// Indicate whether the struct was populated with the full set of
	// properties (nvm_get_devices()) or just a minimal set (NFIT + SMBIOS)
	// The calls originate at populate_devices() and use the
	// parameter populate_all_properties to distinguish each
	NVM_BOOL		all_properties_populated;

	// ACPI
	NVM_NFIT_DEVICE_HANDLE	device_handle;          ///< The unique device handle of the memory module
	NVM_UINT16		physical_id;            ///< The unique physical ID of the memory module
	NVM_UINT16		vendor_id;              // The vendor identifier.
	NVM_UINT16		device_id;              ///< The device identifier.
	NVM_UINT16		revision_id;            ///< The revision identifier.
	NVM_UINT16		channel_pos;            ///< The memory module's position in the memory channel
	NVM_UINT16		channel_id;             ///< The memory channel number
	NVM_UINT16		memory_controller_id;   ///< The ID of the associated memory controller
	NVM_UINT16		socket_id;              ///< The processor socket identifier.
	NVM_UINT16		node_controller_id;     ///< The node controller ID.

	// SMBIOS
	enum memory_type	memory_type; ///<	The type of memory used by the DIMM.

	///////////////////////////////////////////////////////////////////////////



	// Slow (>15ms per passthrough ioctl) properties stored on each DIMM
	///////////////////////////////////////////////////////////////////////////
	// Identify Intel DIMM Gen 1
	// add_identify_dimm_properties_to_device() in device.c
	NVM_UINT32				dimm_sku;
	NVM_MANUFACTURER			manufacturer;                   ///< The manufacturer ID code determined by JEDEC JEP-106
	NVM_SERIAL_NUMBER			serial_number;                  // Serial number assigned by the vendor.
	NVM_UINT16				subsystem_vendor_id;            // vendor identifier of the AEP DIMM non-volatile
	// memory subsystem controller
	NVM_UINT16				subsystem_device_id;            // device identifier of the AEP DIMM non-volatile
	// memory subsystem controller
	NVM_UINT16				subsystem_revision_id;          // revision identifier of the AEP DIMM non-volatile
	// memory subsystem controller
	NVM_BOOL				manufacturing_info_valid;       // manufacturing location and date validity
	NVM_UINT8				manufacturing_location;         // AEP DIMM manufacturing location assigned by vendor
	// only valid if manufacturing_info_valid=1
	NVM_UINT16				manufacturing_date;             // Date the AEP DIMM was manufactured, assigned by vendor
	// only valid if manufacturing_info_valid=1
	char					part_number[NVM_PART_NUM_LEN];  // The manufacturer's model part number
	NVM_VERSION				fw_revision;                    // The current active firmware revision.
	NVM_VERSION				fw_api_version;                 // API version of the currently running FW
	NVM_UINT64				capacity;                       // Raw capacity in bytes.
	NVM_UINT16				interface_format_codes[NVM_MAX_IFCS_PER_DIMM];
	// calculate_capabilities_for_populated_devices() in device.c
	struct device_security_capabilities	security_capabilities;
	struct device_capabilities		device_capabilities; // Capabilities supported by the device

	// Calculated by MGMT from NFIT table properties
	NVM_UID					uid; // Unique identifier of the device.


	// Get Security State
	// add_security_state_to_device() in device.c
	enum lock_state				lock_state; // Indicates if the DIMM is in a locked security state
	///////////////////////////////////////////////////////////////////////////

	// Whether the dimm is manageable or not is derived based on what calls are
	// made to populate this struct. If partial properties are requested, then
	// only those properties are used to derive this value. If all properties are
	// requested, then the partial properties plus the firmware API version
	// (requires a DSM call) are used to set this value.
	enum manageability_state manageability;
};

struct fw_error_log_sequence_numbers {
	NVM_UINT16	oldest;
	NVM_UINT16	current;
};

struct device_error_log_status {
	struct fw_error_log_sequence_numbers	therm_low;
	struct fw_error_log_sequence_numbers	therm_high;
	struct fw_error_log_sequence_numbers	media_low;
	struct fw_error_log_sequence_numbers	media_high;
};

/**
 * The status of a particular device
 */

struct device_status {
	enum device_health		health;                                 ///< Overall device health.
	NVM_BOOL			is_new;                                 ///< Unincorporated with the rest of the devices.
	NVM_BOOL			is_configured;                          ///< only the values 1(Success) and 6 (old config used) from CCUR are considered configured
	NVM_BOOL			is_missing;                             ///< If the device is missing.
	NVM_UINT8			package_spares_available;               ///< Number of package spares on the AEP DIMM that are available.
	NVM_UINT8			last_shutdown_status;                   ///< State of last AEP DIMM shutdown.
	NVM_UINT8			last_shutdown_status_extended[3];       ///< DEPRECATED; Extendeded fields as per FIS 1.6
	enum config_status		config_status;                          ///< Status of last configuration request.
	NVM_UINT64			last_shutdown_time;                     ///< Time of the last shutdown - seconds since 1 January 1970
	NVM_BOOL			mixed_sku;                              ///< DEPRECATED; One or more AEP DIMMs have different SKUs.
	NVM_BOOL			sku_violation;                          ///< The AEP DIMM configuration is unsupported due to a license issue.
	NVM_BOOL			viral_state;                            ///< Current viral status of AEP DIMM.
	enum device_ars_status		ars_status;                             ///< Address range scrub operation status for the AEP DIMM
	enum device_overwritedimm_status	overwritedimm_status;         ///< Overwrite DIMM operation status for the AEP DIMM
	NVM_UINT32			new_error_count;                        ///< DEPRECATED; Count of new fw errors from the AEP DIMM
	NVM_UINT64			newest_error_log_timestamp;             ///< DEPRECATED Timestamp of the newest log entry in the fw error log
	NVM_BOOL			ait_dram_enabled;                       ///< Whether or not the AIT DRAM is enabled.
	NVM_UINT64			boot_status;                            ///< The status of the AEP DIMM as reported by the firmware in the BSR
	NVM_UINT32			injected_media_errors;                  ///< The number of injected media errors on AEP DIMM
	NVM_UINT32			injected_non_media_errors;              ///< The number of injected non-media errors on AEP DIMM
	struct device_error_log_status	error_log_status;                       // DEPRECATED;
};

/**
 * A snapshot of the performance metrics for a specific device.
 * @remarks All data is cumulative over the life the device.
 */
struct device_performance {
	time_t		time; ///< The time the performance snapshot was gathered.
	// These next fields are 16 bytes in the fw spec, but it would take 100 years
	// of over 31 million reads/writes per second to reach the limit, so we
	// are just using 8 bytes here.
	NVM_UINT64	bytes_read;     ///< Total bytes of data read from the DIMM.
	NVM_UINT64	host_reads;     ///< Lifetime number of read requests the DIMM serviced.
	NVM_UINT64	bytes_written;  ///< Total bytes of data written to the DIMM.
	NVM_UINT64	host_writes;    ///< Lifetime number of write requests the DIMM serviced.
	NVM_UINT64	block_reads;    ///< Lifetime number of BW read requests the DIMM has services.
	NVM_UINT64	block_writes;   ///< Lifetime number of BW write requests the DIMM has services.
};

/**
 * The threshold settings for a particular sensor
 */
struct sensor_settings {
	NVM_BOOL	enabled;                        ///< If firmware notifications are enabled when sensor value is critical.
	NVM_UINT64	upper_critical_threshold;       ///< The upper critical threshold.
	NVM_UINT64	lower_critical_threshold;       ///< The lower critical threshold.
	NVM_UINT64	upper_fatal_threshold;          ///< The upper fatal threshold.
	NVM_UINT64	lower_fatal_threshold;          ///< The lower fatal threshold.
	NVM_UINT64	upper_noncritical_threshold;    ///< The upper noncritical threshold.
	NVM_UINT64	lower_noncritical_threshold;    ///< The lower noncritical threshold.
};

/**
 * The current state and settings of a particular sensor
 */
struct sensor {
	enum sensor_type	type;                           ///< The type of sensor.
	enum sensor_units	units;                          ///< The units of measurement for the sensor.
	enum sensor_status	current_state;                  ///< The current state of the sensor.
	NVM_UINT64		reading;                        ///< The current value of the sensor.
	struct sensor_settings	settings;                       ///< The settings for the sensor.
	NVM_BOOL		lower_critical_settable;        ///< If the lower_critical_threshold value is modifiable.
	NVM_BOOL		upper_critical_settable;        ///< If the upper_critical_threshold value is modifiable.
	NVM_BOOL		lower_critical_support;         ///< If the lower_critical_threshold value is supported.
	NVM_BOOL		upper_critical_support;         ///< If the upper_critical_threshold value is supported.
	NVM_BOOL		lower_fatal_settable;           ///< If the lower_fatal_threshold value is modifiable.
	NVM_BOOL		upper_fatal_settable;           ///< If the upper_fatal_threshold value is modifiable.
	NVM_BOOL		lower_fatal_support;            ///< If the lower_fatal_threshold value is supported.
	NVM_BOOL		upper_fatal_support;            ///< If the upper_fatal_threshold value is supported.
	NVM_BOOL		lower_noncritical_settable;     ///< If the lower_noncritical_threshold value is modifiable.
	NVM_BOOL		upper_noncritical_settable;     ///< If the upper_noncritical_threshold value is modifiable.
	NVM_BOOL		lower_noncritical_support;      ///< If the lower_noncritical_threshold value is supported.
	NVM_BOOL		upper_noncritical_support;      ///< If the upper_noncritical_threshold value is supported.
};

/**
 * Device partition capacities (in bytes) used for a single device or aggregated across the server.
 */
struct device_capacities {
	NVM_UINT64	capacity;                       ///< The total AEP DIMM capacity in bytes.
	NVM_UINT64	memory_capacity;                ///< The total AEP DIMM capacity in bytes for memory mode.
	NVM_UINT64	app_direct_capacity;            ///< The total AEP DIMM capacity in bytes for app direct mode.
	NVM_UINT64	mirrored_app_direct_capacity;   ///< The total AEP DIMM mirrored app direct capacity.
	NVM_UINT64	storage_capacity;               ///< AEP DIMM capacity allocated that can be used as storage.
	NVM_UINT64	unconfigured_capacity;          ///< Unconfigured AEP DIMM capacity. Can be used as storage.
	NVM_UINT64	inaccessible_capacity;          ///< AEP DIMM capacity not licensed for this AEP DIMM SKU.
	NVM_UINT64	reserved_capacity;              ///< AEP DIMM capacity reserved for metadata.
};

/**
 * Modifiable settings of a device.
 */
struct device_settings {
	NVM_BOOL	first_fast_refresh;     ///< Enable/disable acceleration of first refresh cycle.
	NVM_BOOL	viral_policy;           ///< Enable/disable viral policies.
};

/**
 * Detailed information about firmware image log information of a device.
 */
struct device_fw_info {
	/*
	 * BCD-formatted revision of the active firmware in the format MM.mm.hh.bbbb
	 * MM = 2-digit major version
	 * mm = 2-digit minor version
	 * hh = 2-digit hot fix version
	 * bbbb = 4-digit build version
	 */
	NVM_VERSION		active_fw_revision;
	enum device_fw_type	active_fw_type;                         ///< active FW type.
	char			active_fw_commit_id[NVM_COMMIT_ID_LEN]; ///< commit id of active FW for debug/troubleshooting

	// build configuration of active FW for debug/troubleshooting
	char			active_fw_build_configuration[NVM_BUILD_CONFIGURATION_LEN];

	NVM_VERSION		staged_fw_revision;     ///<  BCD formatted revision of the staged FW.
	enum fw_update_status	fw_update_status;       ///< status of last FW update operation.
};

/**
 * Detailed information about a device.
 */
struct device_details {
	struct device_discovery		discovery;                              // Basic device identifying information.
	struct device_status		status;                                 ///< Device health and status.
	struct device_fw_info		fw_info;                                ///< The firmware image information for the Apache Pass DIMM.
	NVM_UINT8			padding[2];                             ///< struct alignment
	struct device_performance	performance;                            ///< A snapshot of the performance metrics.
	struct sensor			sensors[NVM_MAX_DEVICE_SENSORS];        ///< Device sensors.
	struct device_capacities	capacities;                             ///< Partition information

	// from SMBIOS Type 17 Table
	enum device_form_factor		form_factor;                            ///< The type of DIMM.
	NVM_UINT64			data_width;                             // The width in bits used to store user data.
	NVM_UINT64			total_width;                            // The width in bits for data and ECC and/or redundancy.
	NVM_UINT64			speed;                                  ///< The speed in nanoseconds.
	char				device_locator[NVM_DEVICE_LOCATOR_LEN]; ///< The socket or board position label
	char				bank_label[NVM_BANK_LABEL_LEN];         ///< The bank label

	NVM_BOOL			power_management_enabled;               ///< Enable or disable power management.
	NVM_UINT16			peak_power_budget;                      ///< instantaneous power budget in mW (100-20000 mW).
	NVM_UINT16			avg_power_budget;                       ///< average power budget in mW (100-18000 mW).
	NVM_BOOL			package_sparing_enabled;                    ///< Enable or disable package sparing.
	NVM_UINT8			package_sparing_level;                      ///< How aggressive to be in package sparing (0-255).
	struct device_settings		settings;                               ///< Modifiable features of the device.
};

/**
 * Supported capabilities of a specific memory mode
 */
struct memory_capabilities {
	NVM_BOOL			supported;                                      ///< is the memory mode supported by the BIOS
	NVM_UINT16			interleave_alignment_size;                      ///< interleave alignment size in 2^n bytes.
	NVM_UINT16			interleave_formats_count;                       ///< Number of interleave formats supported by BIOS
	struct interleave_format	interleave_formats[NVM_INTERLEAVE_FORMATS];     ///< interleave formats
};

/**
 * Supported features and capabilities BIOS supports
 */
struct platform_capabilities {
	NVM_BOOL			bios_config_support;            ///< available BIOS support for CR config changes
	NVM_BOOL			bios_runtime_support;           ///< runtime interface used to validate management configuration
	NVM_BOOL			memory_mirror_supported;        ///< indicates if DIMM mirror is supported
	NVM_BOOL			storage_mode_supported;         ///< is storage mode supported
	NVM_BOOL			memory_spare_supported;         ///< pm spare is supported
	NVM_BOOL			memory_migration_supported;     ///< pm memory migration is supported
	struct memory_capabilities	one_lm_mode;                    ///< capabilities for 1LM mode
	struct memory_capabilities	memory_mode;                    ///< capabilities for Memory mode
	struct memory_capabilities	app_direct_mode;                ///< capabilities for App Direct mode
	enum volatile_mode		current_volatile_mode;          ///< The volatile memory mode selected by the BIOS.
	enum app_direct_mode		current_app_direct_mode;        ///< The App Direct mode selected by the BIOS.
};

/**
 * AEP DIMM software-supported features
 */
struct nvm_features {
	NVM_BOOL	get_platform_capabilities;      ///< get platform supported capabilities
	NVM_BOOL	get_devices;                    ///< retrieve the list of AEP DIMMs installed on the server
	NVM_BOOL	get_device_smbios;              ///< retrieve the SMBIOS information for AEP DIMMs
	NVM_BOOL	get_device_health;              ///< retrieve the health status for AEP DIMMs
	NVM_BOOL	get_device_settings;            ///< retrieve AEP DIMM settings
	NVM_BOOL	modify_device_settings;         ///< modify AEP DIMM settings
	NVM_BOOL	get_device_security;            ///< retrieve AEP DIMM security state
	NVM_BOOL	modify_device_security;         ///< modify AEP DIMM security settings
	NVM_BOOL	get_device_performance;         ///< retrieve AEP DIMM performance metrics
	NVM_BOOL	get_device_firmware;            ///< retrieve AEP DIMM firmware version
	NVM_BOOL	update_device_firmware;         ///< update the firmware version on AEP DIMMs
	NVM_BOOL	get_sensors;                    ///< get health sensors on AEP DIMMS
	NVM_BOOL	modify_sensors;                 ///< modify the AEP DIMM health sensor settings
	NVM_BOOL	get_device_capacity;            ///< retrieve how AEP DIMM capacity is mapped by BIOS
	NVM_BOOL	modify_device_capacity;         ///< modify how the AEP DIMM capacity is provisioned
	NVM_BOOL	get_regions;                      ///< retrieve regions of AEP DIMM capacity
	NVM_BOOL	get_namespaces;                 ///< retrieve the list of namespaces allocated from regions
	NVM_BOOL	get_namespace_details;          ///< retrieve detailed info about each namespace
	NVM_BOOL	create_namespace;               ///< create a new namespace
	NVM_BOOL	rename_namespace;               ///< rename an existing namespace
	NVM_BOOL	grow_namespace;                 ///< increase the capacity of a namespace
	NVM_BOOL	shrink_namespace;               ///< decrease the capacity of a namespace
	NVM_BOOL	enable_namespace;               ///< enable a namespace
	NVM_BOOL	disable_namespace;              ///< disable a namespace
	NVM_BOOL	delete_namespace;               ///< delete a namespace
	NVM_BOOL	get_address_scrub_data;         ///< retrieve address range scrub data
	NVM_BOOL	start_address_scrub;            ///< initiate an address range scrub
	NVM_BOOL	quick_diagnostic;               ///< quick health diagnostic
	NVM_BOOL	platform_config_diagnostic;     ///< platform configuration diagnostic
	NVM_BOOL	pm_metadata_diagnostic;         ///< persistent memory metadata diagnostic
	NVM_BOOL	security_diagnostic;            ///< security diagnostic
	NVM_BOOL	fw_consistency_diagnostic;      ///< firmware consistency diagnostic
	NVM_BOOL	memory_mode;                    ///< access AEP DIMM capacity as memory
	NVM_BOOL	app_direct_mode;                ///< access AEP DIMM persistent memory in App Direct Mode
	NVM_BOOL	storage_mode;                   ///< access AEP DIMM persistent memory in Storage Mode
	NVM_BOOL	error_injection;                ///< error injection on AEP DIMMs
};

/**
 * Supported features and capabilities the driver/software supports
 */
struct sw_capabilities {
	NVM_UINT64	min_namespace_size; ///< smallest namespace supported by the driver, in bytes
	NVM_BOOL	namespace_memory_page_allocation_capable;
};

/**
 * Aggregation of AEP DIMM SKU capabilities across all manageable AEP DIMMs in the system.
 */
struct dimm_sku_capabilities {
	NVM_BOOL	mixed_sku;      ///< One or more AEP DIMMs have different SKUs.
	NVM_BOOL	sku_violation;  ///< One or more AEP DIMMs are in violation of their SKU.
	NVM_BOOL	memory_sku;     ///< One or more AEP DIMMs support memory mode.
	NVM_BOOL	app_direct_sku; ///< One or more AEP DIMMs support app direct mode.
	NVM_BOOL	storage_sku;    ///< One or more AEP DIMMs support storage mode.
};

/**
 * Combined AEP DIMM capabilities
 */
struct nvm_capabilities {
	struct nvm_features		nvm_features;           ///< supported features of the AEP DIMM software
	struct sw_capabilities		sw_capabilities;        ///< driver supported capabilities
	struct platform_capabilities	platform_capabilities;  ///< platform-supported capabilities
	struct dimm_sku_capabilities	sku_capabilities;       ///< aggregated AEP DIMM SKU capabilities
};

/*
 * Interleave set information
 */
struct interleave_set {
	NVM_UINT32			set_index;      ///< unique identifier from the PCD
	NVM_UINT32			driver_id;      ///< unique identifier from the driver
	NVM_UINT64			size;           ///< size in bytes
	NVM_UINT64			available_size; ///< free size in bytes
	struct interleave_format	settings;
	NVM_UINT8			socket_id;
	NVM_UINT8			dimm_count;
	NVM_UID				dimms[NVM_MAX_DEVICES_PER_SOCKET];
	NVM_BOOL			mirrored;
	enum interleave_set_health	health;
	enum encryption_status		encryption;     ///< on if lockstates of all dimms is enabled
	NVM_BOOL			erase_capable;  ///< true if all dimms in the set support erase
};

/**
 * Information about a region of AEP DIMM capacity
 */
struct region {
	NVM_UID			region_uid;       ///< Unique identifier of the region.
	enum region_type		type;           ///< The type of region.
	NVM_UINT64		capacity;       ///< Size of the region in bytes.
	NVM_UINT64		free_capacity;  ///< Available size of the region in bytes.
	// The processor socket identifier.
	NVM_INT16		socket_id;
	NVM_UINT16		dimm_count;     ///< The number of dimms in this region.
	NVM_UINT16		ilset_count;    ///< The number of interleave sets in this region.
	// Raw capacity of each dimm in the region in bytes.
	NVM_UINT64		raw_capacities[NVM_MAX_DEVICES_PER_POOL];
	// Memory mode capacity of each dimm in the region in bytes.
	NVM_UINT64		memory_capacities[NVM_MAX_DEVICES_PER_POOL];
	NVM_UID			dimms[NVM_MAX_DEVICES_PER_POOL]; ///< Unique ID's of underlying AEP DIMMs.
	// The interleave sets in this region
	struct interleave_set	ilsets[NVM_MAX_DEVICES_PER_POOL * 2];
	enum region_health	health; ///< Rolled up health of the underlying AEP DIMMs.
	// possible to create a namespace wholly contained on AEP DIMMs that have encryption enabled.
	NVM_BOOL		encryption_enabled;
	// possible to create a namespace wholly contained on AEP DIMMs that support encryption.
	NVM_BOOL		encryption_capable;
	// true if its possible to create namespace that would be wholly contained on erasable dimms
	NVM_BOOL		erase_capable;
};

/**
 * Describes the configuration goal for a particular AEP DIMM.
 */
struct config_goal_input {
	NVM_UINT8	persistent_mem_type;
	NVM_UINT32	volatile_percent;
	NVM_UINT32	reserved_percent;
	NVM_UINT32	reserve_dimm;
	NVM_UINT16	namespace_label_major;
	NVM_UINT16	namespace_label_minor;
};

struct config_goal {
	NVM_UID			dimm_uid;
	NVM_UINT16		socket_id;
	NVM_UINT32		persistent_regions;
	NVM_UINT64		volatile_size; // Gibibytes of memory mode capacity on the DIMM.
	NVM_UINT64		storage_capacity;
	enum interleave_type	interleave_set_type[MAX_IS_PER_DIMM];
	NVM_UINT64		appdirect_size[MAX_IS_PER_DIMM];
	enum interleave_size	imc_interleaving[MAX_IS_PER_DIMM];
	enum interleave_size	channel_interleaving[MAX_IS_PER_DIMM];
	NVM_UINT8		appdirect_index[MAX_IS_PER_DIMM];
	enum config_goal_status status; // Status for the config goal. Ignored for input.
};

/**
 * Basic discovery information about a namespace.
 */
struct namespace_discovery {
	NVM_UID namespace_uid;                          ///< Unique identifier of the namespace.
	char	friendly_name[NVM_NAMESPACE_NAME_LEN];  ///< User supplied friendly name.
};

/**
 * Structure that describes the security features of a namespace
 */
struct namespace_security_features {
	// encryption status of the NVDIMM or interleave set
	enum encryption_status		encryption;
	// true if the parent NVDIMM or interleave set is erase capable
	enum erase_capable_status	erase_capable;
};

/**
 * Detailed information about a namespace.
 */
struct namespace_details {
	struct namespace_discovery		discovery;              ///< Basic discovery information.
	NVM_UID					region_uid;               ///< The region the namespace is created from.
	NVM_UINT32				block_size;             ///< Block size in bytes.
	NVM_UINT64				block_count;            ///< Number of blocks.
	enum namespace_type			type;                   ///< The type of namespace
	enum namespace_health			health;                 ///< Rolled-up health of the underlying AEP DIMMs.
	enum namespace_enable_state		enabled;                ///< If namespace is exposed to the OS.
	NVM_BOOL				btt;                    ///< optimized for speed
	struct namespace_security_features	security_features;      ///< Security features
	struct interleave_format		interleave_format;
	NVM_BOOL				mirrored;
	union {
		NVM_UID		device_uid;             ///< Used when creating a storage Namespace
		NVM_UINT32	interleave_setid;       ///< Used when creating an app direct Namespace
	} creation_id;                                  ///< the identifier used by the driver when creating a Namespace
	enum namespace_memory_page_allocation	memory_page_allocation;
};

/**
 * Caller specified settings for creating a new namespace.
 */
struct namespace_create_settings {
	char					friendly_name[NVM_NAMESPACE_NAME_LEN];  ///< User supplied friendly name.
	NVM_UINT16				block_size;                             ///< Block size in bytes.
	NVM_UINT64				block_count;                            ///< The number of blocks.
	enum namespace_type			type;                                   ///< The type of namespace.
	enum namespace_enable_state		enabled;                                ///< If the namespace is exposed to OS after creation.
	NVM_BOOL				btt;                                    ///< optimized for speed
	struct namespace_security_features	security_features;                      ///< Security features
	enum namespace_memory_page_allocation	memory_page_allocation;
};

/**
 * Namespace size ranges. All sizes are in bytes.
 */
struct possible_namespace_ranges {
	NVM_UINT64	largest_possible_app_direct_ns;         ///< largest app direct namespace size possible
	NVM_UINT64	smallest_possible_app_direct_ns;        ///< smallest app direct namespace size possible
	NVM_UINT64	app_direct_increment;                   ///< Valid increment between smallest & largest app direct size
};

/*
 * The details of a specific device event that can be subscribed to
 * using #nvm_add_event_notify.
 */
struct event {
	NVM_UINT32		event_id;                       ///< Unique ID of the event.
	enum event_type		type;                           ///< The type of the event that occurred.
	enum event_severity	severity;                       ///< The severity of the event.
	NVM_UINT16		code;                           ///< A numerical code for the specific event that occurred.
	NVM_BOOL		action_required;                ///< A flag indicating that the event needs a corrective action.
	NVM_UID			uid;                            ///< The unique ID of the item that had the event.
	time_t			time;                           ///< The time the event occurred.
	NVM_EVENT_MSG		message;                        ///< A detailed description of the event type that occurred in English.
	NVM_EVENT_ARG		args[NVM_MAX_EVENT_ARGS];       ///< The message arguments.
	enum diagnostic_result	diag_result;                    ///< The diagnostic completion state (only for diag events).
};

/**
 * Limits the events returned by the #nvm_get_events method to
 * those that meet the conditions specified.
 */
struct event_filter {
	/*
	 * A bit mask specifying the values in this structure used to limit the results.
	 * Any combination of the following or 0 to return all events.
	 * NVM_FILTER_ON_TYPE
	 * NVM_FITLER_ON_SEVERITY
	 * NVM_FILTER_ON_CODE
	 * NVM_FILTER_ON_UID
	 * NVM_FILTER_ON_AFTER
	 * NVM_FILTER_ON_BEFORE
	 * NVM_FILTER_ON_EVENT
	 * NVM_FILTER_ON_AR
	 */
	NVM_UINT8		filter_mask;

	/*
	 * The type of events to retrieve. Only used if
	 * NVM_FILTER_ON_TYPE is set in the #filter_mask.
	 */
	enum event_type		type;

	/*
	 * The type of events to retrieve. Only used if
	 * NVM_FILTER_ON_SEVERITY is set in the #filter_mask.
	 */
	enum event_severity	severity;

	/*
	 * The specific event code to retrieve. Only used if
	 * NVM_FILTER_ON_CODE is set in the #filter_mask.
	 */
	NVM_UINT16		code;

	/*
	 * The identifier to retrieve events for.
	 * Only used if NVM_FILTER_ON_UID is set in the #filter_mask.
	 */
	NVM_UID			uid; ///< filter on specific item

	/*
	 * The time after which to retrieve events.
	 * Only used if NVM_FILTER_ON_AFTER is set in the #filter_mask.
	 */
	time_t			after; ///< filter on events after specified time

	/*
	 * The time before which to retrieve events.
	 * Only used if NVM_FILTER_ON_BEFORE is set in the #filter_mask.
	 */
	time_t			before; ///< filter on events before specified time

	/*
	 * Event ID number (row ID)
	 * Only used if NVM_FILTER_ON_EVENT is set in the #filter mask.
	 */
	int			event_id; ///< filter of specified event

	/*
	 * Only this action_required events are to be retrieved.
	 */
	NVM_BOOL		action_required;
};

/**
 * An entry in the native API trace log.
 */
struct nvm_log {
	NVM_PATH	file_name;                      ///<DEPRECATED, message string contains all data; The file that generated the log.
	int		line_number;                    ///<DEPRECATED, message string contains all data; The line number that generated the log.
	enum log_level	level;                          ///<DEPRECATED, message string contains all data; The log level.
	char		message[NVM_LOG_MESSAGE_LEN];   ///< The log message
	time_t		time;                           ///<DEPRECATED, message string contains all data; The time
};

/**
 * An injected device error.
 */
struct device_error {
	enum error_type		type;           ///< The type of error to inject.
	enum poison_memory_type memory_type;    ///< Poison type
	NVM_UINT64		dpa;            ///< only valid if injecting poison error
	NVM_UINT64		temperature;    ///< only valid if injecting temperature error
	NVM_UINT64		percentageRemaining;  ///< only valid if injecting percentage remaining error
};

/**
 * A structure to hold a diagnostic threshold.
 * Primarily for allowing caller to override default thresholds.
 */
struct diagnostic_threshold {
	diagnostic_threshold_type	type;                                   ///< A diagnostic threshold indicator
	NVM_UINT64			threshold;                              ///< numeric threshold
	char				threshold_str[NVM_THRESHOLD_STR_LEN];   ///< text value used as a "threshold"
};

/**
 * A diagnostic test.
 */
struct diagnostic {
	enum diagnostic_test		test;           ///< The type of diagnostic test to run
	NVM_UINT64			excludes;       ///< Bitmask - zero or more diagnostic_threshold_type enums
	struct diagnostic_threshold *	p_overrides;    ///< override default thresholds that trigger failure
	NVM_UINT32			overrides_len;  ///< size of p_overrides array
};

/**
 * Describes the identity of a system's physical processor in a NUMA context
 */
struct socket {
	NVM_UINT16	id;                                             ///< Zero-indexed NUMA node number
	NVM_UINT8	type;                                           ///< DEPRECATED; Physical processor type number (via CPUID)
	NVM_UINT8	model;                                          ///< DEPRECATED; Physical processor model number (via CPUID)
	NVM_UINT8	brand;                                          ///< DEPRECATED; Physical processor brand index (via CPUID)
	NVM_UINT8	family;                                         ///< DEPRECATED; Physical processor family number (via CPUID)
	NVM_UINT8	stepping;                                       ///< DEPRECATED; Physical processor stepping number (via CPUID)
	char		manufacturer[NVM_SOCKET_MANUFACTURER_LEN];      ///< DEPRECATED; Physical processor manufacturer (via CPUID)
	NVM_UINT16	logical_processor_count;                        ///< DEPRECATED; Logical processor count on node (incl. Hyperthreading)
	NVM_UINT64	mapped_memory_limit;                            ///< Maximum allowed memory (via PCAT)
	NVM_UINT64	total_mapped_memory;                            ///< Current occupied memory (via PCAT)
	NVM_UINT64	total_2lm_ddr_cache_memory;                     ///< DEPRECATED; cache size when in 2LM (via PCAT)
	NVM_BOOL	is_capacity_skuing_supported;                   ///< DEPRECATED; set to 1 if PCAT type 6 table found
};

struct job {
	NVM_UID			uid;
	NVM_UINT8		percent_complete;
	enum nvm_job_status	status;
	enum nvm_job_type	type;
	NVM_UID			affected_element;
	void *			result;
};

typedef union {
	struct pt_fw_thermal_log_entry_temp_data {
		unsigned int	temp : 15;
		unsigned int	sign : 1;
		unsigned int	reported : 3;
		unsigned int	type : 2;
		unsigned int	reserved : 11;
	}parts;
	unsigned int data;
}SMART_TEMP;

#define TEMP_POSITIVE           0
#define TEMP_NEGATIVE           1
#define TEMP_USER_ALARM         0
#define TEMP_LOW                        1
#define TEMP_HIGH                       2
#define TEMP_CRIT                       4
#define TEMP_TYPE_MEDIA         0
#define TEMP_TYPE_CORE          1
/*
 * ****************************************************************************
 * ENTRY POINT METHODS
 * ****************************************************************************
 */

/*
 * system.c
 */
extern NVM_API int nvm_acpi_event_create_ctx(unsigned int dimm_handle, void **ctx);
extern NVM_API int nvm_acpi_event_free_ctx(void *ctx);
extern NVM_API int nvm_acpi_event_ctx_get_dimm_handle(void *ctx, unsigned int *dev_handle);
extern NVM_API int nvm_acpi_event_get_event_state(void *ctx, enum acpi_event_type event_type, enum acpi_event_state *event_state);
extern NVM_API int nvm_acpi_event_set_monitor_mask(void *ctx, const unsigned int acpi_monitored_event_mask);
extern NVM_API int nvm_acpi_event_get_monitor_mask(void *ctx, unsigned int *mask);
extern NVM_API int nvm_acpi_wait_for_event(void *acpi_event_contexts[], const NVM_UINT32 dimm_cnt, const int timeout_sec, enum acpi_get_event_result *event_result);

extern NVM_API int nvm_get_dimm_id(const NVM_UID device_uid, unsigned int *dimm_id, unsigned int *dimm_handle);

extern NVM_API int nvm_get_config_int(const char *param_name, int default_val);
/**
 * @brief  Retrieve just the host server name that the native API is running on.
 * @param[in, out] host_name
 *              A caller supplied buffer to hold the host server name
 * @param[in] host_name_len
 *              The length of the host_name buffer. Should be = NVM_COMPUTERNAME_LEN.
 * @return Returns one of the following @link #return_code return_codes: @endlink @n
 *              NVM_SUCCESS @n
 *    NVM_ERR_INVALID_PARAMETER @n
 *              NVM_ERR_UNKNOWN @n
 */
extern NVM_API int nvm_get_host_name(char *host_name, const NVM_SIZE host_name_len);

/**
 * @brief Retrieve basic information about the host server the native API library is running on.
 * @param[in,out] p_host
 *              A pointer to a #host structure allocated by the caller.
 * @pre The caller must have administrative privileges.
 * @return Returns one of the following @link #return_code return_codes: @endlink @n
 *              NVM_SUCCESS @n
 *              NVM_ERR_INVALIDPARAMETER @n
 *              NVM_ERR_UNKNOWN @n
 */
extern NVM_API int nvm_get_host(struct host *p_host);

/**
 * @brief Retrieve a list of installed software versions related to AEP DIMM management.
 * @param[in,out] p_inventory
 *              A pointer to a #sw_inventory structure allocated by the caller.
 * @pre The caller must have administrative privileges.
 * @remarks If a version cannot be retrieved, the version is returned as all zeros.
 * @remarks AEP DIMM firmware revisions are not included.
 * @return Returns one of the following return_codes:
 *              NVM_SUCCESS @n
 *              NVM_ERR_INVALIDPARAMETER @n
 *              NVM_ERR_UNKNOWN @n
 */
extern NVM_API int nvm_get_sw_inventory(struct sw_inventory *p_inventory);

/**
 * @deprecated Please use: nvm_get_number_of_sockets
 * @brief Retrieves the number of physical processors (NUMA nodes) in the system.
 * @pre
 *              The OS must support its respective NUMA implementation.
 * @remarks
 *              This method should be called before #nvm_get_socket or #nvm_get_sockets
 * @remarks
 *              This method should never return a value less than 1.
 * @return
 *              Returns the number of nodes on success or one of the following @link #return_code
 *              return_codes: @endlink @n
 *              -1 @n
 */
extern NVM_API int nvm_get_socket_count();

/**
 * @brief Retrieves the number of physical processors (NUMA nodes) in the system.
 * @pre
 *              The OS must support its respective NUMA implementation.
 * @remarks
 *              This method should be called before #nvm_get_socket or #nvm_get_sockets
 * @remarks
 *              This method should never return a value less than 1.
 * @param[in,out] count
 *              A pointer to an integer which contain the number of sockets on return.
 * @return
 *              Returns the number of nodes on success or one of the following @link #return_code
 *              return_codes: @endlink @n
 *              NVM_SUCCESS @n
 *              NVM_ERR_INVALIDPARAMETER @n
 *              NVM_ERR_UNKNOWN @n
 */
extern NVM_API int nvm_get_number_of_sockets(int *count);
/**
 * @brief Retrieves #socket information about each processor socket in the system.
 *
 * @param[in,out] p_sockets
 *              An array of #socket structures allocated by the caller.
 * @param[in] count
 *              The size of the array
 * @return
 *              Returns the number of nodes on success or one of the following @link #return_code
 *              return_codes: @endlink @n
 *              NVM_SUCCESS @n
 *              NVM_ERR_INVALIDPARAMETER @n
 *              NVM_ERR_UNKNOWN @n
 */
extern NVM_API int nvm_get_sockets(struct socket *p_sockets, const NVM_UINT16 count);

/**
 * @brief Retrieves #socket information about a given processor socket.
 * @pre
 *              The OS must support its respective NUMA implementation.
 * @param[in] socket_id
 *              The NUMA node identifier
 * @param[in,out] p_socket
 *              A pointer to a #socket structure allocated by the caller.
 * @return
 *              Returns one of the following @link #return_code return_codes: @endlink @n
 *              NVM_SUCCESS @n
 *              NVM_ERR_INVALIDPARAMETER @n
 *              NVM_ERR_UNKNOWN @n
 */
extern NVM_API int nvm_get_socket(const NVM_UINT16 socket_id, struct socket *p_socket);


/**
 * @}
 * @defgroup Device
 * @{
 */

/*
 * @deprecated please use: nvm_get_number_of_memory_topology_devices
 * @brief Returns the number of memory devices installed in the system. This count includes
 * both AEP DIMMs and other memory devices, such as DRAM.
 * @pre The caller must have administrative privileges.
 * @remarks This method should be called before #nvm_get_device_topology.
 * @return Returns the number of devices in the topology on success
 * or one of the following @link #return_code return_codes: @endlink @n
 *              -1
 */
extern NVM_API int nvm_get_memory_topology_count();
extern NVM_API int nvm_get_number_of_memory_topology_devices(int *count);

/**
 * @brief Retrieves basic topology information about all memory devices installed in the
 * system, including both AEP DIMMs and other memory devices, such as DRAM.
 * @pre The caller must have administrative privileges.
 * @return
 *    Returns one of the following @link #return_code return_codes: @endlink @n
 *              NVM_SUCCESS @n
 *              NVM_ERR_INVALIDPARAMETER @n
 *              NVM_ERR_UNKNOWN @n
 */
extern NVM_API int nvm_get_memory_topology(struct memory_topology *p_devices, const NVM_UINT8 count);

/*
 * @deprecated please use: nvm_get_number_of_devices
 * @brief Returns the number of devices installed in the system whether they are
 * fully compatible with the current native API library version or not.
 * @pre The caller must have administrative privileges.
 * @remarks This method should be called before #nvm_get_devices.
 * @remarks The number of devices can be 0.
 * @return Returns the number of devices on success
 * or one of the following @link #return_code return_codes: @endlink @n
 *              -1 @n
 */
extern NVM_API int nvm_get_device_count();
extern NVM_API int nvm_get_number_of_devices(int *count);

/**
 * @brief Retrieves #device_discovery information
 * about each device in the system whether they are fully compatible
 * with the current native API library version or not.
 * @param[in,out] p_devices
 *              An array of #device_discovery structures allocated by the caller.
 * @param[in] count
 *              The size of the array.
 * @pre The caller must have administrative privileges.
 * @remarks To allocate the array of #device_discovery structures,
 * call #nvm_get_device_count before calling this method.
 * @return Returns the number of devices on success
 * or one of the following @link #return_code return_codes: @endlink @n
 *              -1 @n
 */
extern NVM_API int nvm_get_devices(struct device_discovery *p_devices, const NVM_UINT8 count);
extern NVM_API int nvm_get_devices_nfit(struct device_discovery *p_devices, const NVM_UINT8 count);

/**
 * @brief Retrieve #device_discovery information about the device specified.
 * @param[in] device_uid
 *              The device identifier.
 * @param[in,out] p_discovery
 *              A pointer to a #device_discovery structure allocated by the caller.
 * @pre The caller must have administrative privileges.
 * @return Returns one of the following @link #return_code return_codes: @endlink @n
 *              NVM_SUCCESS @n
 *              NVM_ERR_INVALID_PARAMETER @n
 *              NVM_ERR_DIMM_NOT_FOUND @n
 */
extern NVM_API int nvm_get_device_discovery(const NVM_UID device_uid, struct device_discovery *p_discovery);

/**
 * @brief Retrieve the #device_status of the device specified.
 * @param[in] device_uid
 *              The device identifier.
 * @param[in,out] p_status
 *              A pointer to a #device_status structure allocated by the caller.
 * @pre The caller must have administrative privileges.
 * @pre The device is manageable.
 * @return Returns one of the following @link #return_code return_codes: @endlink @n
 *              NVM_SUCCESS @n
 *              NVM_ERR_INVALID_PARAMETER @n
 *              NVM_ERR_DIMM_NOT_FOUND @n
 */
extern NVM_API int nvm_get_device_status(const NVM_UID device_uid, struct device_status *p_status);

/**
 * @brief Retrieve the PMON Registers of device specified.
 * @param[in] device_uid
 *              The device identifier.
 * @param[in,out] output_payload
 * @pre The caller must have administrative privileges.
 * @pre The device is manageable.
 * @return Returns one of the following @link #return_code return_codes: @endlink @n
 *              NVM_SUCCESS @n
 *              NVM_ERR_INVALID_PARAMETER @n
 *              NVM_ERR_DIMM_NOT_FOUND @n
 */
extern NVM_API int nvm_get_pmon_registers(const NVM_UID device_uid, const NVM_UINT8 SmartDataMask, PMON_REGISTERS *p_output_payload);

/**
 * @brief Set the PMON Registers of device specified.
 * @param[in] device_uid
 *              The device identifier.
  * @param[in] device_uid
 *              The device identifier.
 * @pre The caller must have administrative privileges.
 * @pre The device is manageable.
 * @return Returns one of the following @link #return_code return_codes: @endlink @n
 *              NVM_SUCCESS @n
 *              NVM_ERR_INVALID_PARAMETER @n
 *              NVM_ERR_DIMM_NOT_FOUND @n
 */
extern NVM_API int nvm_set_pmon_registers(const NVM_UID device_uid, NVM_UINT8 PMONGroupEnable);


/**
 * @brief Retrieve #device_settings information about the device specified.
 * @param[in] device_uid
 *              The device identifier.
 * @param[out] p_settings
 *              A pointer to a #device_settings structure allocated by the caller.
 * @pre The caller must have administrative privileges.
 * @pre The device is manageable.
 * @return Returns one of the following @link #return_code return_codes: @endlink @n
 *              NVM_SUCCESS @n
 *              NVM_ERR_INVALID_PARAMETER @n
 *              NVM_ERR_UNKNOWN @n
 */
extern NVM_API int nvm_get_device_settings(const NVM_UID device_uid, struct device_settings *p_settings);

/**
 * @brief Set one or more configurable properties on the specified device.
 * @param[in] device_uid
 *              The device identifier.
 * @param[in] p_settings
 *              A pointer to an #device_settings structure containing the modified settings.
 * @pre The caller must have administrative privileges.
 * @pre The device is manageable.
 * @remarks Retrieve the current #device_settings using #nvm_get_device_details and change
 * the specific settings as desired.
 * @remarks A given property change may require similar changes to related devices to
 * represent a consistent correct configuration.
 * @return Returns one of the following @link #return_code return_codes: @endlink @n
 *              NVM_SUCCESS @n
 *              NVM_ERR_INVALID_PARAMETER @n
 *              NVM_ERR_UNKNOWN @n
 */
extern NVM_API int nvm_modify_device_settings(const NVM_UID device_uid, const struct device_settings *p_settings);

/**
 * @brief Retrieve #device_details information about the device specified.
 * @param[in] device_uid
 *              The device identifier.
 * @param[in,out] p_details
 *              A pointer to a #device_details structure allocated by the caller.
 * @pre The caller must have administrative privileges.
 * @pre The device is manageable.
 * @return Returns one of the following @link #return_code return_codes: @endlink @n
 *              NVM_SUCCESS @n
 *              NVM_ERR_INVALID_PARAMETER @n
 *              NVM_ERR_DIMM_NOT_FOUND @n
 *              NVM_ERR_UNKNOWN @n
 */
extern NVM_API int nvm_get_device_details(const NVM_UID device_uid, struct device_details *p_details);

/**
 * @brief Retrieve a current snapshot of the performance metrics for the device specified.
 * @param[in] device_uid
 *              The device identifier.
 * @param[in,out] p_performance
 *              A pointer to a #device_performance structure allocated by the caller.
 * @pre The caller must have administrative privileges.
 * @pre The device is manageable.
 * @return Returns one of the following @link #return_code return_codes: @endlink @n
 *              NVM_SUCCESS @n
 *              NVM_ERR_INVALID_PARAMETER @n
 *              NVM_ERR_UNKNOWN @n
 */
extern NVM_API int nvm_get_device_performance(const NVM_UID device_uid, struct device_performance *p_performance);

/**
 * @brief Retrieve the firmware image log information from the device specified.
 * @param[in] device_uid
 *              The device identifier.
 * @param[in, out] p_fw_info
 *              A pointer to a #device_fw_info structure allocated by the caller.
 * @pre The caller has administrative privileges.
 * @pre The device is manageable.
 * @return Returns one of the following @link #return_code return_codes: @endlink @n
 *              NVM_SUCCESS @n
 *              NVM_ERR_INVALID_PARAMETER @n
 *              NVM_ERR_UNKNOWN @n
 */
extern NVM_API int nvm_get_device_fw_image_info(const NVM_UID device_uid, struct device_fw_info *p_fw_info);

/**
 * @brief Push a new FW image to the device specified.
 * @param[in] device_uid
 *              The device identifier.
 * @param[in] path
 *              Absolute file path to the new firmware image.
 * @param[in] path_len
 *              String length of path, should be < NVM_PATH_LEN.
 * @param[in] force
 *              If attempting to downgrade the minor version, force must be true.
 * @pre The caller has administrative privileges.
 * @pre The device is manageable.
 * @remarks A FW update may require similar changes to related devices to
 * represent a consistent correct configuration.
 * @return Returns one of the following @link #return_code return_codes: @endlink @n
 *              NVM_SUCCESS @n
 *              NVM_ERR_NOTSUPPORTED @n
 *              NVM_ERR_NOMEMORY @n
 *              NVM_ERR_BADDEVICE @n
 *              NVM_ERR_INVALIDPERMISSIONS @n
 *              NVM_ERR_INVALIDPARAMETER @n
 *              NVM_ERR_NOTMANAGEABLE @n
 *		NVM_ERR_DRIVERFAILED @n
 *              NVM_ERR_BADFILE @n
 *              NVM_ERR_DATATRANSFERERROR @n
 *              NVM_ERR_DEVICEERROR @n
 *              NVM_ERR_DEVICEBUSY @n
 *              NVM_ERR_UNKNOWN @n
 *              NVM_ERR_BADFIRMWARE @n
 *              NVM_ERR_REQUIRESFORCE @n
 *              NVM_ERR_BADDRIVER @n
 *              NVM_ERR_NOSIMULATOR (Simulated builds only)
 */
extern NVM_API int nvm_update_device_fw(const NVM_UID device_uid, const NVM_PATH path, const NVM_SIZE path_len, const NVM_BOOL force);

/**
 * @brief Examine the FW image to determine if it is valid for the device specified.
 * @param[in] device_uid
 *              The device identifier.
 * @param[in] path
 *              Absolute file path to the new firmware image.
 * @param[in] path_len
 *              String length of path, should be < NVM_PATH_LEN.
 * @param image_version
 *              Firmware image version returned after examination
 * @param image_version_len
 *              Buffer size for the image version
 * @pre The caller has administrative privileges.
 * @pre The device is manageable.
 * @remarks A FW update may require similar changes to related devices to
 * represent a consistent correct configuration.
 * @return Returns one of the following @link #return_code return_codes: @endlink @n
 *              NVM_SUCCESS @n
 *              NVM_ERR_BADFIRMWARE @n
 *              NVM_ERR_NOTSUPPORTED @n
 *              NVM_ERR_NOMEMORY @n
 *              NVM_ERR_BADDEVICE @n
 *              NVM_ERR_INVALIDPERMISSIONS @n
 *              NVM_ERR_INVALIDPARAMETER @n
 *              NVM_ERR_NOTMANAGEABLE @n
 *              NVM_ERR_DRIVERFAILED @n
 *              NVM_ERR_BADFILE @n
 *              NVM_ERR_DATATRANSFERERROR @n
 *              NVM_ERR_DEVICEERROR @n
 *              NVM_ERR_DEVICEBUSY @n
 *              NVM_ERR_REQUIRESFORCE @n
 *              NVM_ERR_UNKNOWN @n
 *              NVM_ERR_BADDRIVER @n
 *              NVM_ERR_NOSIMULATOR (Simulated builds only)
 */
extern NVM_API int nvm_examine_device_fw(const NVM_UID device_uid, const NVM_PATH path, const NVM_SIZE path_len, NVM_VERSION image_version, const NVM_SIZE image_version_len);

/**
 * @brief Retrieve the supported capabilities for all devices in aggregate.
 * @param[in,out] p_capabilties
 *              A pointer to an #nvm_capabilities structure allocated by the caller.
 * @pre The caller must have administrative privileges.
 * @return Returns one of the following @link #return_code return_codes: @endlink @n
 *              NVM_SUCCESS @n
 *              NVM_ERR_INVALID_PARAMETER @n
 *              NVM_ERR_UNKNOWN @n
 */
extern NVM_API int nvm_get_nvm_capabilities(struct nvm_capabilities *p_capabilties);

/**
 * @brief Retrieve the aggregate capacities across all manageable AEP DIMMs in the system.
 * @param[in,out] p_capacities
 *              A pointer to an #device_capacities structure allocated by the caller.
 * @pre The caller must have administrative privileges.
 * @return Returns one of the following @link #return_code return_codes: @endlink @n
 *              NVM_SUCCESS @n
 *              NVM_ERR_INVALID_PARAMETER @n
 *              NVM_ERR_UNKNOWN @n
 */
extern NVM_API int nvm_get_nvm_capacities(struct device_capacities *p_capacities);

/**
 * @}
 * @defgroup Security
 * @{
 */

/***
 * @brief If data at rest security is not enabled, this method enables it and
 * sets the passphrase. If data at rest security was previously enabled, this method changes
 * the passphrase to the new passphrase specified.
 * @param[in] device_uid
 *              The device identifier.
 * @param[in] old_passphrase
 *              The current passphrase or NULL if security is disabled.
 * @param[in] old_passphrase_len
 *              String length of old_passphrase,
 *              should be <= NVM_PASSPHRASE_LEN or 0 if security is disabled.
 * @param[in] new_passphrase
 *              The new passphrase.
 * @param[in] new_passphrase_len
 *              String length of new_passphrase, should be <= NVM_PASSPHRASE_LEN.
 * @pre The caller has administrative privileges.
 * @pre The device is manageable.
 * @pre Device security is not frozen.
 * @pre The device passphrase limit has not been reached.
 * @post The device will be unlocked and frozen.
 * @post The device will be locked on the next reset.
 * @return Returns one of the following @link #return_code return_codes: @endlink @n
 *              NVM_ERR_NOTSUPPORTED @n
 */
extern NVM_API int nvm_set_passphrase(const NVM_UID device_uid, const NVM_PASSPHRASE old_passphrase, const NVM_SIZE old_passphrase_len, const NVM_PASSPHRASE new_passphrase, const NVM_SIZE new_passphrase_len);

/***
 * @deprecated Not supported
 * @brief Disables data at rest security and removes the passphrase.
 * @param[in] device_uid
 *              The device identifier.
 * @param[in] passphrase
 *              The current passphrase.
 * @param[in] passphrase_len
 *              String length of passphrase, should be <= NVM_PASSPHRASE_LEN.
 * @pre The caller has administrative privileges.
 * @pre The device is manageable.
 * @pre Device security is enabled and the passphrase has been set using #nvm_set_passphrase.
 * @pre Device security is not frozen.
 * @pre The device passphrase limit has not been reached.
 * @post The device will be unlocked if it is currently locked.
 * @post Device security will be disabled.
 * @return Returns one of the following @link #return_code return_codes: @endlink @n
 *              NVM_SUCCESS @n
 *              NVM_ERR_NOTSUPPORTED @n
 *              NVM_ERR_NOMEMORY @n
 *              NVM_ERR_BADDEVICE @n
 *              NVM_ERR_INVALIDPARAMETER @n
 *              NVM_ERR_INVALIDPERMISSIONS @n
 *              NVM_ERR_NOTMANAGEABLE @n
 *              NVM_ERR_DRIVERFAILED @n
 *              NVM_ERR_SECURITYFROZEN @n
 *              NVM_ERR_SECURITYDISABLED @n
 *              NVM_ERR_LIMITPASSPHRASE @n
 *              NVM_ERR_BADPASSPHRASE @n
 *              NVM_ERR_DATATRANSFERERROR @n
 *              NVM_ERR_DEVICEERROR @n
 *              NVM_ERR_DEVICEBUSY @n
 *              NVM_ERR_UNKNOWN @n
 *              NVM_ERR_BADDRIVER @n
 *              NVM_ERR_NOSIMULATOR (Simulated builds only)
 */
extern NVM_API int nvm_remove_passphrase(const NVM_UID device_uid, const NVM_PASSPHRASE passphrase, const NVM_SIZE passphrase_len);

/***
 * @brief Unlocks the device with the passphrase specified.
 * @param[in] device_uid
 *              The device identifier.
 * @param[in] passphrase
 *              The current passphrase.
 * @param[in] passphrase_len
 *              String length of passphrase, should be <= NVM_PASSPHRASE_LEN.
 * @pre The caller has administrative privileges.
 * @pre The device is manageable.
 * @pre Device security is enabled and the passphrase has been set using #nvm_set_passphrase.
 * @pre Device security is not frozen.
 * @pre The device passphrase limit has not been reached.
 * @post The device will be unlocked and frozen.
 * @return Returns one of the following @link #return_code return_codes: @endlink @n
 *              NVM_ERR_NOTSUPPORTED @n
 */
extern NVM_API int nvm_unlock_device(const NVM_UID device_uid, const NVM_PASSPHRASE passphrase, const NVM_SIZE passphrase_len);

/***
 * @brief Prevent security lock state changes to the dimm until the next reboot
 * @param[in] device_uid
 *              The device identifier.
 * @pre The caller has administrative privileges.
 * @pre The device is manageable.
 * @pre The device supports unlocking a device.
 * @pre Current dimm security state is unlocked
 * @post dimm security state will be frozen
 * @post Device security will be changed.
 * @return Returns one of the following @link #return_code return_codes: @endlink @n
 *              NVM_ERR_NOTSUPPORTED @n
 */
extern NVM_API int nvm_freezelock_device(const NVM_UID device_uid);

/***
 * @brief Erases data on the device specified by zeroing the device encryption key.
 * @param[in] device_uid
 *              The device identifier.
 * @param[in] passphrase
 *              The current passphrase.
 * @param[in] passphrase_len
 *              String length of passphrase, should be <= NVM_PASSPHRASE_LEN.
 * @pre The caller has administrative privileges.
 * @pre The device is manageable.
 * @pre The device supports overwriting a device.
 * @pre Device security is disabled or sanitize antifreeze.
 * @post All user data is inaccessible.
 * @post Device security will be changed.
 * @return Returns one of the following @link #return_code return_codes: @endlink @n
 *              NVM_SUCCESS @n
 *              NVM_ERR_NOTSUPPORTED @n
 *              NVM_ERR_NOMEMORY @n
 *		NVM_ERR_BADDEVICE @n
 *              NVM_ERR_INVALIDPARAMETER @n
 *              NVM_ERR_INVALIDPERMISSIONS @n
 *              NVM_ERR_NOTMANAGEABLE @n
 *              NVM_ERR_DRIVERFAILED @n
 *              NVM_ERR_SECURITYFROZEN @n
 *              NVM_ERR_BADPASSPHRASE @n
 *              NVM_ERR_DATATRANSFERERROR @n
 *              NVM_ERR_DEVICEERROR @n
 *              NVM_ERR_DEVICEBUSY @n
 *              NVM_ERR_UNKNOWN @n
 *              NVM_ERR_BADDRIVER @n
 *              NVM_ERR_NOSIMULATOR (Simulated builds only)
 */
extern NVM_API int nvm_erase_device(const NVM_UID device_uid, const NVM_PASSPHRASE passphrase, const NVM_SIZE passphrase_len);

/***
 * @deprecated Not supported
 * @brief Get security permission to access/modify the device.
 * @param[in] p_discovery
 *              pinter to struct device_discovery.
 * @pre The caller has administrative privileges.
 * @pre The device is manageable.
 * @return Returns one of the following @link #return_code return_codes: @endlink @n
 *              NVM_SUCCESS @n
 *              NVM_ERR_UNKNOWN @n
 *		NVM_ERR_SECURITYFROZEN @n
 *              NVM_ERR_LIMITPASSPHRASE @n
 *		NVM_ERR_NOTSUPPORTED @n
 */
extern NVM_API int nvm_get_security_permission(struct device_discovery *p_discovery);

/**
 * @}
 * @defgroup Monitor
 * @{
 */

/***
 * @brief Retrieve all the health sensors for the specified AEP DIMM.
 * @param[in] device_uid
 *              The device identifier.
 * @param[in,out] p_sensors
 *              An array of #sensor structures allocated by the caller.
 * @param[in] count
 *              The size of the array.  Should be NVM_MAX_DEVICE_SENSORS.
 * @pre The caller has administrative privileges.
 * @pre The device is manageable.
 * @remarks Sensors are used to monitor a particular aspect of a device by
 * settings thresholds against a current value.
 * @remarks The number of sensors for a device is defined as NVM_MAX_DEVICE_SENSORS.
 * @remarks Sensor information is returned as part of the #device_details structure.
 * @return Returns one of the following @link #return_code return_codes: @endlink @n
 *              NVM_SUCCESS @n
 *              NVM_ERR_INVALID_PARAMETER @n
 *              NVM_ERR_UNKNOWN @n
 */
extern NVM_API int nvm_get_sensors(const NVM_UID device_uid, struct sensor *p_sensors, const NVM_UINT16 count);

/***
 * @brief Retrieve a specific health sensor from the specified AEP DIMM.
 * @param[in] device_uid
 *              The device identifier.
 * @param[in] s_type
 *              The specific #sensor_type to retrieve.
 * @param[in,out] p_sensor
 *              A pointer to a #sensor structure allocated by the caller.
 * @pre The caller has administrative privileges.
 * @pre The device is manageable.
 * @return Returns one of the following @link #return_code return_codes: @endlink @n
 *              NVM_SUCCESS @n
 *              NVM_ERR_INVALID_PARAMETER @n
 *              NVM_ERR_UNKNOWN @n
 */
extern NVM_API int nvm_get_sensor(const NVM_UID device_uid, const enum sensor_type type, struct sensor *p_sensor);

/**
 * @brief Change the critical threshold on the specified health sensor for the specified
 * AEP DIMM.
 * @param[in] device_uid
 *              The device identifier.
 * @param[in] s_type
 *              The specific #sensor_type to modify.
 * @param[in] p_sensor_settings
 *              The modified settings.
 * @pre The caller has administrative privileges.
 * @pre The device is manageable.
 * @return Returns one of the following @link #return_code return_codes: @endlink @n
 *              NVM_SUCCESS @n
 *              NVM_ERR_INVALID_PARAMETER @n
 *              NVM_ERR_UNKNOWN @n
 */
extern NVM_API int nvm_set_sensor_settings(const NVM_UID device_uid, const enum sensor_type type, const struct sensor_settings *p_settings);

/***
 * @deprecated Not supprted
 * @brief Adds a function to be called when an event that matches the specified type.
 * An event structure is populated and passed
 * to the callback function describing the event that occurred.
 * @param[in] type
 *              The type of event to notify on.  Use #EVENT_TYPE_ALL to subscribe
 *              to all events for the device.
 * @param[in] p_event_callback
 *              A pointer to the callback function.
 * @pre The caller has administrative privileges.
 * @remarks Callers must remove the callback using #nvm_remove_event_notify
 * to avoid memory leaks.
 * @return Returns a callback identifier if the callback was added or one of the
 * following @link #return_code return_codes: @endlink @n
 *              NVM_ERR_NOTSUPPORTED @n
 *              NVM_ERR_NOMEMORY @n
 *              NVM_ERR_INVALIDPARAMETER @n
 *              NVM_ERR_INVALIDPERMISSIONS @n
 *              NVM_ERR_UNKNOWN @n
 */
extern NVM_API int nvm_add_event_notify(const enum event_type type, void (*p_event_callback)(struct event *p_event));

/***
 * @deprecated Not supprted
 * @brief Remove a callback subscription that was previously added using #nvm_add_event_notify.
 * @param[in] callback_id
 *              The identifier for a specific callback, returned from #nvm_add_event_notify.
 * @pre The caller has administrative privileges.
 * @pre The callback was previously added using #nvm_add_event_notify
 * @return Returns one of the following @link #return_code return_codes: @endlink @n
 *              NVM_SUCCESS @n
 *              NVM_ERR_NOTSUPPORTED @n
 *              NVM_ERR_INVALIDPERMISSIONS @n
 *              NVM_ERR_NOMEMORY @n
 *              NVM_ERR_BADCALLBACK @n
 *              NVM_ERR_UNKNOWN @n
 */
extern NVM_API int nvm_remove_event_notify(const int callback_id);

/***
 * @deprecated please us: nvm_get_number_of_events
 * @brief Retrieve the number of events in the native API library event database.
 * @param[in] p_filter
 *              A pointer to an event_filter structure allocated by the caller to
 *              optionally filter the event count. NULL will return the count of
 *              all event log entries.
 * @pre The caller must have administrative privileges.
 * @return Returns the number of events on success or
 * one of the following @link #return_code return_codes: @endlink @n
 *              -1 @n
 */
extern NVM_API int nvm_get_event_count(const struct event_filter *p_filter);

/***
 * @brief Retrieve the number of events in the native API library event database.
 * @param[in] p_filter
 *              A pointer to an event_filter structure allocated by the caller to
 *              optionally filter the event count.
 * @pre The caller must have administrative privileges.
 * @return Returns the number of events on success or
 * one of the following @link #return_code return_codes: @endlink @n
 *     NVM_SUCCESS @n
 *              NVM_ERR_INVALIDPARAMETER @n
 */
extern NVM_API int nvm_get_number_of_events(const struct event_filter *p_filter, int *count);

/***
 * @brief Retrieve a list of stored events from the native API library database and
 * optionally filter the results.
 * @param[in] p_filter
 *              A pointer to an event_filter structure to optionally
 *              limit the results.  NULL to return all the events.
 * @param[in,out] p_events
 *              An array of #event structures allocated by the caller.
 * @param[in] count
 *              The size of the array.
 * @pre The caller must have administrative privileges.
 * @remarks The native API library stores a maximum of 10,000 events in the table,
 * rolling the table once the maximum is reached. However, the maximum number of events
 * is configurable by modifying the EVENT_LOG_MAX_ROWS value in the configuration database.
 * @return Returns the number of events on success or
 * one of the following @link #return_code return_codes: @endlink @n
 *    NVM_SUCCESS @n
 *              NVM_ERR_INVALIDPARAMETER @n
 */
extern NVM_API int nvm_get_events(const struct event_filter *p_filter, struct event *p_events, const NVM_UINT16 count);

/***
 * @brief Purge stored events from the native API database.
 * @param[in] p_filter
 *              A pointer to an event_filter structure to optionally
 *              purge only specific events.
 * @pre The caller must have administrative privileges.
 * @return Returns the number of events removed or
 * one of the following @link #return_code return_codes: @endlink @n
 *              NVM_SUCCESS @n
 *              NVM_ERR_INVALIDPARAMETER @n
 *              NVM_ERR_UNKNOWN @n
 */
extern NVM_API int nvm_purge_events(const struct event_filter *p_filter);

/***
 * @brief Acknowledge an event from the native API database.
 * (i.e. setting action required field from true to false)
 * @param[in] event_id
 *              The event id of the event to be acknowledged.
 * @return Returns one of the following @link #return_code return_codes: @endlink @n
 *              NVM_SUCCESS @n
 *              NVM_ERR_UNKNOWN @n
 */
extern NVM_API int nvm_acknowledge_event(NVM_UINT32 event_id);

/**
 * @}
 * @defgroup Namespace
 * @{
 */

/***
 * @deprecated please use: nvm_get_number_of_regions
 * @brief Retrieve the number of configured regions of AEP DIMM capacity in the host server.
 * @pre The caller has administrative privileges.
 * @remarks This method should be called before #nvm_get_regions.
 * @return Returns the number of regions on success or
 * one of the following @link #return_code return_codes: @endlink @n
 *              NVM_ERR_INVALIDPERMISSIONS @n
 *              NVM_ERR_NOTSUPPORTED @n
 *              NVM_ERR_NOMEMORY @n
 *              NVM_ERR_DRIVERFAILED @n
 *              NVM_ERR_UNKNOWN @n
 *              NVM_ERR_BADDRIVER @n
 *              NVM_ERR_NOSIMULATOR (Simulated builds only) @n
 */
extern NVM_API int nvm_get_region_count();

/***
 * @brief Retrieve the number of configured regions of AEP DIMM capacity in the host server.
 * @pre The caller has administrative privileges.
 * @remarks This method should be called before #nvm_get_regions.
 * @param[in,out] count
 *              A pointer an integer that will contain the number of region count on return
 * @return one of the following @link #return_code return_codes: @endlink @n
 *              NVM_SUCCESS @n
 *              NVM_ERR_INVALIDPARAMETER @n
 *              NVM_ERR_UNKNOWN @n
 */
extern NVM_API int nvm_get_number_of_regions(int *count);

/***
 * @brief Retrieve a list of the configured regions of AEP DIMM capacity in host server.
 * @param[in,out] p_regions
 *              An array of #region structures allocated by the caller.
 * @param[in] count
 *              The size of the array.
 * @pre The caller has administrative privileges.
 * @remarks To allocate the array of #region structures,
 * call #nvm_get_region_count before calling this method.
 * @return Returns the number of regions on success
 * or one of the following @link #return_code return_codes: @endlink @n
 *              NVM_SUCCESS
 *              NVM_ERR_INVALIDPARAMETER @n
 *              NVM_ERR_UNKNOWN @n
 *              NVM_ERR_NO_MEM @n
 */
extern NVM_API int nvm_get_regions(struct region *p_regions, const NVM_UINT8 count);

/***
 * @brief Retrieve a specific region of AEP DIMM capacity.
 * @param[in] region_uid
 *              The identifier of the region to retrieve
 * @param[in,out] p_region
 *              A pointer to a #region structure allocated by the caller.
 * @pre The caller has administrative privileges.
 * @return Returns one of the following @link #return_code return_codes: @endlink @n
 *              NVM_SUCCESS @n
 *              NVM_ERR_INVALIDPARAMETER @n
 *              NVM_ERR_NO_MEM @n
 *              NVM_ERR_UNKNOWN @n
 */
extern NVM_API int nvm_get_region(const NVM_UID region_uid, struct region *p_region);

/***
 * @deprecated This function is no longer supported.
 * @brief Takes a region UUID and returns the largest and smallest app direct and storage namespaces
 * that can be created on that region.
 * @pre The caller has administrative privileges.
 * @param[in] region_id
 *              UUID of the region getting ranges for
 * @return Returns one of the following @link #return_code return_codes: @endlink @n
 * @param[in,out] p_range
 *              Structure that will contain the ranges
 * @return Returns one of the following @link #return_code return_codes: @endlink @n
 *              NVM_ERR_API_NOT_SUPPORTED @n
 */
extern NVM_API int nvm_get_available_persistent_size_range(const NVM_UID region_uid, struct possible_namespace_ranges *p_range, const NVM_UINT8 ways);

/***
 * @brief Modify how the AEP DIMM capacity is provisioned by the BIOS on the next reboot.
 * @param p_device_uids
 *              Pointer to list of device uids to configure.
 *              If NULL, all devices on platform will be configured.
 * @param device_uids_couut
 *              Number of devices in p_device_uids list.
 * @pre The caller has administrative privileges.
 * @pre The specified AEP DIMM is manageable by the host software.
 * @pre Any existing namespaces created from capacity on the
 *              AEP DIMM must be deleted first.
 * @remarks This operation stores the specified configuration goal on the AEP DIMM
 *              for the BIOS to read on the next reboot.
 * @return Returns one of the following @link #return_code return_codes: @endlink @n
 *		NVM_SUCCESS @n
 *              NVM_ERR_INVALIDPARAMETER @n
 *              NVM_ERR_UNKNOWN @n
 */
NVM_API int nvm_create_config_goal(NVM_UID *p_device_uids, NVM_UINT32 device_uids_count, struct config_goal_input *p_goal);

/***
 * @brief Retrieve the configuration goal from the specified AEP DIMM.
 * @param p_device_uids
 *              Pointer to list of device uids to retrieve config goal from.
 *              If NULL, retrieve goal configs from all devices on platform.
 * @param device_uids_count
 *              Number of devices in p_device_uids list.
 * @param p_goal
 *              A pointer to a list of config_goal structures allocated by the caller.
 * @pre The caller has administrative privileges.
 * @pre The specified AEP DIMM is manageable by the host software.
 * @remarks A configuration goal is stored on the AEP DIMM until the
 *              BIOS successfully processes it on reboot.
 *              Use @link nvm_delete_config_goal @endlink to erase a
 *              configuration goal from an AEP DIMM.
 * @return Returns one of the following @link #return_code return_codes: @endlink @n
 *		NVM_SUCCESS @n
 *              NVM_ERR_INVALIDPARAMETER @n
 *              NVM_ERR_UNKNOWN @n
 */
extern NVM_API int nvm_get_config_goal(NVM_UID *p_device_uids, NVM_UINT32 device_uids_count, struct config_goal *p_goal);

/***
 * @brief Erase the region configuration goal from the specified AEP DIMM.
 * @param p_device_uids
 *              Pointer to list of device uids to erase the region config goal.
 *              If NULL, all devices on platform will have their region config goal erased.
 * @param device_uids_count
 *              Number of devices in p_device_uids list.
 * @pre The caller has administrative privileges.
 * @pre The specified AEP DIMM is manageable by the host software.
 * @return Returns one of the following @link #return_code return_codes: @endlink @n
 *		NVM_SUCCESS @n
 *              NVM_ERR_INVALIDPARAMETER @n
 *              NVM_ERR_UNKNOWN @n
 */
extern NVM_API int nvm_delete_config_goal(NVM_UID *p_device_uids, NVM_UINT32 device_uids_count);

/***
 * @deprecated No longer supported
 * @brief Store the configuration settings of how the AEP DIMM capacity
 * is currently provisioned to a file in order to duplicate the
 * configuration elsewhere.
 * @param device_uid
 *              The AEP DIMM identifier.
 * @param file
 *              The absolute file path in which to store the configuration data.
 * @param file_len
 *              String length of file, should be < #NVM_PATH_LEN.
 * @param append
 *              Whether to append the AEP DIMM configuration to an existing file
 *              or create/replace the file.
 * @pre The caller has administrative privileges.
 * @pre The specified AEP DIMM is manageable by the host software.
 * @pre The specified AEP DIMM is currently configured.
 * @return Returns one of the following @link #return_code return_codes: @endlink @n
 *		NVM_ERR_API_NOT_SUPPORTED @n
 */
extern NVM_API int nvm_dump_config(const NVM_UID device_uid, const NVM_PATH file, const NVM_SIZE file_len, const NVM_BOOL append);

/***
 * @brief Store the configuration settings of how the AEP DIMM capacity
 * is currently provisioned to a file in order to duplicate the
 * configuration elsewhere.
 * @param file
 *              The absolute file path in which to store the configuration data.
 * @param file_len
 *              String length of file, should be < #NVM_PATH_LEN.
 * @pre The caller has administrative privileges.
 * @pre The specified AEP DIMM is manageable by the host software.
 * @pre The specified AEP DIMM is currently configured.
 * @return Returns one of the following @link #return_code return_codes: @endlink @n
 *		  NVM_SUCCESS @n
 *              NVM_ERR_DUMP_FILE_OPERATION_FAILED @n
 *              NVM_ERR_UNKNOWN @n
 */
extern NVM_API int nvm_dump_goal_config(const NVM_PATH file, const NVM_SIZE file_len);

/***
 * @deprecate Please use  nvm_load_goal_config
 * Modify how the AEP DIMM capacity is provisioned by the BIOS on the
 * next reboot by applying the configuration goal previously stored in the
 * specified file with @link nvm_dump_config @endlink.
 * @param device_uid
 *              The AEP DIMM identifier.
 * @param file
 *              The absolute file path containing the region configuration goal to load.
 * @param file_len
 *              String length of file, should be < #NVM_PATH_LEN.
 * @pre The caller has administrative privileges.
 * @pre The specified AEP DIMM is manageable by the host software.
 * @pre Any existing namespaces created from capacity on the
 *              AEP DIMM must be deleted first.
 * @pre If the configuration goal contains any app direct memory,
 *              all AEP DIMMs that are part of the interleave set must be included in the file.
 * @pre The specified AEP DIMM must be >= the total capacity of the AEP DIMM
 *              specified in the file.
 * @return Returns one of the following @link #return_code return_codes: @endlink @n
 *		NVM_ERR_API_NOT_SUPPORTED @n
 */
extern NVM_API int nvm_load_config(const NVM_UID device_uid, const NVM_PATH file, const NVM_SIZE file_len);

/***
 * @brief Modify how the AEP DIMM capacity is provisioned by the BIOS on the
 * next reboot by applying the configuration goal previously stored in the
 * specified file with @link nvm_dump_config @endlink.
 * @param file
 *              The absolute file path containing the region configuration goal to load.
 * @param file_len
 *              String length of file, should be < NVM_PATH_LEN.
 * @pre The caller has administrative privileges.
 * @pre The specified AEP DIMM is manageable by the host software.
 * @pre Any existing namespaces created from capacity on the
 *              AEP DIMM must be deleted first.
 * @pre If the configuration goal contains any app direct memory,
 *              all AEP DIMMs that are part of the interleave set must be included in the file.
 * @pre The specified AEP DIMM must be >= the total capacity of the AEP DIMM
 *              specified in the file.
 * @return Returns one of the following @link #return_code return_codes: @endlink @n
 *		  NVM_SUCCESS @n
 *              NVM_ERR_CREATE_GOAL_NOT_ALLOWED @n
 *              NVM_ERR_UNKNOWN @n
 */
extern NVM_API int nvm_load_goal_config(const NVM_PATH file, const NVM_SIZE file_len);

/***
 * @deprecated Not supported
 * @brief Retrieve the number of namespaces allocated from regions of AEP DIMM
 * capacity in the host server.
 * @pre The caller must have administrative privileges.
 * @remarks This method should be called before #nvm_get_namespaces.
 * @return Returns the number of namespaces on success or
 * one of the following @link #return_code return_codes: @endlink @n
 *              -1 @n
 */
extern NVM_API int nvm_get_namespace_count();



/***
 * @deprecated Not supported
 * @brief Determine if any existing namespaces of the specified type
 * utilize capacity from the specified device.
 * @param device_uid
 *              The AEP DIMM identifier.
 * @param[in] type
 *              The type of namespace, use UNKNOWN to check for all namespaces.
 * @pre The caller must have administrative privileges.
 * @return Returns 1 if namespaces exist or
 * one of the following @link #return_code return_codes: @endlink @n
 *              -1 @n
 */
extern NVM_API int nvm_get_device_namespace_count(const NVM_UID uid, const enum namespace_type type);


/***
 * @deprecated Not supported
 * @brief Retrieve discovery information about each namespace allocated from regions
 * of AEP DIMM capacity in the host server.
 * @param[in,out] p_namespaces
 *              An array of #namespace_discovery structures allocated by the caller.
 * @param[in] count
 *              The size of the array.
 * @pre The caller must have administrative privileges.
 * @remarks To allocate the array of #namespace_discovery structures,
 * call #nvm_get_namespace_count before calling this method.
 * @return Returns the number of namespaces on success
 * or one of the following @link #return_code return_codes: @endlink @n
 *              NVM_ERR_API_NOT_SUPPORTED @n
 */
extern NVM_API int nvm_get_namespaces(struct namespace_discovery *p_namespaces, const NVM_UINT8 count);

/***
 * @deprecated Not supported
 * @brief Retrieve detailed information about the specified namespace.
 * @param[in] namespace_uid
 *              The namespace identifier.
 * @param[in,out] p_namespace
 *              A pointer to an #namespace_details structure allocated by the caller.
 * @pre The caller must have administrative privileges.
 * @return Returns one of the following @link #return_code return_codes: @endlink @n
 *              NVM_ERR_API_NOT_SUPPORTED @n
 */
extern NVM_API int nvm_get_namespace_details(const NVM_UID namespace_uid, struct namespace_details *p_namespace);

/***
 * @deprecated Not supported
 * @brief Given a struct namespace_create_settings, adjust the block count so that the namespace
 * size meets alignment requirements for a namespace creation request.
 * @param[in] region_uid
 *              The region identifier to create the namespace from.
 * @param[in,out] p_settings
 *              The creation settings for a namespace
 * @param[in] *p_format
 *              An optionally supplied pointer to an interleave_format struct allocated by the caller to
 *              specify desired interleave set format for the namespace
 *      @pre The caller has administrative privileges
 *      @remarks  the namespace size is adjusted for alignment only, no other size requirements
 *      are considered
 *      @return Returns one of the  following @link #return_code return_codes: @endlink @n
 *		NVM_ERR_API_NOT_SUPPORTED @n
 */
extern NVM_API int nvm_adjust_create_namespace_block_count(const NVM_UID region_uid, struct namespace_create_settings *p_settings, const struct interleave_format *p_format);

/***
 * @deprecated Not supported
 * @brief Given namespace_uid and a block count, adjust the block count so that the namespace
 * size meets alignment requirements for a namespace modification request.
 * @param[in] namespace_uid
 *              The namespace that is to be modified.
 * @param[in,out] block_count
 *              The requested block count for the namespace
 *      @pre The caller has administrative privileges
 *      @remarks  the namespace size is adjusted for alignment only, no other size requirements
 *      are considered
 *      @return Returns one of the  following @link #return_code return_codes: @endlink @n
 *		NVM_ERR_API_NOT_SUPPORTED @n
 */
extern NVM_API int nvm_adjust_modify_namespace_block_count(const NVM_UID namespace_uid, NVM_UINT64 *p_block_count);

/***
 * @deprecated Not supported
 * @brief Create a new namespace on the specified persistent memory region of AEP DIMM capacity.
 * @param[in,out] p_namespace_uid
 *              The namespace identifier of the newly created namespace.
 * @param[in] region_uid
 *              The region identifier to create the namespace from.
 * @param[in] p_settings
 *              A pointer to an #namespace_create_settings structure describing the new
 *              namespace settings.
 * @param[in] p_format
 *              An optionally supplied pointer to an #interleave_format structure describing
 *              the interleave set format for the namespace.
 * @pre The caller has administrative privileges.
 * @remarks block_size * block_size must result in a size that is less than
 * or equal to the available capacity of the region.
 * @return Returns one of the following @link #return_code return_codes: @endlink @n
 *              NVM_ERR_API_NOT_SUPPORTED @n
 */
extern NVM_API int nvm_create_namespace(NVM_UID *p_namespace_uid, const NVM_UID region_uid, struct namespace_create_settings *p_settings, const struct interleave_format *p_format, const NVM_BOOL allow_adjustment);

/***
 * @deprecated Not supported
 * @brief Change the friendly_name setting on the specified namespace.
 * @param[in] namespace_uid
 *              The namespace identifier.
 * @param[in] name
 *              A c-style string that contains the friendly name of the namespace.
 * @pre The caller has administrative privileges.
 * @return Returns one of the following @link #return_code return_codes: @endlink @n
 *              NVM_ERR_API_NOT_SUPPORTED @n
 */
extern NVM_API int nvm_modify_namespace_name(const NVM_UID namespace_uid, const NVM_NAMESPACE_NAME name);

/***
 * @deprecated Not supported
 * @brief Change the block_count setting on the specified namespace.
 * @param[in] namespace_uid
 *              The namespace identifier.
 * @param[in] block_count
 *              The number of blocks in the new size of the namespace.
 * @pre The caller has administrative privileges.
 * @remarks block_size * block_size must result in a size that is less than
 * or equal to the available capacity of the region.
 * @return Returns one of the following @link #return_code return_codes: @endlink @n
 *              NVM_ERR_API_NOT_SUPPORTED @n
 */
extern NVM_API int nvm_modify_namespace_block_count(const NVM_UID namespace_uid, NVM_UINT64 block_count, NVM_BOOL allow_adjustment);

/***
 * @deprecated Not supported
 * @brief Change the enabled setting on the specified namespace.
 * @param[in] namespace_uid
 *              The namespace identifier.
 * @param[in] enabled
 *              NAMESPACE_ENABLE_STATE_ENABLED or NAMESPACE_ENABLE_STATE_DISABLED
 * @pre The caller has administrative privileges.
 * @return Returns one of the following @link #return_code return_codes: @endlink @n
 *              NVM_ERR_API_NOT_SUPPORTED @n
 */
extern NVM_API int nvm_modify_namespace_enabled(const NVM_UID namespace_uid, const enum namespace_enable_state enabled);

/***
 * @deprecated Not supported
 * @brief Delete an existing namespace.
 * @param[in] namespace_uid
 *              The namespace identifier.
 * @pre The caller has administrative privileges.
 * @remarks Resources allocated to the deleted namespace are returned to the
 * region from which they originated.
 * @return Returns one of the following @link #return_code return_codes: @endlink @n
 *              NVM_ERR_API_NOT_SUPPORTED @n
 */
extern NVM_API int nvm_delete_namespace(const NVM_UID namespace_uid);

/**
 * @}
 * @defgroup Support
 * @{
 */

/***
 * @brief Retrieve the native API library major version number.
 * @remarks Applications and the native API Library are not compatible if they were
 *              written against different major versions of the native API definition.
 *              For this reason, it is recommended that every application that uses the
 *              native API Library to perform the following check:
 *              if (#nvm_get_major_version() != NVM_VERSION_MAJOR)
 * @return The major version number of the library.
 */
extern NVM_API int nvm_get_major_version();

/***
 * @brief Retrieve the native API library minor version number.
 * @remarks Unless otherwise stated, every data structure, function, and description
 *              described in this document has existed with those exact semantics since version 1.0
 *              of the library.  In cases where functions have been added,
 *              the appropriate section in this document will describe the version that introduced
 *              the new feature.  Applications wishing to check for features that were added
 *		may do so by comparing the return value from #nvm_get_minor_version() against the
 *              minor number in this specification associated with the introduction of the new feature.
 * @return The minor version number of the library.
 */
extern NVM_API int nvm_get_minor_version();

/***
 * @brief Retrieve the native API library hot fix version number.
 * @return The hot fix version number of the library.
 */
extern NVM_API int nvm_get_hotfix_number();

/***
 * @brief Retrieve the native API library build version number.
 * @return The build version number of the library.
 */
extern NVM_API int nvm_get_build_number();

/**
 * @brief Retrieve native API library version as a string in the format MM.mm.hh.bbbb,
 * where MM is the major version, mm is the minor version, hh is the hotfix number
 * and bbbb is the build number.
 * @param[in,out] version_str
 *              A buffer for the version string allocated by the caller.
 * @param[in] str_len
 *              Size of the version_str buffer.  Should be NVM_VERSION_LEN.
 * @return Returns one of the following @link #return_code return_codes: @endlink @n
 *              NVM_SUCCESS @n
 *              NVM_ERR_INVALID_PARAMETER @n
 */
extern NVM_API int nvm_get_version(NVM_VERSION version_str, const NVM_SIZE str_len);

/**
 * @brief Given an numeric #return_code, retrieve a textual description of the return code in English.
 * @param[in] code
 *              The #return_code to retrieve a description of.
 * @param[in,out] description
 *              A buffer for the the textual description allocated by the caller.
 * @param[in] description_len
 *              The size of the description buffer. Should be NVM_ERROR_LEN.
 * @pre No limiting conditions apply to this function.
 * @return Returns one of the following @link #return_code return_codes: @endlink @n
 *              NVM_SUCCESS @n
 *              NVM_ERR_INVALIDPARAMETER @n
 *              NVM_ERR_BADERRORCODE @n
 */
/*
 * extern NVM_API int nvm_get_error(const enum return_code code, NVM_ERROR_DESCRIPTION description,
 *              const NVM_SIZE description_len);
 */

/***
 * @brief Collect support data into a single file to document the context of a problem
 * for offline analysis by support or development personnel.
 * @param[in] support_file
 *              Absolute file path where the support file will be stored.
 * @param[in] support_file_len
 *              String length of the file path, should be < NVM_PATH_LEN.
 * @pre The caller must have administrative privileges.
 * @post A support file exists at the path specified for debug by
 * support or development personnel.
 * @remarks The support file contains a current snapshot of the system, events logs, current
 * performance data, basic #host server information, SW version, memoryresources, system
 * capabilities, topology, sensor values and diagnostic data.
 * @remarks This operation will be attempt to gather as much information as possible about
 * the state of the system.  Therefore, it will ignore errors during the information
 * gathering process and only generate errors for invalid input parameters
 * or if the support file is not able to be generated.
 * @return Returns one of the following @link #return_code return_codes: @endlink @n
 *              NVM_SUCCESS @n
 *              NVM_ERR_UNKNOWN @n
 */
extern NVM_API int nvm_gather_support(const NVM_PATH support_file, const NVM_SIZE support_file_len);

/***
 * @deprecated Please use nvm_gather_support instead.
 * @brief Collect failure analysis data into a single file to document the context of a problem
 * for offline analysis by support or development personnel.
 * @param[in] device_uid
 *		DimmUID for which the support files will be generated.
 * @param[in] support_file
 *		Absolute file path which will be used to generate the support files. For each AEP DIMM,
 *		one or more support files may be generated at the specified destination path
 *		with the DimmUID and file number appended to the filename. Note the support
 *		files are encrypted
 * @param[in] support_file_len
 *		String length of the file path, should be < NVM_PATH_LEN.
 * @param[out] support_files
 *		Array of the absolute paths to the support files. This parameter can be NULL.
 * @pre The caller must have administrative privileges.
 * @post A support file exists at the path specified for debug by
 * support or development personnel.
 * @remarks The failure analysis file contains a dump of encrypted logs from the FW. One or more
 *			files for individual data blobs from the FW may be generated depending on the number of
 *			blobs returned by the FW.
 * @return Returns one of the following @link #return_code return_codes: @endlink @n
 *		NVM_ERR_API_NOT_SUPPORTED @n
 */
extern NVM_API int nvm_dump_device_support(const NVM_UID device_uid, const NVM_PATH support_file, const NVM_SIZE support_file_len, NVM_PATH support_files[NVM_MAX_EAFD_FILES]);

/***
 * @deprecated Please use nvm_gather_support instead.
 * @brief Capture a snapshot of the current state of the system in the configuration database
 * with the current date/time and optionally a user supplied name and description.
 * @param[in] name
 *              Optional name to describe the snapshot.
 * @param[in] name_len
 *              String length of the name. Ignored if name is NULL.
 * @pre The caller must have administrative privileges.
 * @remarks This operation will be attempt to gather as much information as possible about
 * the state of the system. Therefore, it will ignore errors during the information gathering
 * process and return the first error (if any) that it comes across.
 * @return Returns one of the following @link #return_code return_codes: @endlink @n
 *              NVM_ERR_API_NOT_SUPPORTED @n
 */
extern NVM_API int nvm_save_state(const char *name, const NVM_SIZE name_len);

/***
 * @deprecated To purge the last saved state please remove the file generated by nvm_gather_support
 * @brief Clear any snapshots generated with #nvm_save_state from the configuration database.
 * @pre The caller must have administrative privileges.
 * @return Returns one of the following @link #return_code return_codes: @endlink @n
 *              NVM_ERR_API_NOT_SUPPORTED @n
 */
extern NVM_API int nvm_purge_state_data();

/***
 * @deprecated Not supported
 * @brief Load a simulator database file.  A simulator may be used to emulate
 * a server with 0 or more devices for debugging purposes.
 * @param[in] simulator
 *              Absolute path to a simulator file.
 * @param[in] simulator_len
 *              String length of the simulator file path, should be < NVM_PATH_LEN.
 * @pre Only available for simulated builds of the Native API library, all other
 * builds will return NVM_ERR_NOTSUPPORTED.
 * @remarks If another file is currently open, it will remove it before
 * loading the current file.
 * @return Returns one of the following @link #return_code return_codes: @endlink @n
 *              NVM_ERR_API_NOT_SUPPORTED @n
 */
extern NVM_API int nvm_add_simulator(const NVM_PATH simulator, const NVM_SIZE simulator_len);

/***
 * @deprecated Not supported
 * @brief Remove a simulator database file previously added using #nvm_add_simulator.
 * @pre Only available for simulated builds of the Native API library, all other
 * builds will return NVM_ERR_NOTSUPPORTED.
 * @pre A simulator file was previously loaded using #nvm_add_simulator.
 * @return Returns one of the following @link #return_code return_codes: @endlink @n
 *              NVM_ERR_API_NOT_SUPPORTED @n
 */
extern NVM_API int nvm_remove_simulator();

/***
 * @brief Retrieve the current level of debug logging performed on the specified AEP DIMM.
 * @param[in] device_uid
 *              The device identifier.
 * @param[out,in] p_log_level
 *              A buffer for the log_level, allocated by the caller.
 * @return Returns one of the following @link #return_code return_codes: @endlink @n
 *              NVM_SUCCESS @n
 *              NVM_ERR_INVALIDPARAMETER @n
 *              NVM_ERR_UNKNOWN @n
 */
extern NVM_API int nvm_get_fw_log_level(const NVM_UID device_uid, enum fw_log_level *p_log_level);

/***
 * @brief Set the current level of debug logging performed on the specified AEP DIMM.
 * @param[in] device_uid
 *              The device identifier.
 * @param[in] log_level
 *              The firmware log level.
 * @return Returns one of the following @link #return_code return_codes: @endlink @n
 *              NVM_SUCCESS @n
 *              NVM_ERR_UNKNOWN @n
 */
extern NVM_API int nvm_set_fw_log_level(const NVM_UID device_uid, const enum fw_log_level log_level);

/***
 * @brief Inject an error into the device specified for debugging purposes.
 * @param[in] device_uid
 *              The device identifier.
 * @param[in] p_error
 *              A pointer to a #device_error structure containing the injected
 *              error information allocated by the caller.
 * @pre The caller has administrative privileges.
 * @pre The device is manageable.
 * @pre This interface is only supported by the underlying AEP DIMM firmware when it's in a
 * debug state.
 * @return Returns one of the following @link #return_code return_codes: @endlink @n
 *              NVM_SUCCESS @n
 *              NVM_ERR_INVALIDPARAMETER @n
 *              NVM_ERR_UNKNOWN @n
 */
extern NVM_API int nvm_inject_device_error(const NVM_UID device_uid, const struct device_error *p_error);

/***
 * @brief Clear an injected error into the device specified for debugging purposes.
 * @param[in] device_uid
 *              The device identifier.
 * @param[in] p_error
 *              A pointer to a #device_error structure containing the injected
 *              error information allocated by the caller.
 * @pre The caller has administrative privileges.
 * @pre The device is manageable.
 * @pre This interface is only supported by the underlying AEP DIMM firmware when it's in a
 * debug state.
 * @return Returns one of the following @link #return_code return_codes: @endlink @n
 *              NVM_SUCCESS @n
 *              NVM_ERR_INVALIDPARAMETER @n
 *              NVM_ERR_UNKNOWN @n
 */
extern NVM_API int nvm_clear_injected_device_error(const NVM_UID device_uid, const struct device_error *p_error);

/***
 * @brief Run a diagnostic test on the device specified.
 * @param[in] device_uid
 *              The device identifier.
 * @param[in] p_diagnostic
 *              A pointer to a #diagnostic structure containing the
 *              diagnostic to run allocated by the caller.
 * @param[in,out] p_results
 *              The number of diagnostic failures. To see full results use #nvm_get_events
 * @pre The caller has administrative privileges.
 * @pre The device is manageable.
 * @return Returns one of the following @link #return_code return_codes: @endlink @n
 *              NVM_SUCCESS @n
 *              NVM_ERR_INVALIDPARAMETER @n
 *              NVM_ERR_UNKNOWN @n
 */
extern NVM_API int nvm_run_diagnostic(const NVM_UID device_uid, const struct diagnostic *p_diagnostic, NVM_UINT32 *p_results);

/***
 * @brief Set the user preference config value in AEP DIMM software.
 * @param[in] key
 *              The property key.
 * @param[in] value
 *              The property value.
 * @return Returns one of the following @link #return_code return_codes: @endlink @n
 *              NVM_SUCCESS @n
 *              NVM_ERR_UNKNOWN @n
 */
extern NVM_API int nvm_set_user_preference(const NVM_PREFERENCE_KEY key, const NVM_PREFERENCE_VALUE value);

/***
 * @brief Clear namespace label storage area in PCD on the specified AEP DIMM.
 * @param[in] device_uid
 *              The device identifier.
 * @return Returns one of the following @link #return_code return_codes: @endlink @n
 *              NVM_SUCCESS @n
 *              NVM_ERR_DIMM_NOT_FOUND @n
 *              NVM_ERR_UNKNOWN @n
 */
extern NVM_API int nvm_clear_dimm_lsa(const NVM_UID device_uid);

/**
 * @}
 * @defgroup Logging
 * @{
 */

/***
 * @brief Determine if the native API debug logging is enabled.
 * @pre The caller must have administrative privileges.
 * @return Returns true (1) if debug logging is enabled and false (0) if not,
 * or one of the following @link #return_code return_codes: @endlink @n
 *              NVM_SUCCESS @n
 *              NVM_ERR_UNKNOWN @n
 */
extern NVM_API int nvm_debug_logging_enabled();

/***
 * @brief Toggle whether the native API library performs debug logging.
 * @param[in] debug @n
 *              0: Log native API errors only. @n
 *              1: Log all native API traces. @n
 * @pre The caller must have administrative privileges.
 * @remarks By default, the native API library starts logging errors only.
 * @remarks Debug logging may impact native API library performance depending
 * on the workload of the library.  It's recommended that debug logging is only
 * turned on during troubleshooting or debugging.
 * @remarks Changing the debug log level is persistent.
 * @return Returns one of the following @link #return_code return_codes: @endlink @n
 *              NVM_SUCCESS @n
 *              NVM_ERR_UNKNOWN @n
 */
extern NVM_API int nvm_toggle_debug_logging(const NVM_BOOL enabled);

/***
 * @brief Clear any debug logs captured by the native API library.
 * @pre The caller must have administrative privileges.
 * @return Returns one of the following @link #return_code return_codes: @endlink @n
 *              NVM_SUCCESS @n
 *              NVM_ERR_UNKNOWN @n
 */
extern NVM_API int nvm_purge_debug_log();

/***
 * @deprecated please use: nvm_get_number_of_debug_logs
 * @brief Retrieve the number of debug log entries in the native API library database.
 * @pre The caller must have administrative privileges.
 * @return Returns the number of debug log entries on success or
 * one of the following @link #return_code return_codes: @endlink @n
 *              -1 @n
 */
extern NVM_API int nvm_get_debug_log_count();

/***
 * @brief Retrieve the number of debug log entries in the native API library database.
 * @pre The caller must have administrative privileges.
 * @return Returns the number of debug log entries on success or
 * one of the following @link #return_code return_codes: @endlink @n
 *              NVM_SUCCESS @n
 *              NVM_ERR_UNKNOWN @n
 */
extern NVM_API int nvm_get_number_of_debug_logs(int *count);

/***
 * @brief Retrieve a list of stored debug log entries from the native API library database
 * @todo Does this need to support filtering?
 * @param[in,out] p_logs
 *              An array of #log structures allocated by the caller.
 * @param[in] count
 *              The size of the array.
 * @pre The caller must have administrative privileges.
 * @remarks To allocate the array of #log structures,
 * call #nvm_get_debug_log_count before calling this method.
 * @return Returns the number of debug log entries on success
 * or one of the following @link #return_code return_codes: @endlink @n
 *              NVM_ERR_INVALIDPARAMETER @n
 *              NVM_ERR_UNKNOWN @n
 */
extern NVM_API int nvm_get_debug_logs(struct nvm_log *p_logs, const NVM_UINT32 count);


/**
 * @deprecated Not supported
 * @brief Get the number of current jobs.
 * @pre The caller must have administrative privileges.
 * @return Return the number of current jobs: @endlink @n
 *              -1 @n
 */
extern NVM_API int nvm_get_job_count();



/***
 * @brief Retrieves #job information about each device in the system
 * @param[in,out] p_jobs
 *              An array of #job structures allocated by the caller.
 * @param[in] count
 *              The size of the array.
 * @pre The caller must have administrative privileges.
 * @return Returns the number of devices on success
 * or one of the following @link #return_code return_codes: @endlink @n
 *              -1 @n
 */
extern NVM_API int nvm_get_jobs(struct job *p_jobs, const NVM_UINT32 count);

/**
 * @brief Initialize a new context
 */
extern NVM_API int nvm_create_context();

/**
 * @brief Clean up the current context
 */
extern NVM_API int nvm_free_context(const NVM_BOOL force);

/**
 * @brief A device pass-through command. Refer to the FW specification
 * for specific details about the individual fields
 */
struct device_pt_cmd {
	NVM_UINT8	opcode;                         ///< Command opcode.
	NVM_UINT8	sub_opcode;                     ///<  Command sub-opcode.
	NVM_UINT32	input_payload_size;             ///<  Size of the input payload.
	void *		input_payload;                  ///< A pointer to the input payload buffer.
	NVM_UINT32	output_payload_size;            ///< Size of the output payload.
	void *		output_payload;                 ///< A pointer to the output payload buffer.
	NVM_UINT32	large_input_payload_size;       ///< Size of the large input payload.
	void *		large_input_payload;            ///< A pointer to the large input payload buffer.
	NVM_UINT32	large_output_payload_size;      ///< Size of the large output payload.
	void *		large_output_payload;           ///< A pointer to the large output payload buffer.
	int		result;                         ///< Return code from the pass through command
};

/**
 * @brief Send a firmware command directly to the specified device without
 * checking for valid input.
 * @param device_uid
 *              The device identifier.
 * @param p_cmd
 *              A pointer to a @link #device_pt_command @endlink structure defining the command to send.
 * @return Returns one of the following @link #return_code return_codes: @endlink @n
 *              NVM_SUCCESS @n
 *              NVM_ERR_INVALIDPARAMETER @n
 *              NVM_ERR_INVALIDPERMISSIONS @n
 *              NVM_ERR_NOTSUPPORTED @n
 *              NVM_ERR_NOMEMORY @n
 *              NVM_ERR_UNKNOWN @n
 *              NVM_ERR_BADDEVICE @n
 *              NVM_ERR_DRIVERFAILED
 *              NVM_ERR_DATATRANSFERERROR @n
 *              NVM_ERR_DEVICEERROR @n
 *              NVM_ERR_DEVICEBUSY @n
 *              NVM_ERR_NOSIMULATOR (Simulated builds only)
 */
extern NVM_API int nvm_send_device_passthrough_cmd(const NVM_UID device_uid, struct device_pt_cmd *p_cmd);


extern NVM_API int nvm_get_fw_error_log_entry_cmd(const NVM_UID device_uid, const unsigned short seq_num, const unsigned char log_level, const unsigned char log_type, void *buffer, unsigned int buffer_size);


NVM_API int nvm_get_fw_err_log_stats(const NVM_UID device_uid, struct device_error_log_status *error_log_stats);

NVM_API void nvm_sync_lock_api();
NVM_API void nvm_sync_unlock_api();

#ifdef __cplusplus
}
#endif

#endif  /* _NVM_MANAGEMENT_H_ */
