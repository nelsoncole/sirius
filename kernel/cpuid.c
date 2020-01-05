/*
 * File Name: cpuid.c
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
#include <os.h>

UINTN
cpuid_vendor(OUT CHAR8 *string)
{
	UINT32 *str_p = (UINT32*) string;
	
	__asm__ __volatile__("cpuid;":"=b"(str_p[0]),"=c"(str_p[2]),"=d"(str_p[1]):"a"(CPUID_GET_VENDOR_STRING));
	
	*(CHAR8*)((CHAR8*)str_p + 12) 	= 0;
	
	return 0;
}


UINTN 
cpuid_processor_brand(OUT CHAR8 *string)
{
	UINT32 *str_p = (UINT32*) string;
	__asm__ __volatile__("cpuid;":\
	"=a"(str_p[0]),"=b"(str_p[1]),"=c"(str_p[2]),"=d"(str_p[3]):\
	"a"(CPUID_GET_BRAND_STRING));
	
	__asm__ __volatile__("cpuid;":\
	"=a"(str_p[4]),"=b"(str_p[5]),"=c"(str_p[6]),"=d"(str_p[7]):\
	"a"(CPUID_GET_BRAND_STRING_MORE));
	
	__asm__ __volatile__("cpuid;":\
	"=a"(str_p[8]),"=b"(str_p[9]),"=c"(str_p[10]),"=d"(str_p[11]):\
	"a"(CPUID_GET_BRAND_STRING_END));
	
	
	return 0;
}

