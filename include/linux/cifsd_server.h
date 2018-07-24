/*
 *   Copyright (C) 2018 Samsung Electronics Co., Ltd.
 *
 *   linux-cifsd-devel@lists.sourceforge.net
 *
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation; either version 2 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program; if not, write to the Free Software
 *   Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA
 */

#ifndef _LINUX_CIFSD_SERVER_H
#define _LINUX_CIFSD_SERVER_H

#include <linux/types.h>

#define CIFSD_GENL_NAME      "CIFSD_GENL"
#define CIFSD_GENL_VERSION    0x01

#ifndef __align
#define __align		__attribute__((__aligned__(4)))
#endif

#define CIFSD_REQ_MAX_ACCOUNT_NAME_SZ	48
#define CIFSD_REQ_MAX_HASH_SZ		18
#define CIFSD_REQ_MAX_SHARE_NAME	64

struct cifsd_heartbeat {
	__u32	handle;
} __align;

struct cifsd_startup_shutdown {
	__s8	reserved[4];
} __align;

struct cifsd_login_request {
	__u32	handle;
	__s8	account[CIFSD_REQ_MAX_ACCOUNT_NAME_SZ];
} __align;

struct cifsd_login_response {
	__u32	handle;
	__u32	gid;
	__u32	uid;
	__s8	account[CIFSD_REQ_MAX_ACCOUNT_NAME_SZ];
	__u16	status;
	__u16	hash_sz;
	__s8	hash[CIFSD_REQ_MAX_HASH_SZ];
} __align;

struct cifsd_share_config_request {
	__u32	handle;
	__s8	share_name[CIFSD_REQ_MAX_SHARE_NAME];
} __align;

struct cifsd_share_config_response {
	__u32	handle;
	__u32	flags;
	__u32	veto_list_sz;
	__s8	____payload[0];
} __align;

#define CIFSD_SHARE_CONFIG_VETO_LIST(s)				\
	(char *)((void *)(s) + offsetof(struct cifsd_share_config_response,\
		____payload))

#define CIFSD_SHARE_CONFIG_PATH(s)					\
	CIFSD_SHARE_CONFIG_VETO_LIST(s) + s->veto_list_sz

struct cifsd_tree_connect_request {
	__u32	handle;
	__u16	account_flags;
	__u16	flags;
	__u64	session_id;
	__u64	connect_id;
	__s8	account[CIFSD_REQ_MAX_ACCOUNT_NAME_SZ];
	__s8	share[CIFSD_REQ_MAX_SHARE_NAME];
	__s8	peer_addr[64];
} __align;

struct cifsd_tree_connect_response {
	__u32	handle;
	__u16	status;
	__u16	connection_flags;
} __align;

struct cifsd_tree_disconnect_request {
	__u64	session_id;
	__u64	connect_id;
} __align;

struct cifsd_logout_request {
	__s8	account[CIFSD_REQ_MAX_ACCOUNT_NAME_SZ];
} __align;

/*
 * This also used as NETLINK attribute type value.
 *
 * NOTE:
 * Response message type value should be equal to
 * request message type value + 1.
 */
enum cifsd_event {
	CIFSD_EVENT_UNSPEC			= 0,
	CIFSD_EVENT_HEARTBEAT_REQUEST,
	CIFSD_EVENT_HEARTBEAT_RESPONSE,

	CIFSD_EVENT_STARTING_UP,
	CIFSD_EVENT_SHUTTING_DOWN,

	CIFSD_EVENT_LOGIN_REQUEST,
	CIFSD_EVENT_LOGIN_RESPONSE,

	CIFSD_EVENT_SHARE_CONFIG_REQUEST,
	CIFSD_EVENT_SHARE_CONFIG_RESPONSE,

	CIFSD_EVENT_TREE_CONNECT_REQUEST,
	CIFSD_EVENT_TREE_CONNECT_RESPONSE,

	CIFSD_EVENT_TREE_DISCONNECT_REQUEST,

	CIFSD_EVENT_LOGOUT_REQUEST,

	CIFSD_EVENT_MAX
};

enum CIFSD_TREE_CONN_STATUS {
	CIFSD_TREE_CONN_STATUS_OK		= 0,
	CIFSD_TREE_CONN_STATUS_NOMEM,
	CIFSD_TREE_CONN_STATUS_NO_SHARE,
	CIFSD_TREE_CONN_STATUS_NO_USER,
	CIFSD_TREE_CONN_STATUS_INVALID_USER,
	CIFSD_TREE_CONN_STATUS_HOST_DENIED,
	CIFSD_TREE_CONN_STATUS_CONN_EXIST,
	CIFSD_TREE_CONN_STATUS_TOO_MANY_CONNS,
	CIFSD_TREE_CONN_STATUS_ERROR,
};

/*
 * User config flags.
 */
#define CIFSD_USER_FLAG_INVALID		(0)
#define CIFSD_USER_FLAG_OK		(1 << 0)
#define CIFSD_USER_FLAG_BAD_PASSWORD	(1 << 1)
#define CIFSD_USER_FLAG_BAD_UID		(1 << 2)
#define CIFSD_USER_FLAG_BAD_USER	(1 << 3)
#define CIFSD_USER_FLAG_ANONYMOUS	(1 << 4)
#define CIFSD_USER_FLAG_GUEST_ACCOUNT	(1 << 5)

/*
 * Share config flags.
 */
#define CIFSD_SHARE_FLAG_INVALID		(0)
#define CIFSD_SHARE_FLAG_AVAILABLE		(1 << 0)
#define CIFSD_SHARE_FLAG_BROWSEABLE		(1 << 1)
#define CIFSD_SHARE_FLAG_WRITEABLE		(1 << 2)
#define CIFSD_SHARE_FLAG_READONLY		(1 << 3)
#define CIFSD_SHARE_FLAG_GUEST_OK		(1 << 4)
#define CIFSD_SHARE_FLAG_GUEST_ONLY		(1 << 5)
#define CIFSD_SHARE_FLAG_STORE_DOS_ATTRS	(1 << 6)
#define CIFSD_SHARE_FLAG_OPLOCKS		(1 << 7)
#define CIFSD_SHARE_FLAG_PIPE			(1 << 8)

/*
 * Tree connect request flags.
 */
#define CIFSD_TREE_CONN_FLAG_REQUEST_SMB1	(0)
#define CIFSD_TREE_CONN_FLAG_REQUEST_IPV6	(1 << 0)
#define CIFSD_TREE_CONN_FLAG_REQUEST_SMB2	(1 << 1)

/*
 * Tree connect flags.
 */
#define CIFSD_TREE_CONN_FLAG_GUEST_ACCOUNT	(1 << 0)
#define CIFSD_TREE_CONN_FLAG_READ_ONLY		(1 << 1)
#define CIFSD_TREE_CONN_FLAG_WRITABLE		(1 << 2)
#define CIFSD_TREE_CONN_FLAG_ADMIN_ACCOUNT	(1 << 3)

/*
 * DCERPC 1.1 RPC
 *
 * http://pubs.opengroup.org/onlinepubs/9629399/
 */

/* NOTE: we don't support extended headers at the moment */

#define DCERPC_PTYPE_RPC_REQUEST	0x00
#define DCERPC_PTYPE_RPC_PING		0x01
#define DCERPC_PTYPE_RPC_RESPONSE	0x02
#define DCERPC_PTYPE_RPC_FAULT		0x03
#define DCERPC_PTYPE_RPC_WORKING	0x04
#define DCERPC_PTYPE_RPC_NOCALL		0x05
#define DCERPC_PTYPE_RPC_REJECT		0x06
#define DCERPC_PTYPE_RPC_ACK		0x07
#define DCERPC_PTYPE_RPC_CL_CANCEL	0x08
#define DCERPC_PTYPE_RPC_FACK		0x09
#define DCERPC_PTYPE_RPC_CANCEL_ACK	0x0A
#define DCERPC_PTYPE_RPC_BIND		0x0B
#define DCERPC_PTYPE_RPC_BINDACK	0x0C
#define DCERPC_PTYPE_RPC_BINDNACK	0x0D
#define DCERPC_PTYPE_RPC_ALTCONT	0x0E
#define DCERPC_PTYPE_RPC_ALTCONTRESP	0x0F
#define DCERPC_PTYPE_RPC_AUTH3		0x10
#define DCERPC_PTYPE_RPC_SHUTDOWN	0x11
#define DCERPC_PTYPE_RPC_CO_CANCEL	0x12
#define DCERPC_PTYPE_RPC_ORPHANED	0x13

#define DCERPC_PFC_FIRST_FRAG	0x01  /* First fragment */
#define DCERPC_PFC_LAST_FRAG	0x02  /* Last fragment */
#define DCERPC_PFC_PENDING_CANCEL	0x04  /* Cancel was pending at sender */
#define DCERPC_PFC_RESERVED_1	0x08
#define DCERPC_PFC_CONC_MPX	0x10  /* supports concurrent multiplexing
                                        * of a single connection. */
#define DCERPC_PFC_DID_NOT_EXECUTE	0x20  /* only meaningful on `fault' packet;
                                        * if true, guaranteed call did not
                                        * execute. */
#define DCERPC_PFC_MAYBE		0x40  /* `maybe' call semantics requested */
#define DCERPC_PFC_OBJECT_UUID	0x80  /* if true, a non-nil object UUID
                                        * was specified in the handle, and
                                        * is present in the optional object
                                        * field. If false, the object field
                                        * is omitted. */

struct dcerpc_bind_hdr {
	/* start 8-octet aligned */

	/* common fields */
	__u8	rpc_vers;            /* 00:01 RPC version */
	__u8	rpc_vers_minor;      /* 01:01 minor version */
	__u8	PTYPE;               /* 02:01 bind PDU */
	__u8	pfc_flags;           /* 03:01 flags */
	__s8	packed_drep[4];      /* 04:04 NDR data rep format label*/
	__u16	frag_length;         /* 08:02 total length of fragment */
	__u16	auth_length;         /* 10:02 length of auth_value */
	__u32	call_id;             /* 12:04 call identifier */

	/* end common fields */
};

#endif /* _LINUX_CIFSD_SERVER_H */
