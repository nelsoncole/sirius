/*
 * File Name: taskbar.c
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

#include <gx.h>

const char *clock_table[60]/*[2]*/ = {
"00","01","02","03","04","05","06","07","08","09",
"10","11","12","13","14","15","16","17","18","19",
"20","21","22","23","24","25","26","27","28","29",
"30","31","32","33","34","35","36","37","38","39",
"40","41","42","43","44","45","46","47","48","49",
"50","51","52","53","54","55","56","57","58","59",
};

int cpy(char *dest,const char *src, unsigned c)
{
    	int i;
	unsigned char *udest = (unsigned char *)dest;
	unsigned char *usrc  = (unsigned char *) src;

	for(i =0;i < c;i++) *udest++ = *usrc++;

    	return i;
}

int main() {

	int i;
	unsigned char *clock = (unsigned char *)(*(unsigned int*)(0x10001124));

	char sclock[] = "00:00:00\0";

	gx_hand_t *w = create2_window (0, G->VerticalResolution - 31, G->HorizontalResolution - 1, 32, 0);

	gx_hand_t *l = create_label( w, sclock,G->HorizontalResolution - 80,4,64,28, 0);

	for ( ; ;) {

		i = clock[2]&0x1f;
		cpy(sclock,clock_table[i], 2);

		i = clock[1]&0x3f;
		cpy(sclock + 3,clock_table[i], 2);

		i = clock[0]&0x3f;
		cpy(sclock + 6,clock_table[i], 2);

		update_str_label(l, sclock);

	}

	return 0;
}
