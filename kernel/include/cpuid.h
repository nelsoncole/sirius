/*
 * File Name: cpuid.h
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
 
#ifndef __CPUID_H__
#define __CPUID_H__

enum {
	
	// Basic CPUID Information
	CPUID_GET_VENDOR_STRING,
	CPUID_GET_FEATURES,
	CPUID_GET_TLB,
	CPUID_GET_SERIAL,
	// Extended Function CPUID Information
	CPUID_GET_EXTENDED = 0x80000000,
	CPUID_GET_EXTENDED_FEATURES,
	CPUID_GET_BRAND_STRING,
	CPUID_GET_BRAND_STRING_MORE,
	CPUID_GET_BRAND_STRING_END
	
	
	
}CPUID_REQUEST;


UINTN 
cpuid_vendor(OUT CHAR8 *string);
UINTN 
cpuid_processor_brand(OUT CHAR8 *string);


#endif



// Volume 2: Instruction Set Reference, A-Z p471, Volume 3 p2701, Volume 4: Model-Specific Registers p4413
