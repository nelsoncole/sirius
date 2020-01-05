/*
 * File Name: nic.h
 *
 *
 * BSD 3-Clause License
 * 
 * Copyright (c) 2019, nelsoncole
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 * 
 * 1. Redistributions of source code must retain the above copyright notice, this
 *    list of conditions and the following disclaimer.
 * 
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 
 * 3. Neither the name of the copyright holder nor the names of its
 *    contributors may be used to endorse or promote products derived from
 *   this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */
#ifndef __NIC_H__
#define __NIC_H__


//Status
#define NIC_STATUS_PIF 		0x80
#define NIC_STATUS_IPCS 	0x40
#define NIC_STATUS_TCPCS	0x20 
#define NIC_STATUS_RSV 		0x10
#define NIC_STATUS_VP		0x8 
#define NIC_STATUS_IXSM 	0x4
#define NIC_STATUS_EOP		0x2 
#define NIC_STATUS_DD		0x1

// Errors
#define NIC_ERROR_RXE 		0x80
#define NIC_ERROR_IPE 		0x40
#define NIC_ERROR_TCPE 		0x20
#define NIC_ERROR_RSV_CXE 	0x10
#define NIC_ERROR_RSV 		0x8
#define NIC_ERROR_SEQ_RSV 	0x4
#define NIC_ERROR_SE_RSV 	0x2
#define NIC_ERROR_CE		0x1

typedef volatile struct _RDESC {
	UINT64	length : 16;
	UINT64	packet_checksum : 16;
	UINT64	status : 8;
	UINT64	errors : 8;
	UINT64	especial : 16;

}__attribute__((packed)) RDESC;



#endif
