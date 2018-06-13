/*
 * Copyright (c) 2018, Intel Corporation.
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*
 * @file nvm_types.h
 * @brief This file defines standard types used in the Native Management API.
 */

#ifndef NVM_TYPES_H_
#define NVM_TYPES_H_

#include <stddef.h>
#include <limits.h>
#include <NvmStatusValues.h>

#ifdef _MSC_VER
#include <stdlib.h>
#define PATH_MAX _MAX_PATH
#endif

#define DIMM_ACPI_EVENT_SMART_HEALTH_MASK 1 << ACPI_SMART_HEALTH
#define DIMM_ACPI_EVENT_UNCORRECTABLE_MASK  1 << ACPI_UNCORRECTABLE

/*
byte swap
*/
#define SWAP_BYTES_U16(u16) (((u16 >> 8)& 0x00FF) | ((u16 << 8)& 0xFF00))

#define	NVM_PRODUCT_NAME	"Intel DIMM Gen 1 Software\0"
#define	NVM_DIMM_NAME_LONG	"Intel DIMM Gen 1"
#ifndef NVM_DIMM_NAME
#define	NVM_DIMM_NAME	"Apache Pass DIMM"
#endif // NVM_DIMM_NAME
#define	NVM_SYSLOG_SOURCE	"NVM_MGMT"
#define	NVM_DEFAULT_NAMESPACE_NAME	"NvDimmVol"
#define	NVM_SYSTEM	"Intel DIMM Gen 1"
#define	NVM_INTEL_VENDOR_ID	0x8980
#define	NVM_DIMM_SUBSYSTEM_VENDOR_ID SWAP_BYTES_U16(0x8980)
#define NVM_DIMM_SUBSYSTEM_DEVICE_ID_1 SWAP_BYTES_U16(0x0979)
#define NVM_DIMM_SUBSYSTEM_DEVICE_ID_2 SWAP_BYTES_U16(0x097A)
#define NVM_DIMM_SUBSYSTEM_DEVICE_ID_3 SWAP_BYTES_U16(0x097B)

#define NVM_INTEL_VENDOR_ID_STR "0x8089"
#define NVM_INTEL_DEVICE_ID_STR "0x979"
#define NVM_INTEL_SUBSYSTEM_VENDOR_ID_STR "0x8980"
#define NVM_DIMM_SUBSYSTEM_DEVICE_ID_1_STR "0x979"

#define NVM_COMPUTERNAME_LEN 256 // Length of host string
#define NVM_OSNAME_LEN  256 // Length of host OS string
#define NVM_OSVERSION_LEN 256 // Length of host OS version number
#define NVM_PROCESSOR_LEN 256 // Length of host processor string
#define NVM_VERSION_LEN 25  // Length of version string
#define NVM_ERROR_LEN 256 // Length of return code description
#define NVM_MAX_HANDLE_LEN 11 // Max length of a uint32 in decimal + '\0'
#define NVM_MAX_UID_LEN 22 // Max Length of Unique ID
#define NVM_MAX_DIMMID_STR_LEN 7 // Max length of Dimm ID string
#define NVM_SOCKET_MANUFACTURER_LEN 16 // Socket manufacturer string length
#define NVM_MANUFACTURER_LEN  2 // Number of bytes in the manufacturer ID
#define NVM_MANUFACTURERSTR_LEN 256 // Manufacturer string length
#define NVM_SERIAL_LEN  4 // Number of bytes in the serial number
#define NVM_SERIALSTR_LEN 11 // Serial number string length including '\0' and prefix "0x"
#define NVM_PASSPHRASE_LEN  32 // Length of security passphrase
#define NVM_MAX_DEVICE_SENSORS  11 // Maximum number of sensors
#define NVM_EVENT_MSG_LEN 1024 // Length of event message string
#define NVM_EVENT_ARG_LEN 1024 // Length of event argument string
#define NVM_MAX_EVENT_ARGS  3 // Maximum number of event arguments
#define NVM_FILTER_ON_TYPE  0x01 // Filter on event type
#define NVM_FILTER_ON_SEVERITY  0x02 // Filter on event severity
#define NVM_FILTER_ON_CODE  0x04 // Filter on code
#define NVM_FILTER_ON_UID 0x08 // Filter on device uid
#define NVM_FILTER_ON_AFTER 0x10 // Filter on time after
#define NVM_FILTER_ON_BEFORE  0x20 // Filter on time before
#define NVM_FILTER_ON_EVENT 0x40 // Filter on event ID
#define NVM_FILTER_ON_AR  0x80 // Filter on action required
#define NVM_PATH_LEN  PATH_MAX // Max length of file or directory path string (OS specific)
#define NVM_DEVICE_LOCATOR_LEN  128 // Length of the device locator string
#define NVM_BANK_LABEL_LEN  128 // Length of the bank label string
#define NVM_NAMESPACE_NAME_LEN  64 // Length of namespace friendly name string
#define NVM_NAMESPACE_PURPOSE_LEN 64 // Length of namespace purpose string
#define NVM_MAX_SOCKETS 4 // Maximum number of sockets per system
#define NVM_MAX_SOCKET_DIGIT_COUNT  4 // Maximum number of digits in a socket count
#define NVM_MEMORY_CONTROLLER_CHANNEL_COUNT 3 // expected number of channels per iMC
#define NVM_MAX_INTERLEAVE_SETS_PER_DIMM  2 // Max number of App Direct interleave sets per DIMM
#define NVM_MAX_POOLS_PER_NAMESPACE 128 // Maximum number of pools for a namespace
#define NVM_PART_NUM_LEN  21 // Length of device part number string
// TODO -guessing and interleave formats size. HSD-20363 should address this.
#define NVM_INTERLEAVE_FORMATS  32 // Maximum number of memory interleave formats
#define NVM_MAX_DEVICES_PER_POOL  128 // Maximum number of DIMMs that can be used in a pool
// This number of devices that can go on a socket may go up on future architectures
// so this is something to keep an eye on. 24 should be good for a while
#define NVM_MAX_DEVICES_PER_SOCKET  24 // Maximum number of dimms that can be on a socket
#define NVM_LOG_MESSAGE_LEN 2048 // Length of log message string
#define NVM_MAX_BLOCK_SIZES_PER_POOL  16
#define NVM_MAX_BLOCK_SIZES 16 // maximum number of block sizes supported by the driver
#define NVM_MAX_TOPO_SIZE 96 // Maximum number of DIMMs possible for a given memory topology
#define NVM_THRESHOLD_STR_LEN 1024 // Max threshold string value len
#define NVM_VOLATILE_POOL_SOCKET_ID -1 // Volatile pools are system wide and not tied to a socket
#define NVM_MAX_CONFIG_LINE_LEN 512 // Maximum line size for config data in a dump file
#define NVM_DIE_SPARES_MAX  4 // Maximum number of spare dies
#define NVM_COMMIT_ID_LEN 41
#define NVM_BUILD_CONFIGURATION_LEN 17
#define NVM_MAX_IFCS_PER_DIMM 9
#define NVM_REQUEST_MAX_AVAILABLE_BLOCK_COUNT 0
#define NVM_MIN_EAFD_FILES 1
#define NVM_MAX_EAFD_FILES 10
#define NVM_TRUE 1
#define NVM_FALSE 0
#define NVM_ARG0 0
#define NVM_ARG1 1
#define NVM_ARG2 2
#define MAX_IS_PER_DIMM 2

typedef size_t NVM_SIZE; // String length size
typedef char NVM_INT8; // 8 bit signed integer
typedef signed short NVM_INT16; // 16 bit signed integer
typedef signed int  NVM_INT32; // 32 bit signed integer
typedef unsigned char NVM_BOOL; // 8 bit unsigned integer as a boolean
typedef unsigned char NVM_UINT8; // 8 bit unsigned integer
#define NVM_UINT8_DEFINED
typedef unsigned short NVM_UINT16; // 16 bit unsigned integer
typedef unsigned int NVM_UINT32; // 32 bit unsigned integer
typedef unsigned long long NVM_UINT64; // 64 bit unsigned integer
typedef long long NVM_INT64; // 64 bit integer
typedef float NVM_REAL32; // 32 bit floating point number
typedef char NVM_VERSION[NVM_VERSION_LEN]; // Version number string
typedef char NVM_ERROR_DESCRIPTION[NVM_ERROR_LEN]; // Return code description
typedef char NVM_UID[NVM_MAX_UID_LEN]; // Unique ID
typedef char NVM_PASSPHRASE[NVM_PASSPHRASE_LEN]; // Security passphrase
typedef char NVM_EVENT_MSG[NVM_EVENT_MSG_LEN]; // Event message string
#define NVM_EVENT_MSG_DEFINED
typedef char NVM_EVENT_ARG[NVM_EVENT_ARG_LEN]; // Event argument string
typedef char NVM_PATH[NVM_PATH_LEN]; // TFile or directory path
typedef unsigned char NVM_MANUFACTURER[NVM_MANUFACTURER_LEN]; // Manufacturer identifier
typedef unsigned char NVM_SERIAL_NUMBER[NVM_SERIAL_LEN]; // Serial Number
typedef char NVM_NAMESPACE_NAME[NVM_NAMESPACE_NAME_LEN]; // Namespace name
typedef char NVM_PREFERENCE_KEY[NVM_THRESHOLD_STR_LEN]; // Config value property name
typedef char NVM_PREFERENCE_VALUE[NVM_THRESHOLD_STR_LEN]; // Config value property value
typedef char NVM_LOG_MSG[NVM_LOG_MESSAGE_LEN]; // Event message string

typedef union
{
  struct device_handle_parts
  {
    NVM_UINT32 mem_channel_dimm_num:4;
    NVM_UINT32 mem_channel_id:4;
    NVM_UINT32 memory_controller_id:4;
    NVM_UINT32 socket_id:4;
    NVM_UINT32 node_controller_id:12;
    NVM_UINT32 rsvd:4;
  } parts;
  NVM_UINT32 handle;
} NVM_NFIT_DEVICE_HANDLE;

/*
 * Type of region.
 */
enum region_type
{
  REGION_TYPE_UNKNOWN = 0,
  REGION_TYPE_PERSISTENT = 1, // REGION type is non-mirrored App Direct or Storage.
  REGION_TYPE_VOLATILE = 2, // Volatile.
  REGION_TYPE_PERSISTENT_MIRROR = 3, // Persistent.
};

/*
 * Rolled-up health of the underlying AEP DIMMs from which the REGION is created.
 */
enum region_health
{
  REGION_HEALTH_UNKNOWN = 0, // The REGION health cannot be determined.
  REGION_HEALTH_NORMAL = 1, // All underlying AEP DIMM Persistent memory capacity is available.
  REGION_HEALTH_PENDING = 2, // A new memory allocation goal has been created but not applied.
  REGION_HEALTH_ERROR = 3, // There is an issue with some or all of the underlying
              // AEP DIMM capacity.
  REGION_HEALTH_LOCKED = 4 // One or more of the underlying AEP DIMMs are locked.
};

/*
 * Health of an individual interleave set.
 */
enum interleave_set_health
{
  INTERLEAVE_HEALTH_UNKNOWN = 0, // Health cannot be determined.
  INTERLEAVE_HEALTH_NORMAL = 1, // Available and underlying AEP DIMMs have good health.
  INTERLEAVE_HEALTH_DEGRADED = 2, // In danger of failure, may have degraded performance.
  INTERLEAVE_HEALTH_FAILED = 3 // Interleave set has failed and is unavailable.
};

/*
 * Type of the namespace
 */
enum namespace_type
{
  NAMESPACE_TYPE_UNKNOWN = 0, // Type cannot be determined
  NAMESPACE_TYPE_APP_DIRECT = 1 // App Direct namespace
};

/*
 * Namespace health
 */
enum namespace_health
{
  NAMESPACE_UNKNOWN_HEALTH = 0, // Namespace health cannot be determined
  NAMESPACE_HEALTH_NORMAL = 5, // Namespace is OK
  NAMESPACE_HEALTH_NONCRITICAL = 10, // Non-critical health issue
  // NAMESPACE_HEALTH_CRITICAL = 25, // Critical health issue
  NAMESPACE_HEALTH_BROKENMIRROR = 65535 // Broken mirror
};

/*
 * If the namespace is exposed to the OS
 */
enum namespace_enable_state
{
  NAMESPACE_ENABLE_STATE_UNKNOWN = 0, // Cannot be determined
  NAMESPACE_ENABLE_STATE_ENABLED = 2, // Exposed to OS
  NAMESPACE_ENABLE_STATE_DISABLED = 3 // Hidden from OS
};

/*
 * Where the capacity is allocated from for the underlying OS structures
 * if access to the AppDirect namespace capacity is supported using
 * legacy memory page protocols such as DMA/RDMA
 */
enum namespace_memory_page_allocation
{
  NAMESPACE_MEMORY_PAGE_ALLOCATION_UNKNOWN = 0, // Cannot be determined
  NAMESPACE_MEMORY_PAGE_ALLOCATION_NONE = 1, // No support to access NS using memoryPageProtocols
  NAMESPACE_MEMORY_PAGE_ALLOCATION_DRAM = 2, // Capacity is allocated from DRAM
  NAMESPACE_MEMORY_PAGE_ALLOCATION_APP_DIRECT = 3 // Capacity is allocated from the NS itself
};

/*
 * Security related definition of interleave set or namespace.
 */
enum encryption_status
{
  NVM_ENCRYPTION_OFF = 0,
  NVM_ENCRYPTION_ON = 1,
  NVM_ENCRYPTION_IGNORE = 2
};

/*
 * Erase capable definition of interleave set or namespace.
 */
enum erase_capable_status
{
  NVM_ERASE_CAPABLE_FALSE = 0,
  NVM_ERASE_CAPABLE_TRUE = 1,
  NVM_ERASE_CAPABLE_IGNORE = 2
};

/*
 * Details about a specific interleave format supported by memory
 */
enum interleave_size
{
  INTERLEAVE_SIZE_NONE = 0x00,
  INTERLEAVE_SIZE_64B  = 0x01,
  INTERLEAVE_SIZE_128B = 0x02,
  INTERLEAVE_SIZE_256B = 0x04,
  INTERLEAVE_SIZE_4KB  = 0x40,
  INTERLEAVE_SIZE_1GB  = 0x80
};

enum interleave_ways
{
  INTERLEAVE_WAYS_0  = 0x00,
  INTERLEAVE_WAYS_1  = 0x01,
  INTERLEAVE_WAYS_2  = 0x02,
  INTERLEAVE_WAYS_3  = 0x04,
  INTERLEAVE_WAYS_4  = 0x08,
  INTERLEAVE_WAYS_6  = 0x10,
  INTERLEAVE_WAYS_8  = 0x20,
  INTERLEAVE_WAYS_12 = 0x40,
  INTERLEAVE_WAYS_16 = 0x80,
  INTERLEAVE_WAYS_24 = 0x100
};

enum interleave_type
{
  INTERLEAVE_TYPE_DEFAULT = 0,
  INTERLEAVE_TYPE_INTERLEAVED = 1,
  INTERLEAVE_TYPE_NOT_INTERLEAVED = 2,
  INTERLEAVE_TYPE_MIRRORED  = 3
};

struct interleave_format
{
  NVM_BOOL recommended; // is this format a recommended format
  enum interleave_size channel; // channel interleave of this format
  enum interleave_size imc; // memory controller interleave of this format
  enum interleave_ways ways; // number of ways for this format
};

enum acpi_get_event_result
{
  ACPI_EVENT_SIGNALLED_RESULT = 0,
  ACPI_EVENT_TIMED_OUT_RESULT,
  ACPI_EVENT_UNKNOWN_RESULT
};

enum acpi_event_state
{
  ACPI_EVENT_SIGNALLED = 0,
  ACPI_EVENT_NOT_SIGNALLED,
  ACPI_EVENT_UNKNOWN
};

enum acpi_event_type
{
  ACPI_SMART_HEALTH = 0,
  ACPI_UNCORRECTABLE
};


typedef struct _PMON_REGISTERS {
  /**
  This will specify whether or not to return the extra smart data along with the PMON
  Counter data.
  0x0 - No Smart Data DDRT or 3D XPoint�
  0x1 - DDRT Data only to be returned
  0x2 - 3D XPoint� Data only to be returned
  0x3 - DDRT & 3D XPoint� Data to be returned
  All other values reserved
  **/
  NVM_UINT8 SmartDataMask;
  NVM_UINT8 Reserved1[3];
  /**
  This will specify which group that is currently enabled. If no groups are enabled Group
  F will be returned
  **/
  NVM_UINT8 GroupEnabled;
  NVM_UINT8 Reserved2[18];
  NVM_UINT8 PMON4Counter[4];
  NVM_UINT8 PMON5Counter[4];
  NVM_UINT8 Reserved3[4];
  NVM_UINT8 PMON7Counter[4];
  NVM_UINT8 PMON8Counter[4];
  NVM_UINT8 PMON9Counter[4];
  NVM_UINT8 Reserved4[15];
  NVM_UINT8 PMON14Counter[4];
  NVM_UINT8 Reserved5[4];
  /**
  DDRT Reads for current power cycle
  **/
  NVM_UINT8 DDRTRD[8];
  /**
  DDRT Writes for current power cycle
  **/
  NVM_UINT8 DDRTWR[8];
  /**
  3D XPoint� Reads for current power cycle
  **/
  NVM_UINT8 SXPRD[8];
  /**
  3D XPoint� Writes for current power cycle
  **/
  NVM_UINT8 SXPWR[8];
  /**
  Current 3D XPoint� Media temp
  **/
  NVM_UINT8 MTP[2];
  /**
  Current Controller temp
  **/
  NVM_UINT8 CTP[2];
  NVM_UINT8 Reserved[19];
}PMON_REGISTERS;

#endif /* NVM_TYPES_H_ */
