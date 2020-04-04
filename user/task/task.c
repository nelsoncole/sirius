/*
 * File Name: task.c
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
 
#include <io.h>
#include <ctype.h>


const char *clock_table[60] = {
"00","01","02","03","04","05","06","07","08","09",
"10","11","12","13","14","15","16","17","18","19",
"20","21","22","23","24","25","26","27","28","29",
"30","31","32","33","34","35","36","37","38","39",
"40","41","42","43","44","45","46","47","48","49",
"50","51","52","53","54","55","56","57","58","59",
};


UINTN _cpy(CHAR8 *Destination,const char *Source,UINTN count)
{
    	UINTN i;
	CHAR8 *des = Destination;
	CHAR8 *src =(char*) Source;

	for(i =0;i <count;i++)
	*des++ = *src++;
	
    	return i;
}

INTN main() {



	UINT32 *p = (UINT32*)0x10001124;
	unsigned char *clock = (UINT8*)(*p++);

	
	CHAR8 *_st, string_clock[8] = "00:00:00";

	GW_HAND *task_handle = CreateTask(0,0,1024,32, 
	GW_STYLE(FORE_GROUND(GW_GRAY) | BACK_GROUND(GW_DARKGRAY)),GW_FLAG_VISIBLE);


	GW_HAND *box_handle = CreateBox(task_handle,task_handle->Area.Width - 8*10,8,8*8,task_handle->Area.Height - 2,
	GW_STYLE(FORE_GROUND(GW_LIGHTGREEN) | BACK_GROUND(GW_DARKGRAY)),GW_FLAG_VISIBLE);

	


	

	while(TRUE) {

		_st = string_clock;


	
		_cpy(_st,clock_table[clock[2] &0x1f],2);
		_st++; _st++; _st++;
		
		_cpy(_st,clock_table[clock[1] &0x3f],2);
		_st++; _st++; _st++;

		_cpy(_st,clock_table[clock[0] &0x3f],2);
	
		
		_st = string_clock;

		TASK_Clearscreen();

		UpdateTask(task_handle);

		UpdateBoxDrawString(box_handle,_st);
	
		TASK_RefreshRate();	

	}


	return 0;

}
